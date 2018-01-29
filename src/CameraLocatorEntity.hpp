#pragma once

#include "BaseAlembicObject.hpp"

#include <QEntity>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>

namespace abcentity
{

class CameraLocatorEntity : public BaseAlembicObject
{
    Q_OBJECT

public:
    CameraLocatorEntity(Qt3DCore::QNode* = nullptr);
    ~CameraLocatorEntity() = default;

};

} // namespace
