/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015 Dominik Haumann <dhaumann@kde.org>
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

#include "PropertyManager.h"
#include "Document.h"

#include <QFile>

namespace tikz {
namespace core {

PropertyInfo::PropertyInfo(const QString & name, const QJsonObject & json)
    : m_name(name)
    , m_json(json)
{
}

bool PropertyInfo::isValid() const
{
    return !m_json.isEmpty();
}

QString PropertyInfo::type() const
{
    return m_json["type"].toString();
}

QString PropertyInfo::name() const
{
    return m_name;
}

QString PropertyInfo::title() const
{
    return m_json["title"].toString();
}

QVariant PropertyInfo::minimum() const
{
    return m_json["minimum"].toVariant();
}

QVariant PropertyInfo::maximum() const
{
    return m_json["maximum"].toVariant();
}

double PropertyInfo::singleStep() const
{
    return m_json["singleStep"].toDouble(1.0);
}

QString PropertyInfo::modifiedFunction() const
{
    return m_json["modified"].toString();
}

bool PropertyInfo::isResettable() const
{
    return !m_json["reset"].toString().isEmpty();
}

QString PropertyInfo::resetFunction() const
{
    return m_json["reset"].toString();
}

PropertyManager::PropertyManager()
{
    QFile file("../data/properties.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not read file properties.json";
        return;
    }
    m_json = QJsonDocument::fromJson(file.readAll());
}

PropertyInfo PropertyManager::info(const QString & name) const
{
    return PropertyInfo(name, m_json["properties"][name].toObject());
}


PropertyManager & propertyManager()
{
    static PropertyManager manager;
    return manager;
}

}
}

// kate: indent-width 4; replace-tabs on;
