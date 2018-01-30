#pragma once

#include <QEntity>
#include <Alembic/AbcGeom/All.h>

namespace abcentity
{

/**
 * @brief BaseAlembicObject is the base class for QEntities instantiated by AlembicEntity
 */
class BaseAlembicObject : public Qt3DCore::QEntity
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap arbProperties READ arbProperties CONSTANT)
    Q_PROPERTY(QVariantMap userProperties READ userProperties CONSTANT)

public:

    BaseAlembicObject(Qt3DCore::QNode* = nullptr);
    ~BaseAlembicObject() = default;

    void setTransform(const Alembic::Abc::M44d&);

    const QVariantMap& arbProperties() const { return _arbProperties; }
    const QVariantMap& userProperties() const { return _userProperties; }

    void fillArbProperties(const Alembic::Abc::ICompoundProperty& iParent);
    void fillUserProperties(const Alembic::Abc::ICompoundProperty& iParent);

protected:
    /// report alembic properties to the given variantMap
    void fillPropertyMap(const Alembic::Abc::ICompoundProperty& iParent, QVariantMap& variantMap);

    template<typename PODTYPE>
    void addScalarProperty(QVariantMap& data, const Alembic::Abc::IScalarProperty& prop);

    template<typename PODTYPE>
    void addArrayProperty(QVariantMap &data, const Alembic::Abc::IArrayProperty &prop);

    template<typename PODTYPE>
    void addProperty(QVariantMap& data, const Alembic::Abc::ICompoundProperty iParent, const Alembic::Abc::PropertyHeader& propHeader);

protected:
    QVariantMap _arbProperties;
    QVariantMap _userProperties;
};

}
