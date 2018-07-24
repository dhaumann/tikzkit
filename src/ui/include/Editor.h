/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014-2016 Dominik Haumann <dhaumann@kde.org>
 * Copyright (C) 2014 Christoph Cullmann <cullmann@kde.org>
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

#ifndef TIKZUI_EDITOR_H
#define TIKZUI_EDITOR_H

#include "tikzui_export.h"

#include <QObject>
#include <QVector>

namespace tikz {
namespace ui {

class Application;
class Document;
class View;
class EditorPrivate;
class ConfigPage;

class TIKZUI_EXPORT Editor : public QObject
{
    Q_OBJECT

protected:
    /**
     * Constructor.
     *
     * Create the Editor object and pass it the internal
     * implementation to store a d-pointer.
     *
     * @param impl d-pointer to use
     */
    Editor(EditorPrivate *impl);

    /**
     * Virtual destructor.
     */
    virtual ~Editor();

public:
    /**
     * Accessor to get the Editor instance.
     *
     * @note This object will stay alive until QCoreApplication terminates.
     *       You shall not delete it yourself.
     *       There is only ONE Editor instance of this per process.
     *
     * \return Editor controller, after initial construction, will
     *        live until QCoreApplication is terminating.
     */
    static Editor *instance();

public:
    /**
     * Set the global application object.
     * This will allow the editor component to access
     * the hosting application.
     * @param application application object. If the argument is a nullptr,
     *                    this will reset the application back to a dummy interface.
     */
    virtual void setApplication(tikz::ui::Application *application) = 0;

    /**
     * Current hosting application, if any set.
     * @return current application object or a dummy interface that allows
     *         calling the functions without having to check for a nullptr.
     */
    virtual tikz::ui::Application *application() const = 0;

    /*
     * Methods to create and manage the documents.
     */
public:
    /**
     * Create a new document object with \p parent.
     *
     * For each created document, the signal documentCreated() is emitted.
     *
     * \param parent parent object
     * \return new tikz::ui::Document object
     * \see documents(), documentCreated()
     */
    virtual Document *createDocument(QObject *parent) = 0;

    /**
     * Get a list of all documents of this editor.
     * \return list of all existing documents
     * \see createDocument()
     */
    virtual QVector<tikz::ui::Document *> documents() = 0;

    /**
     * Get a list of all Views of this editor.
     * \return list of all existing Views
     */
    virtual QVector<tikz::ui::View *> views() = 0;

Q_SIGNALS:
    /**
     * The \p editor emits this signal whenever a \p document was successfully
     * created.
     * \param document the newly created document instance
     * \see createDocument()
     */
    void documentCreated(tikz::ui::Document * document);

    /*
     * Configuration management.
     */
public:
    /**
     * Get the number of available config pages.
     * If a number < 1 is returned, it does not support config pages.
     * \return number of config pages
     * \see configPage()
     */
    virtual int configPages() const = 0;

    /**
     * Get the config page with the \p number, config pages from 0 to
     * configPages()-1 are available if configPages() > 0.
     * \param number index of config page
     * \param parent parent widget for config page
     * \return created config page or NULL, if the number is out of bounds
     * \see configPages()
     */
    virtual ConfigPage *configPage(int number, QWidget *parent) = 0;

private:
    /**
     * private d-pointer, pointing to the internal implementation
     */
    EditorPrivate *const d;
};

}
}

#endif

// kate: indent-width 4; replace-tabs on;
