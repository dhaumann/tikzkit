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

#ifndef TIKZ_UI_VALUE_PROPERTY_MANAGER_H
#define TIKZ_UI_VALUE_PROPERTY_MANAGER_H

#include <QtAbstractPropertyManager>
#include <tikz/core/Value.h>

namespace tikz {
namespace ui {

class ValuePropertyManagerPrivate;

class ValuePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    ValuePropertyManager(QObject *parent = 0);
    ~ValuePropertyManager();

    tikz::Value value(const QtProperty *property) const;
    tikz::Value minimum(const QtProperty *property) const;
    tikz::Value maximum(const QtProperty *property) const;
    double singleStep(const QtProperty *property) const;
    int decimals(const QtProperty *property) const;
    bool isReadOnly(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const tikz::Value & val);
    void setMinimum(QtProperty *property, const tikz::Value & minVal);
    void setMaximum(QtProperty *property, const tikz::Value & maxVal);
    void setRange(QtProperty *property, const tikz::Value & minVal, const tikz::Value & maxVal);
    void setSingleStep(QtProperty *property, double step);
    void setDecimals(QtProperty *property, int prec);
    void setReadOnly(QtProperty *property, bool readOnly);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const tikz::Value & val);
    void rangeChanged(QtProperty *property, const tikz::Value & minVal, const tikz::Value & maxVal);
    void singleStepChanged(QtProperty *property, double step);
    void decimalsChanged(QtProperty *property, int prec);
    void readOnlyChanged(QtProperty *property, bool readOnly);
protected:
    QString valueText(const QtProperty *property) const;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    ValuePropertyManagerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ValuePropertyManager)
    Q_DISABLE_COPY(ValuePropertyManager)
};

}
}

#endif // TIKZ_UI_VALUE_PROPERTY_MANAGER_H

// kate: indent-width 4; replace-tabs on;
