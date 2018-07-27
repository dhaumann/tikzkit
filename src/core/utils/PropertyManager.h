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
#ifndef TIKZ_CORE_PROPERTY_MANAGER_H
#define TIKZ_CORE_PROPERTY_MANAGER_H

#include "tikz.h"
#include "tikz_export.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

namespace tikz {
namespace core {

class PropertyInfo
{
public:
    /**
     * Constructor for property @p name based on the data @p json.
     */
    PropertyInfo(const QString & name, const QJsonObject & json);

    //! Check whether the property is valid.
    bool isValid() const;

    //! Returns the property type.
    QString type() const;

    //! Returns the property name.
    QString name() const;

    //! Returns a description / title of the property.
    QString title() const;

    //! Returns the minimum value. Not supported by all properties.
    QVariant minimum() const;

    //! Returns the maximum value. Not supported by all properties.
    QVariant maximum() const;

    //! Returns the single step used for e.g. spin boxes.
    //! Not supported by all properties.
    double singleStep() const;

    //! Returns a function to be called via QObject::invokeMethod() to check
    //! whether this property was set. Not supported by all properties.
    QString modifiedFunction() const;

    //! Returns true, if this property can be unset.
    bool isResettable() const;

    //! Returns true, if this property can be unset.
    QString resetFunction() const;

private:
    QString m_name;
    QJsonObject m_json;
};

/**
 * The PropertyManager class provides meta information about properties.
 * The meta information is loaded from a json file.
 *
 * @see Entity
 */
class TIKZCORE_EXPORT PropertyManager
{
public:
    /**
     * Default constructor. Creates an invalid PropertyManager.
     */
    PropertyManager();

    /**
     * Returns the meta information for property @p name.
     */
    PropertyInfo info(const QString & name) const;

private:
    /**
     * Internal data structure for querying meta data about properties.
     */
    QJsonDocument m_json;
};

/**
 * Getter that returns the global instance.
 */
TIKZCORE_EXPORT PropertyManager & propertyManager();

}
}

#endif // TIKZ_CORE_PROPERTY_MANAGER_H

// kate: indent-width 4; replace-tabs on;
