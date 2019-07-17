#pragma once

#include "BaseAlembicObject.hpp"


namespace abcentity
{

class PointCloudEntity : public BaseAlembicObject
{
    Q_OBJECT

public:
    ~PointCloudEntity() = default;
    explicit PointCloudEntity(Qt3DCore::QNode* = nullptr);

public:
    void setData(const Alembic::Abc::IObject&);

};

} // namespace
