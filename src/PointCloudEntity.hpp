#pragma once

#include "BaseAlembicObject.hpp"


namespace abcentity
{

class PointCloudEntity : public BaseAlembicObject
{
    Q_OBJECT

public:
    PointCloudEntity(Qt3DCore::QNode* = nullptr);
    ~PointCloudEntity() = default;

public:
    void setData(const Alembic::Abc::IObject&);

};

} // namespace
