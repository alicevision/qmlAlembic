#pragma once

#include "BaseAlembicObject.hpp"

namespace abcentity
{

class CameraLocatorEntity : public BaseAlembicObject
{
    Q_OBJECT

public:
    explicit CameraLocatorEntity(Qt3DCore::QNode* = nullptr, bool oldVersion = false);
    ~CameraLocatorEntity() override = default;

};

} // namespace
