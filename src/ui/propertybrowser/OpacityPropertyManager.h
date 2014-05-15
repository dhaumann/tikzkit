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

#ifndef TIKZ_UI_OPACITY_PROPERTY_MANAGER_H
#define TIKZ_UI_OPACITY_PROPERTY_MANAGER_H

#include <QtAbstractPropertyManager>

namespace tikz {
namespace ui {

class OpacityPropertyManagerPrivate;

class OpacityPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    OpacityPropertyManager(QObject *parent = 0);
    ~OpacityPropertyManager();

    qreal value(const QtProperty *property) const;
    bool isReadOnly(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, qreal val);
    void setReadOnly(QtProperty *property, bool readOnly);
Q_SIGNALS:
    void valueChanged(QtProperty *property, qreal val);
    void readOnlyChanged(QtProperty *property, bool readOnly);
protected:
    QString valueText(const QtProperty *property) const;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    OpacityPropertyManagerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(OpacityPropertyManager)
    Q_DISABLE_COPY(OpacityPropertyManager)
};

}
}

#endif // TIKZ_UI_OPACITY_PROPERTY_MANAGER_H

// kate: indent-width 4; replace-tabs on;
