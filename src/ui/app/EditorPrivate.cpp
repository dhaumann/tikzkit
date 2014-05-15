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
#include "EditorPrivate.h"

#include <tikz.h>

#include <QApplication>

tikz::ui::EditorPrivate::EditorPrivate(QPointer<tikz::ui::EditorPrivate> &staticInstance)
    : tikz::ui::Editor (this)
    , m_application(nullptr)
{
    // remember this
    staticInstance = this;

    //
    // register some datatypes
    //
    qRegisterMetaType<tikz::Anchor>("Anchor");
    qRegisterMetaType<tikz::Arrow>("Arrow");
    qRegisterMetaType<tikz::LineCap>("LineCap");
    qRegisterMetaType<tikz::LineJoin>("LineJoin");
    qRegisterMetaType<tikz::PenStyle>("PenStyle");
    qRegisterMetaType<tikz::Shape>("Shape");
    qRegisterMetaType<tikz::TextAlignment>("TextAlignment");
    qRegisterMetaType<tikz::Pos>("Pos");
    qRegisterMetaType<tikz::Value>("Value");
}

tikz::ui::EditorPrivate::~EditorPrivate()
{
}

tikz::ui::Document *tikz::ui::EditorPrivate::createDocument(QObject *parent)
{
    auto doc = new tikz::ui::TikzDocument(this);

    emit documentCreated(doc);

    return doc;
}

int tikz::ui::EditorPrivate::configPages() const
{
    return 0;
}

tikz::ui::ConfigPage *tikz::ui::EditorPrivate::configPage(int number, QWidget *parent)
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
    delete tikz::ui::EditorPrivate::self();
}

tikz::ui::EditorPrivate *tikz::ui::EditorPrivate::self()
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

void tikz::ui::EditorPrivate::registerDocument(tikz::ui::DocumentPrivate *doc)
{
    Q_ASSERT (!m_documents.contains(doc));
    m_documents.insert(doc, doc);
}

void tikz::ui::EditorPrivate::deregisterDocument(tikz::ui::DocumentPrivate *doc)
{
    Q_ASSERT (m_documents.contains(doc));
    m_documents.remove(doc);
}

void tikz::ui::EditorPrivate::registerView(tikz::ui::ViewPrivate *view)
{
    Q_ASSERT (!m_views.contains(view));
    m_views.insert(view);
}

void tikz::ui::EditorPrivate::deregisterView(tikz::ui::ViewPrivate *view)
{
    Q_ASSERT (m_views.contains(view));
    m_views.remove(view);
}

// kate: indent-width 4; replace-tabs on;
