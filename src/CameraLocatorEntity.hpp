#pragma once

#include "BaseAlembicObject.hpp"

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
