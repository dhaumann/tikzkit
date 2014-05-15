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

#include "OpacityPropertyManager.h"

#include <QDebug>

namespace tikz {
namespace ui {

class OpacityPropertyManagerPrivate
{
public:
    struct Data
    {
        qreal val = 1.0;
        bool readOnly = false;
    };

    typedef QMap<const QtProperty *, Data> PropertyValueMap;
    PropertyValueMap m_values;
};

OpacityPropertyManager::OpacityPropertyManager(QObject *parent)
    : QtAbstractPropertyManager(parent)
    , d_ptr(new OpacityPropertyManagerPrivate())
{
}

OpacityPropertyManager::~OpacityPropertyManager()
{
    clear();
    delete d_ptr;
}

qreal OpacityPropertyManager::value(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return qreal(0);
    return it.value().val;
}

bool OpacityPropertyManager::isReadOnly(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return false;
    return it.value().readOnly;
}

QString OpacityPropertyManager::valueText(const QtProperty *property) const
{
    const auto it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return QString::number(qRound(it.value().val * 100.0)) + QLatin1Char('%');
}

void OpacityPropertyManager::setValue(QtProperty *property, qreal val)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    auto & data = it.value();

    if (data.val == val)
        return;

    const qreal oldVal = data.val;
    data.val = val < 0.0 ? 0.0
        : val > 1.0 ? 1.0
        : val;

    if (data.val == oldVal)
        return;

    qDebug() << "emiting new value:" << data.val;
    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void OpacityPropertyManager::setReadOnly(QtProperty *property, bool readOnly)
{
    const auto it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    OpacityPropertyManagerPrivate::Data data = it.value();

    if (data.readOnly == readOnly)
        return;

    data.readOnly = readOnly;
    it.value() = data;

    emit propertyChanged(property);
    emit readOnlyChanged(property, data.readOnly);
}

void OpacityPropertyManager::initializeProperty(QtProperty *property)
{
    d_ptr->m_values[property] = OpacityPropertyManagerPrivate::Data();
}


void OpacityPropertyManager::uninitializeProperty(QtProperty *property)
{
    d_ptr->m_values.remove(property);
}

}
}

// kate: indent-width 4; replace-tabs on;
