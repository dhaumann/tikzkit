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

#ifndef TIKZ_UI_OPACITY_EDITOR_FACTORY_H
#define TIKZ_UI_OPACITY_EDITOR_FACTORY_H

#include "OpacityPropertyManager.h"

#include <qteditorfactory.h>
#include <tikz/core/Value.h>

namespace tikz {
namespace ui {

class OpacityEditorFactoryPrivate;

class OpacityEditorFactory : public QtAbstractEditorFactory<OpacityPropertyManager>
{
    Q_OBJECT
public:
    OpacityEditorFactory(QObject *parent = 0);
    ~OpacityEditorFactory();
protected:
    void connectPropertyManager(OpacityPropertyManager *manager) override;
    QWidget *createEditor(OpacityPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(OpacityPropertyManager *manager) override;
private:
    OpacityEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(OpacityEditorFactory)
    Q_DISABLE_COPY(OpacityEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, qreal))
    Q_PRIVATE_SLOT(d_func(), void slotReadOnlyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(qreal))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

}
}

#endif // TIKZ_UI_OPACITY_EDITOR_FACTORY_H

// kate: indent-width 4; replace-tabs on;
