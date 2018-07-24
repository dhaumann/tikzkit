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

#ifndef TIKZ_UI_UID_PROPERTY_MANAGER_H
#define TIKZ_UI_UID_PROPERTY_MANAGER_H

#include <QtAbstractPropertyManager>
#include <tikz/core/Uid.h>

namespace tikz {
namespace ui {

class UidPropertyManagerPrivate;

class UidPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    UidPropertyManager(QObject *parent = nullptr);
    ~UidPropertyManager();

    tikz::core::Uid value(const QtProperty *property) const;
    bool isReadOnly(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const tikz::core::Uid & val);
    void setReadOnly(QtProperty *property, bool readOnly);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const tikz::core::Uid & val);
    void readOnlyChanged(QtProperty *property, bool readOnly);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    UidPropertyManagerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(UidPropertyManager)
    Q_DISABLE_COPY(UidPropertyManager)
};

}
}

#endif // TIKZ_UI_UID_PROPERTY_MANAGER_H

// kate: indent-width 4; replace-tabs on;
