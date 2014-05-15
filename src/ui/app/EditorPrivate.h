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

#ifndef TIKZUI_EDITOR_PRIVATE_H
#define TIKZUI_EDITOR_PRIVATE_H

#include <ktexteditor_export.h>
#include "katescript.h"

#include <ktexteditor/editor.h>
#include "ktexteditor/view.h"

#include <KService>
#include <KAboutData>
#include <KSharedConfig>

#include <ktexteditor/application.h>
#include <tikz::ui/Command>
#include <draft/templateinterface2.h>

#include <QList>
#include <QPointer>
#include <QMap>

namespace tikz {
namespace ui {

/**
 * tikz::ui::EditorPrivate
 * One instance of this class is hold alive during
 * a kate part session, as long as any factory, document
 * or view stay around, here is the place to put things
 * which are needed and shared by all this objects ;)
 */
class KTEXTEDITOR_EXPORT EditorPrivate : public tikz::ui::Editor, public tikz::ui::TemplateScriptRegistrar
{
    Q_OBJECT
    Q_INTERFACES(tikz::ui::TemplateScriptRegistrar)

    friend class tikz::ui::Editor;

    // unit testing
public:
    /**
     * Calling this function internally sets a flag such that unitTestMode()
     * returns \p true.
     */
    static void enableUnitTestMode();
    /**
     * Returns \p true, if the unit test mode was enabled through a call of
     * enableUnitTestMode(), otherwise \p false.
     */
    static bool unitTestMode();

    // for setDefaultEncoding
    friend class KateDocumentConfig;

private:
    /**
     * Default constructor, private, as singleton
     * @param staticInstance pointer to fill with content of this
     */
    EditorPrivate(QPointer<tikz::ui::EditorPrivate> &staticInstance);

public:
    /**
     * Destructor
     */
    ~EditorPrivate();

    /**
     * Create a new document object
     * @param parent parent object
     * @return created tikz::ui::Document
     */
    tikz::ui::Document *createDocument(QObject *parent);

    /**
     * Returns a list of all documents of this editor.
     * @return list of all existing documents
     */
    QList<tikz::ui::Document *> documents()
    {
        return m_documents.keys();
    }

    /**
     * Set the global application object.
     * This will allow the editor component to access
     * the hosting application.
     * @param application application object
     */
    void setApplication(tikz::ui::Application *application)
    {
        m_application = application;
    }

    /**
     * Current hosting application, if any set.
     * @return current application object or nullptr
     */
    tikz::ui::Application *application() const
    {
        return m_application;
    }

    /**
     * General Information about this editor
     */
public:
    /**
     * return the about data
     * @return about data of this editor part
     */
    const KAboutData &aboutData() const
    {
        return m_aboutData;
    }

    /**
     * Configuration management
     */
public:
    /**
     * Number of available config pages
     * If the editor returns a number < 1, it doesn't support this
     * and the embedding app should use the configDialog () instead
     * @return number of config pages
     */
    int configPages() const override;

    /**
     * Returns config page with the given number,
     * config pages from 0 to configPages()-1 are available
     * if configPages() > 0
     */
    tikz::ui::ConfigPage *configPage(int number, QWidget *parent) override;

    /**
     * Kate Part Internal stuff ;)
     */
public:
    /**
     * singleton accessor
     * @return instance of the factory
     */
    static tikz::ui::EditorPrivate *self();

    /**
     * register document at the factory
     * this allows us to loop over all docs for example on config changes
     * @param doc document to register
     */
    void registerDocument(tikz::ui::DocumentPrivate *doc);

    /**
     * unregister document at the factory
     * @param doc document to register
     */
    void deregisterDocument(tikz::ui::DocumentPrivate *doc);

    /**
     * register view at the factory
     * this allows us to loop over all views for example on config changes
     * @param view view to register
     */
    void registerView(tikz::ui::ViewPrivate *view);

    /**
     * unregister view at the factory
     * @param view view to unregister
     */
    void deregisterView(tikz::ui::ViewPrivate *view);

    /**
     * return a list of all registered views
     * @return all known views
     */
    QList<tikz::ui::ViewPrivate *> views()
    {
        return m_views.toList();
    }

    /**
     * return a list of all registered docs
     * @return all known documents
     */
    QList<tikz::ui::DocumentPrivate *> kateDocuments()
    {
        return m_documents.values();
    }

private:
    /**
     * registered docs, map from general to specialized pointer
     */
    QHash<tikz::ui::Document *, tikz::ui::DocumentPrivate *> m_documents;

    /**
     * registered views
     */
    QSet<tikz::ui::ViewPrivate *> m_views;

    /**
     * access to application
     */
    QPointer<tikz::ui::Application> m_application;
};

}
}

#endif // TIKZUI_EDITOR_PRIVATE_H

// kate: indent-width 4; replace-tabs on;
