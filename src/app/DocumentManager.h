/* This file is part of the TikZKit project
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
 * Copyright (C) 2001 Christoph Cullmann <cullmann@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TIKZKIT_DOCUMENT_MANAGER_H
#define TIKZKIT_DOCUMENT_MANAGER_H

#include <tikz/ui/Document.h>
#include <tikz/ui/Editor.h>

#include <QList>
#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QMap>

class MainWindow;

class DocumentManager : public QObject
{
    Q_OBJECT

public:
    DocumentManager(QObject *parent);
    virtual ~DocumentManager();

    tikz::ui::Document * createDocument();

    /** Returns the document with @p url or \e nullptr if no such doc is found */
    tikz::ui::Document * findDocument(const QUrl &url) const;

    const QList<tikz::ui::Document *> & documentList() const {
        return m_documents;
    }

    tikz::ui::Document *openUrl(const QUrl & url);

    bool closeDocument(tikz::ui::Document *, bool closeUrl = true);
    bool closeAllDocuments(bool closeUrl = true);

    QList<tikz::ui::Document *> modifiedDocumentList();
    bool queryCloseDocuments(MainWindow * w);

public Q_SLOTS:
    /**
     * saves all documents that has at least one view.
     * documents with no views are ignored :P
     */
    void saveAll();

    /**
     * reloads all documents that has at least one view.
     * documents with no views are ignored :P
     */
    void reloadAll();

Q_SIGNALS:
    /**
     * This signal is emitted when the \p document was created.
     */
    void documentCreated(tikz::ui::Document *document);

    /**
     * This signal is emitted before a \p document which should be closed is deleted
     * The document is still accessible and usable, but it will be deleted
     * after this signal was send.
     *
     * @param document document that will be deleted
     */
    void aboutToDeleteDocument(tikz::ui::Document *document);

    /**
     * This signal is emitted when the \p document has been deleted.
     *
     *  Warning !!! DO NOT ACCESS THE DATA REFERENCED BY THE POINTER, IT IS ALREADY INVALID
     *  Use the pointer only to remove mappings in hash or maps
     */
    void documentDeleted(tikz::ui::Document *document);

private:
    QList<tikz::ui::Document *> m_documents;
};

#endif

// kate: indent-width 4; replace-tabs on;
