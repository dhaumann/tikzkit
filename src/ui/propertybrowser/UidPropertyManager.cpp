/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2018 Dominik Haumann <dhaumann@kde.org>
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

#include "UidPropertyManager.h"

namespace tikz {
namespace ui {

class UidPropertyManagerPrivate
{
public:
    struct Data
    {
        tikz::core::Uid val;
        bool readOnly = false;
    };

    typedef QMap<const QtProperty *, Data> PropertyValueMap;
    PropertyValueMap m_values;
};

UidPropertyManager::UidPropertyManager(QObject *parent)
    : QtAbstractPropertyManager(parent)
    , d_ptr(new UidPropertyManagerPrivate())
{
}

UidPropertyManager::~UidPropertyManager()
{
    clear();
    delete d_ptr;
}

tikz::core::Uid UidPropertyManager::value(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return tikz::core::Uid();
    return it.value().val;
}

bool UidPropertyManager::isReadOnly(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return false;
    return it.value().readOnly;
}

QString UidPropertyManager::valueText(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return it.value().val.toString();
}

void UidPropertyManager::setValue(QtProperty *property, const tikz::core::Uid & val)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    auto & data = it.value();

    if (data.val == val)
        return;

    data.val = val;

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void UidPropertyManager::setReadOnly(QtProperty *property, bool readOnly)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    UidPropertyManagerPrivate::Data data = it.value();

    if (data.readOnly == readOnly)
        return;

    data.readOnly = readOnly;
    it.value() = data;

    emit propertyChanged(property);
    emit readOnlyChanged(property, data.readOnly);
}

void UidPropertyManager::initializeProperty(QtProperty *property)
{
    d_ptr->m_values[property] = UidPropertyManagerPrivate::Data();
}


void UidPropertyManager::uninitializeProperty(QtProperty *property)
{
    d_ptr->m_values.remove(property);
}

}
}

// kate: indent-width 4; replace-tabs on;
