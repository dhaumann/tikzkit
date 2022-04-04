/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ValuePropertyManager.h"

namespace tikz {
namespace ui {

class ValuePropertyManagerPrivate
{
public:
    struct Data
    {
        tikz::Value val = 0.0_mm;
        tikz::Value minVal = 0.0_mm;
        tikz::Value maxVal = tikz::Value(INT_MAX, Unit::Millimeter);
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
        return 0.0_pt;
    return it.value().val;
}

tikz::Value ValuePropertyManager::minimum(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return 0.0_pt;
    return it.value().minVal;
}

tikz::Value ValuePropertyManager::maximum(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return 0.0_pt;
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

    if (data.val == val && data.val.unit() == val.unit())
        return;

    const tikz::Value oldVal = data.val;
    data.val = val < data.minVal ? data.minVal
        : val > data.maxVal ? data.maxVal
        : val;

    if (data.val == oldVal && data.val.unit() == oldVal.unit())
        return;

    Q_EMIT propertyChanged(property);
    Q_EMIT valueChanged(property, data.val);
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

    Q_EMIT singleStepChanged(property, data.singleStep);
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

    Q_EMIT propertyChanged(property);
    Q_EMIT readOnlyChanged(property, data.readOnly);
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

    Q_EMIT decimalsChanged(property, data.decimals);
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

        Q_EMIT propertyChanged(property);
        Q_EMIT valueChanged(property, data.val);
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

        Q_EMIT propertyChanged(property);
        Q_EMIT valueChanged(property, data.val);
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
        Q_EMIT propertyChanged(property);
        Q_EMIT valueChanged(property, data.val);
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
