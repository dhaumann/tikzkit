#include "ValuePropertyManager.h"

namespace tikz {
namespace ui {

class ValuePropertyManagerPrivate
{
public:
    struct Data
    {
        tikz::Value val = tikz::Value(0, Millimeter);
        tikz::Value minVal = tikz::Value(0, Millimeter);
        tikz::Value maxVal = tikz::Value(INT_MAX, Millimeter);
        double singleStep = 0.1;
        int decimals = 2;
        bool readOnly = false;
    };

    typedef QMap<const QtProperty *, Data> PropertyValueMap;
    PropertyValueMap m_values;
};

ValuePropertyManager::ValuePropertyManager(QObject *parent)
    : QtAbstractPropertyManager(parent)
    , d_ptr(new ValuePropertyManagerPrivate())
{
}

ValuePropertyManager::~ValuePropertyManager()
{
    clear();
    delete d_ptr;
}

tikz::Value ValuePropertyManager::value(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return tikz::Value(0);
    return it.value().val;
}

tikz::Value ValuePropertyManager::minimum(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return tikz::Value(0);
    return it.value().minVal;
}

tikz::Value ValuePropertyManager::maximum(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return tikz::Value(0);
    return it.value().maxVal;
}

double ValuePropertyManager::singleStep(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return 0;
    return it.value().singleStep;
}

int ValuePropertyManager::decimals(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return 2;
    return it.value().decimals;
}

bool ValuePropertyManager::isReadOnly(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return false;
    return it.value().readOnly;
}

QString ValuePropertyManager::valueText(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return it.value().val.toString();
}

void ValuePropertyManager::setValue(QtProperty *property, const tikz::Value & val)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    auto & data = it.value();

    if (data.val == val)
        return;

    const tikz::Value oldVal = data.val;
    data.val = val < data.minVal ? data.minVal
        : val > data.maxVal ? data.maxVal
        : val;

    if (data.val == oldVal)
        return;

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void ValuePropertyManager::setSingleStep(QtProperty *property, double step)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    ValuePropertyManagerPrivate::Data data = it.value();

    if (step < 0)
        step = 0;

    if (data.singleStep == step)
        return;

    data.singleStep = step;

    it.value() = data;

    emit singleStepChanged(property, data.singleStep);
}

void ValuePropertyManager::setReadOnly(QtProperty *property, bool readOnly)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    ValuePropertyManagerPrivate::Data data = it.value();

    if (data.readOnly == readOnly)
        return;

    data.readOnly = readOnly;
    it.value() = data;

    emit propertyChanged(property);
    emit readOnlyChanged(property, data.readOnly);
}

void ValuePropertyManager::setDecimals(QtProperty *property, int prec)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    ValuePropertyManagerPrivate::Data data = it.value();

    if (prec > 13)
        prec = 13;
    else if (prec < 0)
        prec = 0;

    if (data.decimals == prec)
        return;

    data.decimals = prec;

    it.value() = data;

    emit decimalsChanged(property, data.decimals);
}

void ValuePropertyManager::setMinimum(QtProperty *property, const tikz::Value & minVal)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    auto & data = it.value();

    data.minVal = minVal;

    if (data.val < data.minVal) {
        data.val = data.minVal;

        emit propertyChanged(property);
        emit valueChanged(property, data.val);
    }
}

void ValuePropertyManager::setMaximum(QtProperty *property, const tikz::Value & maxVal)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    auto & data = it.value();

    data.maxVal = maxVal;

    if (data.val > data.maxVal) {
        data.val = data.maxVal;

        emit propertyChanged(property);
        emit valueChanged(property, data.val);
    }
}

void ValuePropertyManager::setRange(QtProperty *property, const tikz::Value & minVal, const tikz::Value & maxVal)
{
    Q_ASSERT(minVal <= maxVal);

    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    auto & data = it.value();

    data.minVal = minVal;
    data.maxVal = maxVal;

    bool changed = false;

    if (data.val < data.minVal) {
        data.val = data.minVal;
        changed = true;
    }

    if (data.val > data.maxVal) {
        data.val = data.maxVal;
        changed = true;
    }

    if (changed) {
        emit propertyChanged(property);
        emit valueChanged(property, data.val);
    }
}


void ValuePropertyManager::initializeProperty(QtProperty *property)
{
    d_ptr->m_values[property] = ValuePropertyManagerPrivate::Data();
}


void ValuePropertyManager::uninitializeProperty(QtProperty *property)
{
    d_ptr->m_values.remove(property);
}

}
}

// kate: indent-width 4; replace-tabs on;
