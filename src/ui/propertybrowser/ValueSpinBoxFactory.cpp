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

#include "ValueSpinBoxFactory.h"
#include "ValueSpinBox.h"

namespace tikz {
namespace ui {


template <class Editor>
class EditorFactoryPrivate
{
public:

    typedef QList<Editor *> EditorList;
    typedef QMap<QtProperty *, EditorList> PropertyToEditorListMap;
    typedef QMap<Editor *, QtProperty *> EditorToPropertyMap;

    Editor *createEditor(QtProperty *property, QWidget *parent);
    void initializeEditor(QtProperty *property, Editor *e);
    void slotEditorDestroyed(QObject *object);

    PropertyToEditorListMap  m_createdEditors;
    EditorToPropertyMap m_editorToProperty;
};

template <class Editor>
Editor *EditorFactoryPrivate<Editor>::createEditor(QtProperty *property, QWidget *parent)
{
    Editor *editor = new Editor(parent);
    initializeEditor(property, editor);
    return editor;
}

template <class Editor>
void EditorFactoryPrivate<Editor>::initializeEditor(QtProperty *property, Editor *editor)
{
    typename PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        it = m_createdEditors.insert(property, EditorList());
    it.value().append(editor);
    m_editorToProperty.insert(editor, property);
}

template <class Editor>
void EditorFactoryPrivate<Editor>::slotEditorDestroyed(QObject *object)
{
    const typename EditorToPropertyMap::iterator ecend = m_editorToProperty.end();
    for (typename EditorToPropertyMap::iterator itEditor = m_editorToProperty.begin(); itEditor !=  ecend; ++itEditor) {
        if (itEditor.key() == object) {
            Editor *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            const typename PropertyToEditorListMap::iterator pit = m_createdEditors.find(property);
            if (pit != m_createdEditors.end()) {
                pit.value().removeAll(editor);
                if (pit.value().empty())
                    m_createdEditors.erase(pit);
            }
            m_editorToProperty.erase(itEditor);
            return;
        }
    }
}

class ValueSpinBoxFactoryPrivate : public EditorFactoryPrivate<ValueSpinBox>
{
    ValueSpinBoxFactory *q_ptr;
    Q_DECLARE_PUBLIC(ValueSpinBoxFactory)
public:

