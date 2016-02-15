/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2016 Dominik Haumann <dhaumann@kde.org>
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
#ifndef TIKZ_CORE_BOOL_PROPERTY_H
#define TIKZ_CORE_BOOL_PROPERTY_H

#include "Property.h"

namespace tikz {
namespace core {

/**
 * BoolProperty.
 */
class TIKZCORE_EXPORT BoolProperty : public Property
{
public:
    /**
     * Constructor that creates a property @p propertyName as part of @p entity.
     */
    explicit BoolProperty(const QString & propertyName, PropertyInterface * interface);

    /**
     * Virtual destructor.
     */
    virtual ~BoolProperty();

public:
    /**
     * Set the bool to @p enable.
     */
    void setEnabled(bool enable);

    /**
     * Returns the bool property.
     */
    bool enabled() const;

public: // Property overrides
    /**
     * Returns the type of the property.
     * The type is used to cast the property to the respective derived type.
     */
    const char * propertyType() const override;

    /**
     * Reset the property to its default value.
     * @see isSet()
     */
    void unset() override;

protected:
    /**
     * Load the payload of the Property state from the @p json object.
     */
    void loadData(const QJsonObject & json) override;

    /**
     * Save the payload of the Property state to the @p json object.
     */
    void saveData(QJsonObject & json) override;

private:
    bool m_enabled = false;
};

} //namespace core
} //namespace tikz

#endif // TIKZ_CORE_BOOL_PROPERTY_H

// kate: indent-width 4; replace-tabs on;
