#include "CameraLocatorEntity.hpp"
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QObjectPicker>
#include <Qt3DCore/QTransform>

namespace abcentity
{

CameraLocatorEntity::CameraLocatorEntity(Qt3DCore::QNode* parent)
    : BaseAlembicObject(parent)
{
    using namespace Qt3DRender;

    // create a new geometry renderer
    QGeometryRenderer* customMeshRenderer = new QGeometryRenderer;
    QGeometry* customGeometry = new QGeometry;

    // vertices buffer
    QVector<float> points{0.f,  0.f,  0.f,  0.5,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.5,
                          0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.5,  0.f,  0.f,  0.f,  -0.3,
                          0.2,  -0.3, 0.f,  0.f,  0.f,  -0.3, -0.2, -0.3, 0.f,  0.f,  0.f,
                          0.3,  -0.2, -0.3, 0.f,  0.f,  0.f,  0.3,  0.2,  -0.3, -0.3, 0.2,
                          -0.3, -0.3, -0.2, -0.3, -0.3, -0.2, -0.3, 0.3,  -0.2, -0.3, 0.3,
                          -0.2, -0.3, 0.3,  0.2,  -0.3, 0.3,  0.2,  -0.3, -0.3, 0.2,  -0.3};
    QByteArray positionData((const char*)points.data(), points.size() * sizeof(float));
    QBuffer* vertexDataBuffer = new QBuffer(QBuffer::VertexBuffer);
    vertexDataBuffer->setData(positionData);
    QAttribute* positionAttribute = new QAttribute;
    positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexDataBuffer);
    positionAttribute->setDataType(QAttribute::Float);
    positionAttribute->setDataSize(3);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(points.size() / 3);
    positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    customGeometry->addAttribute(positionAttribute);

    // colors buffer
    QVector<float> colors{1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
                          1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
                          1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
                          1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
                          1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
    QByteArray colorData((const char*)colors.data(), colors.size() * sizeof(float));
    QBuffer* colorDataBuffer = new QBuffer(QBuffer::VertexBuffer);
    colorDataBuffer->setData(colorData);
    QAttribute* colorAttribute = new QAttribute;
    colorAttribute->setAttributeType(QAttribute::VertexAttribute);
    colorAttribute->setBuffer(colorDataBuffer);
    colorAttribute->setDataType(QAttribute::Float);
    colorAttribute->setDataSize(3);
    colorAttribute->setByteOffset(0);
    colorAttribute->setByteStride(3 * sizeof(float));
    colorAttribute->setCount(colors.size() / 3);
    colorAttribute->setName(QAttribute::defaultColorAttributeName());
    customGeometry->addAttribute(colorAttribute);

    // geometry renderer settings
    customMeshRenderer->setInstanceCount(1);
    customMeshRenderer->setFirstVertex(0);
    customMeshRenderer->setFirstInstance(0);
    customMeshRenderer->setPrimitiveType(QGeometryRenderer::Lines);
    customMeshRenderer->setGeometry(customGeometry);
    customMeshRenderer->setVertexCount(points.size() / 3);

    /*
    QCamera* camera = new QCamera(this);
    camera->setProjectionType(QCameraLens::PerspectiveProjection);
    camera->setAspectRatio(4.0f/3.0f);
    camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    camera->setViewCenter(QVector3D(0.0f, 3.5f, 0.0f));
    camera->setPosition(QVector3D(0.0f, 3.5f, 25.0f));
    */

    // object picker
    auto picker = new QObjectPicker;

    // add components
    addComponent(customMeshRenderer);
    addComponent(picker);
}

} // namespace