    void slotPropertyChanged(QtProperty *property, const tikz::Value & value);
    void slotRangeChanged(QtProperty *property, const tikz::Value & min, const tikz::Value & max);
    void slotSingleStepChanged(QtProperty *property, double step);
    void slotDecimalsChanged(QtProperty *property, int prec);
    void slotReadOnlyChanged(QtProperty *property, bool readOnly);
    void slotSetValue(const tikz::Value & value);
};

void ValueSpinBoxFactoryPrivate::slotPropertyChanged(QtProperty *property, const tikz::Value & value)
{
    QList<ValueSpinBox *> editors = m_createdEditors[property];
    QListIterator<ValueSpinBox *> itEditor(m_createdEditors[property]);
    while (itEditor.hasNext()) {
        ValueSpinBox *editor = itEditor.next();
        if (editor->valueWithUnit() != value) {
            editor->blockSignals(true);
            editor->setValueWithUnit(value);
            editor->blockSignals(false);
        }
    }
}

void ValueSpinBoxFactoryPrivate::slotRangeChanged(QtProperty *property,
            const tikz::Value & min, const tikz::Value & max)
{
    if (!m_createdEditors.contains(property))
        return;

    ValuePropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QList<ValueSpinBox *> editors = m_createdEditors[property];
    QListIterator<ValueSpinBox *> itEditor(editors);
    while (itEditor.hasNext()) {
        ValueSpinBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setRange(min.toPoint(), max.toPoint());
        editor->setValueWithUnit(manager->value(property));
        editor->blockSignals(false);
    }
}

void ValueSpinBoxFactoryPrivate::slotSingleStepChanged(QtProperty *property, double step)
{
    if (!m_createdEditors.contains(property))
        return;

    ValuePropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QList<ValueSpinBox *> editors = m_createdEditors[property];
    QListIterator<ValueSpinBox *> itEditor(editors);
    while (itEditor.hasNext()) {
        ValueSpinBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setSingleStep(step);
        editor->blockSignals(false);
    }
}

void ValueSpinBoxFactoryPrivate::slotReadOnlyChanged( QtProperty *property, bool readOnly)
{
    if (!m_createdEditors.contains(property))
        return;

    ValuePropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QListIterator<ValueSpinBox *> itEditor(m_createdEditors[property]);
    while (itEditor.hasNext()) {
        ValueSpinBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setReadOnly(readOnly);
        editor->blockSignals(false);
    }
}

void ValueSpinBoxFactoryPrivate::slotDecimalsChanged(QtProperty *property, int prec)
{
    if (!m_createdEditors.contains(property))
        return;

    ValuePropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QList<ValueSpinBox *> editors = m_createdEditors[property];
    QListIterator<ValueSpinBox *> itEditor(editors);
    while (itEditor.hasNext()) {
        ValueSpinBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setDecimals(prec);
        editor->setValueWithUnit(manager->value(property));
        editor->blockSignals(false);
    }
}

void ValueSpinBoxFactoryPrivate::slotSetValue(const tikz::Value & value)
{
    QObject *object = q_ptr->sender();
    const QMap<ValueSpinBox *, QtProperty *>::ConstIterator itcend = m_editorToProperty.constEnd();
    for (QMap<ValueSpinBox *, QtProperty *>::ConstIterator itEditor = m_editorToProperty.constBegin(); itEditor != itcend; ++itEditor) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            ValuePropertyManager *manager = q_ptr->propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
    }
}

/*! \class ValueSpinBoxFactory

    \brief The ValueSpinBoxFactory class provides ValueSpinBox
    widgets for properties created by ValuePropertyManager objects.

    \sa QtAbstractEditorFactory, ValuePropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
ValueSpinBoxFactory::ValueSpinBoxFactory(QObject *parent)
    : QtAbstractEditorFactory<ValuePropertyManager>(parent)
{
    d_ptr = new ValueSpinBoxFactoryPrivate();
    d_ptr->q_ptr = this;

}

/*!
    Destroys this factory, and all the widgets it has created.
*/
ValueSpinBoxFactory::~ValueSpinBoxFactory()
{
    qDeleteAll(d_ptr->m_editorToProperty.keys());
    delete d_ptr;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void ValueSpinBoxFactory::connectPropertyManager(ValuePropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, const tikz::Value &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const tikz::Value &)));
    connect(manager, SIGNAL(rangeChanged(QtProperty *, const tikz::Value &, const tikz::Value &)),
                this, SLOT(slotRangeChanged(QtProperty *, const tikz::Value &, const tikz::Value &)));
    connect(manager, SIGNAL(singleStepChanged(QtProperty *, double)),
                this, SLOT(slotSingleStepChanged(QtProperty *, double)));
    connect(manager, SIGNAL(decimalsChanged(QtProperty *, int)),
                this, SLOT(slotDecimalsChanged(QtProperty *, int)));
    connect(manager, SIGNAL(readOnlyChanged(QtProperty *, bool)),
                this, SLOT(slotReadOnlyChanged(QtProperty *, bool)));
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget *ValueSpinBoxFactory::createEditor(ValuePropertyManager *manager,
        QtProperty *property, QWidget *parent)
{
    ValueSpinBox *editor = d_ptr->createEditor(property, parent);
    editor->setSingleStep(manager->singleStep(property));
    editor->setDecimals(manager->decimals(property));
    editor->setRange(manager->minimum(property).toPoint(), manager->maximum(property).toPoint());
    editor->setValueWithUnit(manager->value(property));
    editor->setKeyboardTracking(false);
    editor->setReadOnly(manager->isReadOnly(property));

    connect(editor, SIGNAL(valueChanged(const tikz::Value &)), this, SLOT(slotSetValue(const tikz::Value &)));
    connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void ValueSpinBoxFactory::disconnectPropertyManager(ValuePropertyManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, const tikz::Value &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const tikz::Value &)));
    disconnect(manager, SIGNAL(rangeChanged(QtProperty *, const tikz::Value &, const tikz::Value &)),
                this, SLOT(slotRangeChanged(QtProperty *, const tikz::Value &, const tikz::Value &)));
    disconnect(manager, SIGNAL(singleStepChanged(QtProperty *, double)),
                this, SLOT(slotSingleStepChanged(QtProperty *, double)));
    disconnect(manager, SIGNAL(decimalsChanged(QtProperty *, int)),
                this, SLOT(slotDecimalsChanged(QtProperty *, int)));
    disconnect(manager, SIGNAL(readOnlyChanged(QtProperty *, bool)),
                this, SLOT(slotReadOnlyChanged(QtProperty *, bool)));
}

}
}

#include "moc_ValueSpinBoxFactory.cpp"

// kate: indent-width 4; replace-tabs on;
