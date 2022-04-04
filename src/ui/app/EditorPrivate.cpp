/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014-2016 Dominik Haumann <dhaumann@kde.org>
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
#include "EditorPrivate.h"
#include "DocumentPrivate.h"
#include "ViewPrivate.h"

#include <tikz/core/tikz.h>
#include <tikz/core/Value.h>
#include <tikz/core/Pos.h>
#include <tikz/core/Style.h>

#include <QApplication>

namespace tikz {
namespace ui {

EditorPrivate::EditorPrivate(QPointer<tikz::ui::EditorPrivate> &staticInstance)
    : tikz::ui::Editor(this)
    , m_dummyApplication(nullptr)
    , m_dummyMainWindow(nullptr)
    , m_application(&m_dummyApplication)
{
    // remember this
    staticInstance = this;

    //
    // register some datatypes
    //
    qRegisterMetaType<tikz::Arrow>("Arrow");
    qRegisterMetaType<tikz::LineCap>("LineCap");
    qRegisterMetaType<tikz::LineJoin>("LineJoin");
    qRegisterMetaType<tikz::PenStyle>("PenStyle");
    qRegisterMetaType<tikz::Shape>("Shape");
    qRegisterMetaType<tikz::TextAlignment>("TextAlignment");
    qRegisterMetaType<tikz::Unit>("Unit");
    qRegisterMetaType<tikz::Pos>("Pos");
    qRegisterMetaType<tikz::Value>("Value");
    qRegisterMetaType<tikz::core::Style*>("Style*");
    qRegisterMetaType<tikz::core::Uid>("Uid");
}

EditorPrivate::~EditorPrivate()
{
}

tikz::ui::Document * EditorPrivate::createDocument(QObject *parent)
{
    Q_UNUSED(parent) // FIXME: delete parameter? or is 'this' below wrong?
    auto doc = new tikz::ui::DocumentPrivate(this);

    Q_EMIT documentCreated(doc);

    return doc;
}

int EditorPrivate::configPages() const
{
    return 0;
}

tikz::ui::ConfigPage * EditorPrivate::configPage(int number, QWidget *parent)
{
    Q_UNUSED(number)
    Q_UNUSED(parent)

    return nullptr;
}

/**
 * Cleanup the tikz::ui::EditorPrivate during QCoreApplication shutdown
 */
static void cleanupGlobal()
{
    /**
     * delete if there
     */
    delete EditorPrivate::self();
}

EditorPrivate * EditorPrivate::self()
{
    /**
     * remember the static instance in a QPointer
     */
    static bool inited = false;
    static QPointer<tikz::ui::EditorPrivate> staticInstance;

    /**
     * just return it, if already inited
     */
    if (inited) {
        return staticInstance.data();
    }

    /**
     * start init process
     */
    inited = true;

    /**
     * now create the object and store it
     */
    new tikz::ui::EditorPrivate(staticInstance);

    /**
     * register cleanup
     * let use be deleted during QCoreApplication shutdown
     */
    qAddPostRoutine(cleanupGlobal);

    /**
     * return instance
     */
    return staticInstance.data();
}

void EditorPrivate::registerDocument(tikz::ui::DocumentPrivate *doc)
{
    Q_ASSERT (!m_documents.contains(doc));
    m_documents.append(doc);
}

void EditorPrivate::unregisterDocument(tikz::ui::DocumentPrivate *doc)
{
    Q_ASSERT (m_documents.contains(doc));
    m_documents.remove(m_documents.indexOf(doc));
}

QVector<tikz::ui::Document *> EditorPrivate::documents()
{
    QVector<tikz::ui::Document*> docs;
    docs.reserve(m_documents.size());
    for (int i = 0; i < m_documents.size(); ++i) {
        docs.append(m_documents.at(i));
    }
    return docs;
}

QVector<tikz::ui::View *> EditorPrivate::views()
{
    QVector<tikz::ui::View*> list;
    list.reserve(m_views.size());
    for (int i = 0; i < m_views.size(); ++i) {
        list.append(m_views.at(i));
    }
    return list;
}

QVector<tikz::ui::DocumentPrivate *> EditorPrivate::tikzDocuments()
{
    return m_documents;
}

void EditorPrivate::registerView(tikz::ui::ViewPrivate *view)
{
    Q_ASSERT (!m_views.contains(view));
    m_views.append(view);
}

void EditorPrivate::unregisterView(tikz::ui::ViewPrivate *view)
{
    Q_ASSERT (m_views.contains(view));
    m_views.remove(m_views.indexOf(view));
}

QVector<tikz::ui::ViewPrivate *> EditorPrivate::tikzViews()
{
    return m_views;
}

}
}

// kate: indent-width 4; replace-tabs on;
