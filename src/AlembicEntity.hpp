#pragma once

#include <QEntity>
#include <QUrl>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QMaterial>
#include <QQmlListProperty>

namespace abcentity
{
class CameraLocatorEntity;
class PointCloudEntity;

class AlembicEntity : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(
        float particleSize READ particleSize WRITE setParticleSize NOTIFY particleSizeChanged)
    Q_PROPERTY(
        float locatorScale READ locatorScale WRITE setLocatorScale NOTIFY locatorScaleChanged)
    Q_PROPERTY(QQmlListProperty<abcentity::CameraLocatorEntity> cameras READ cameras NOTIFY camerasChanged)
    Q_PROPERTY(QQmlListProperty<abcentity::PointCloudEntity> pointClouds READ pointClouds NOTIFY pointCloudsChanged)

public:
    AlembicEntity(Qt3DCore::QNode* = nullptr);
    ~AlembicEntity() = default;

public:
    Q_SLOT const QUrl& source() const { return _source; }
    Q_SLOT const float& particleSize() const { return _particleSize; }
    Q_SLOT const float& locatorScale() const { return _locatorScale; }
    Q_SLOT void setSource(const QUrl&);
    Q_SLOT void setParticleSize(const float&);
    Q_SLOT void setLocatorScale(const float&);

private:
    /// Delete all child entities/components
    void clear();
    void createMaterials();
    void loadAbcArchive();
    void visitAbcObject(const Alembic::Abc::IObject&, QEntity* parent);

    QQmlListProperty<CameraLocatorEntity> cameras() {
        return QQmlListProperty<CameraLocatorEntity>(this, _cameras);
    }

    QQmlListProperty<PointCloudEntity> pointClouds() {
        return QQmlListProperty<PointCloudEntity>(this, _pointClouds);
    }

public:
    Q_SIGNAL void sourceChanged();
    Q_SIGNAL void camerasChanged();
    Q_SIGNAL void particleSizeChanged();
    Q_SIGNAL void pointCloudsChanged();
    Q_SIGNAL void locatorScaleChanged();
    Q_SIGNAL void objectPicked(Qt3DCore::QTransform* transform);

protected:
    /// Scale child locators
    void scaleLocators() const;

private:
    QUrl _source;
    float _particleSize = 0.5;
    float _locatorScale = 1.0;
    Qt3DRender::QParameter* _particleSizeParameter;
    Qt3DRender::QMaterial* _cloudMaterial;
    Qt3DRender::QMaterial* _cameraMaterial;
    QList<CameraLocatorEntity*> _cameras;
    QList<PointCloudEntity*> _pointClouds;
};

} // namespace
