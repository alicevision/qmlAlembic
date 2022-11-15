#include "AlembicEntity.hpp"
#include "IOThread.hpp"
#include "CameraLocatorEntity.hpp"
#include "PointCloudEntity.hpp"
#include <QFile>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QTechnique>

using namespace Alembic::Abc;
using namespace Alembic::AbcGeom;

namespace abcentity
{

AlembicEntity::AlembicEntity(Qt3DCore::QNode* parent)
    : Qt3DCore::QEntity(parent)
    , _ioThread(new IOThread())
    , _pointSizeParameter(new Qt3DRender::QParameter)
{
    connect(_ioThread.get(), &IOThread::finished, this, &AlembicEntity::onIOThreadFinished);
    createMaterials();
}

void AlembicEntity::setSource(const QUrl& value)
{
    if(_source == value)
        return;
    _source = value;
    loadAbcArchive();
    Q_EMIT sourceChanged();
}

void AlembicEntity::setPointSize(const float& value)
{
    if(_pointSize == value)
        return;
    _pointSize = value;
    _pointSizeParameter->setValue(value);
    _cloudMaterial->setEnabled(_pointSize > 0.0f);
    Q_EMIT pointSizeChanged();
}

void AlembicEntity::setLocatorScale(const float& value)
{
    if(_locatorScale == value)
        return;
    _locatorScale = value;
    scaleLocators();
    Q_EMIT locatorScaleChanged();
}

void AlembicEntity::scaleLocators() const
{
    for(auto* entity : _cameras)
    {
        for(auto* transform : entity->findChildren<Qt3DCore::QTransform*>())
            transform->setScale(_locatorScale);
    }
}

// private
void AlembicEntity::createMaterials()
{
    using namespace Qt3DRender;
    using namespace Qt3DExtras;

    _cloudMaterial = new QMaterial(this);
    _cameraMaterial = new QPerVertexColorMaterial(this);

    // configure cloud material
    auto effect = new QEffect();
    auto technique = new QTechnique();
    auto renderPass = new QRenderPass();
    auto shaderProgram = new QShaderProgram();

    technique->graphicsApiFilter()->setApi(QGraphicsApiFilter::RHI);
    technique->graphicsApiFilter()->setMajorVersion(1);
    technique->graphicsApiFilter()->setMinorVersion(0);
    technique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    shaderProgram->setShaderCode(QShaderProgram::Vertex, R"(#version 450

    layout(location = 0) in vec3 vertexPosition;
    layout(location = 1) in vec3 vertexColor;
    layout(location = 0) out vec3 color;

    layout(std140, binding = 0) uniform qt3d_render_view_uniforms {
        mat4 viewMatrix;
        mat4 projectionMatrix;
        mat4 uncorrectedProjectionMatrix;
        mat4 clipCorrectionMatrix;
        mat4 viewProjectionMatrix;
        mat4 inverseViewMatrix;
        mat4 inverseProjectionMatrix;
        mat4 inverseViewProjectionMatrix;
        mat4 viewportMatrix;
        mat4 inverseViewportMatrix;
        vec4 textureTransformMatrix;
        vec3 eyePosition;
        float aspectRatio;
        float gamma;
        float exposure;
        float time;
    };
    layout(std140, binding = 1) uniform qt3d_command_uniforms {
        mat4 modelMatrix;
        mat4 inverseModelMatrix;
        mat4 modelViewMatrix;
        mat3 modelNormalMatrix;
        mat4 inverseModelViewMatrix;
        mat4 mvp;
        mat4 inverseModelViewProjectionMatrix;
    };
    layout(std140, binding = 2) uniform custom_ubo {
        float pointSize;
    };

    void main()
    {
        color = vertexColor;
        gl_Position = mvp * vec4(vertexPosition, 1.0);
        gl_PointSize = max(viewportMatrix[1][1] * projectionMatrix[1][1] * pointSize / gl_Position.w, 1.0);
    }
    )");

    // set fragment shader
    shaderProgram->setShaderCode(QShaderProgram::Fragment, R"(#version 450

    layout(location = 0) out vec4 fragColor;
    layout(location = 0) in vec3 color;

    void main()
    {
        fragColor = vec4(color, 1.0);
    }
    )");

    // add a pointSize uniform
    _pointSizeParameter->setName("pointSize");
    _pointSizeParameter->setValue(_pointSize);
    _cloudMaterial->addParameter(_pointSizeParameter);

    // build the material
    renderPass->setShaderProgram(shaderProgram);
    technique->addRenderPass(renderPass);
    effect->addTechnique(technique);
    _cloudMaterial->setEffect(effect);
}

void AlembicEntity::clear()
{
    // clear entity (remove direct children & all components)
    auto entities = findChildren<QEntity*>(QString(), Qt::FindDirectChildrenOnly);
    for(auto entity : entities)
    {
        entity->setParent((QNode*)nullptr);
        entity->deleteLater();
    }
    for(auto& component : components())
        removeComponent(component);
    _cameras.clear();
    _pointClouds.clear();
}

// private
void AlembicEntity::loadAbcArchive()
{
    clear();
    if(_source.isEmpty())
    {
        setStatus(AlembicEntity::None);
        return;
    }
    setStatus(AlembicEntity::Loading);
    _ioThread->read(_source);
}

void AlembicEntity::onIOThreadFinished()
{   
    const auto& archive = _ioThread->archive();
    if(!archive.valid())
    {
        setStatus(AlembicEntity::Error);
        return;
    }
    // visit the abc tree
    try 
    {
        visitAbcObject(archive.getTop(), this);

        // store pointers to cameras and point clouds
        _cameras = findChildren<CameraLocatorEntity*>();
        _pointClouds = findChildren<PointCloudEntity*>();

        // perform initial locator scaling
        scaleLocators();

        setStatus(AlembicEntity::Ready);
    }
    catch(...)
    {
        clear();
        setStatus(AlembicEntity::Error);
    }
    _ioThread->clear();
    Q_EMIT camerasChanged();
    Q_EMIT pointCloudsChanged();
}

// private
void AlembicEntity::visitAbcObject(const Alembic::Abc::IObject& iObj, QEntity* parent)
{
    using namespace Alembic::Abc;
    using namespace Alembic::AbcGeom;

    const auto createEntity = [&](const IObject& iObject) -> BaseAlembicObject* {
        const MetaData& md = iObj.getMetaData();
        
        if(IPoints::matches(md))
        {
            IPoints points(iObj, Alembic::Abc::kWrapExisting);
            PointCloudEntity* entity = new PointCloudEntity(parent);
            entity->setData(iObj);
            entity->addComponent(_cloudMaterial);
            entity->fillArbProperties(points.getSchema().getArbGeomParams());
            entity->fillUserProperties(points.getSchema().getUserProperties());
            return entity;
        }
        else if(IXform::matches(md))
        {
            IXform xform(iObj, kWrapExisting);
            BaseAlembicObject* entity = new BaseAlembicObject(parent);
            XformSample xs;
            xform.getSchema().get(xs);
            entity->setTransform(xs.getMatrix());
            entity->fillArbProperties(xform.getSchema().getArbGeomParams());
            entity->fillUserProperties(xform.getSchema().getUserProperties());
            return entity;
        }
        else if(ICamera::matches(md))
        {
            ICamera cam(iObj, Alembic::Abc::kWrapExisting);
            CameraLocatorEntity* entity = new CameraLocatorEntity(parent);
            entity->addComponent(_cameraMaterial);
            entity->fillArbProperties(cam.getSchema().getArbGeomParams());
            entity->fillUserProperties(cam.getSchema().getUserProperties());
            return entity;
        }
        else
        {
            // fallback: create empty object to preserve hierarchy
            return new BaseAlembicObject(parent);
        }
    };

    if(_skipHidden)
    {
        // Skip objects with visibilityProperty explicitly set to hidden
        const auto& prop = iObj.getProperties();

        if(prop.getPropertyHeader(kVisibilityPropertyName))
        {
            IVisibilityProperty visibilityProperty(prop, kVisibilityPropertyName);
            if(ObjectVisibility(visibilityProperty.getValue()) == kVisibilityHidden)
                return;
        }
    }

    BaseAlembicObject* entity = createEntity(iObj);
    entity->setObjectName(iObj.getName().c_str());

    // visit children
    for(size_t i = 0; i < iObj.getNumChildren(); i++)
        visitAbcObject(iObj.getChild(i), entity);
}

} // namespace
