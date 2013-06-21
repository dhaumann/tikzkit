/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_DOCUMENT_H
#define TIKZ_DOCUMENT_H

#include "tikz_export.h"
#include "tikz.h"

#include <QObject>
#include <QVector>

class QUndoStack;

namespace tikz
{

class DocumentPrivate;
class Style;
class Node;
class Edge;

class TIKZCORE_EXPORT Document : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Document(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~Document();

    //
    // Undo / redo management
    //
    public:
        /**
         * Get the undo stack of this document.
         */
        QUndoStack * undoManager();

        /**
         * Check whether the document is in a modified state.
         */
        bool isModified() const;

    //
    // Style management
    //
    public:
        /**
         * Get the global document style.
         * This is the global document style. All other styles fall back to
         * this style, if a property is not set.
         */
        Style& style() const;

        /**
         * Get the list of nodes of the tikz document.
         */
        QVector<Node*> nodes() const;

        /**
         * Get the list of edges of the tikz document.
         */
        QVector<Edge*> edges() const;

    //
    // Implementation details
    //
    protected:
        friend class Node;
        friend class Edge;

        /**
         * Register @p node in this document.
         * The Node calls this function in its constructor.
         */
        void registerNode(Node* node);

        /**
         * Unregister @p node from this document.
         * The Node calls this function in its destructor.
         */
        void unregisterNode(Node* node);

    private:
        DocumentPrivate * const d;
};

}

#endif // TIKZ_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
