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

#ifndef TIKZ_UI_VALUE_SPINBOX_FACTORY_H
#define TIKZ_UI_VALUE_SPINBOX_FACTORY_H

#include "ValuePropertyManager.h"

#include <qteditorfactory.h>
#include <tikz/core/Value.h>

namespace tikz {
namespace ui {

class ValueSpinBoxFactoryPrivate;

class ValueSpinBoxFactory : public QtAbstractEditorFactory<ValuePropertyManager>
{
    Q_OBJECT
public:
    ValueSpinBoxFactory(QObject *parent = 0);
    ~ValueSpinBoxFactory();
protected:
    void connectPropertyManager(ValuePropertyManager *manager) override;
    QWidget *createEditor(ValuePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(ValuePropertyManager *manager) override;
private:
    ValueSpinBoxFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ValueSpinBoxFactory)
    Q_DISABLE_COPY(ValueSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const tikz::Value &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, const tikz::Value &, const tikz::Value &))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotDecimalsChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotReadOnlyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const tikz::Value &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

}
}

#endif // TIKZ_UI_VALUE_SPINBOX_FACTORY_H

// kate: indent-width 4; replace-tabs on;
