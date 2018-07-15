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

#include "OpacityEditorFactory.h"
#include "OpacityEditor.h"

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

class OpacityEditorFactoryPrivate : public EditorFactoryPrivate<OpacityEditor>
{
    OpacityEditorFactory *q_ptr;
    Q_DECLARE_PUBLIC(OpacityEditorFactory)
public:

    void slotPropertyChanged(QtProperty *property, qreal value);
    void slotReadOnlyChanged(QtProperty *property, bool readOnly);
    void slotSetValue(qreal value);
};

void OpacityEditorFactoryPrivate::slotPropertyChanged(QtProperty *property, qreal value)
{
    QList<OpacityEditor *> editors = m_createdEditors[property];
    QListIterator<OpacityEditor *> itEditor(m_createdEditors[property]);
    while (itEditor.hasNext()) {
        OpacityEditor *editor = itEditor.next();
        if (editor->value() != value) {
            editor->blockSignals(true);
            editor->setValue(value);
            editor->blockSignals(false);
        }
    }
}

void OpacityEditorFactoryPrivate::slotReadOnlyChanged(QtProperty *property, bool readOnly)
{
    if (!m_createdEditors.contains(property))
        return;

    OpacityPropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QListIterator<OpacityEditor *> itEditor(m_createdEditors[property]);
    while (itEditor.hasNext()) {
        OpacityEditor *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setReadOnly(readOnly);
        editor->blockSignals(false);
    }
}

void OpacityEditorFactoryPrivate::slotSetValue(qreal value)
{
    QObject *object = q_ptr->sender();
    const QMap<OpacityEditor *, QtProperty *>::ConstIterator itcend = m_editorToProperty.constEnd();
    for (QMap<OpacityEditor *, QtProperty *>::ConstIterator itEditor = m_editorToProperty.constBegin(); itEditor != itcend; ++itEditor) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            OpacityPropertyManager *manager = q_ptr->propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
    }
}

OpacityEditorFactory::OpacityEditorFactory(QObject *parent)
    : QtAbstractEditorFactory<OpacityPropertyManager>(parent)
{
    d_ptr = new OpacityEditorFactoryPrivate();
    d_ptr->q_ptr = this;

}

OpacityEditorFactory::~OpacityEditorFactory()
{
    qDeleteAll(d_ptr->m_editorToProperty.keys());
    delete d_ptr;
}

void OpacityEditorFactory::connectPropertyManager(OpacityPropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, qreal)),
                this, SLOT(slotPropertyChanged(QtProperty *, qreal)));
    connect(manager, SIGNAL(readOnlyChanged(QtProperty *, bool)),
                this, SLOT(slotReadOnlyChanged(QtProperty *, bool)));
}

QWidget *OpacityEditorFactory::createEditor(OpacityPropertyManager *manager,
        QtProperty *property, QWidget *parent)
{
    OpacityEditor *editor = d_ptr->createEditor(property, parent);
    editor->setValue(manager->value(property));
    editor->setReadOnly(manager->isReadOnly(property));

    connect(editor, SIGNAL(valueChanged(qreal)), this, SLOT(slotSetValue(qreal)));
    connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

void OpacityEditorFactory::disconnectPropertyManager(OpacityPropertyManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, qreal)),
                this, SLOT(slotPropertyChanged(QtProperty *, qreal)));
    disconnect(manager, SIGNAL(readOnlyChanged(QtProperty *, bool)),
                this, SLOT(slotReadOnlyChanged(QtProperty *, bool)));
}

}
}

#include "moc_OpacityEditorFactory.cpp"

// kate: indent-width 4; replace-tabs on;
