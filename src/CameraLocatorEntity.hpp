#pragma once

#include "BaseAlembicObject.hpp"

namespace abcentity
{

class CameraLocatorEntity : public BaseAlembicObject
{
    Q_OBJECT

public:
    ~CameraLocatorEntity() = default;
    explicit CameraLocatorEntity(Qt3DCore::QNode* = nullptr);

};

} // namespace
