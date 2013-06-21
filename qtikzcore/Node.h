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

#ifndef TIKZ_NODE_H
#define TIKZ_NODE_H

#include "tikz_export.h"
#include "tikz.h"
#include "Coord.h"

#include <QPointF>
#include <QString>

namespace tikz
{

class NodePrivate;
class NodeStyle;
class Document;

class TIKZCORE_EXPORT Node : public Coord
{
    Q_OBJECT

    public:
        /**
         * Constructor that associates this node with the tikz Document @p doc.
         * @param doc associated document, may be 0
         */
        Node(Document* doc = 0);

        /**
         * Destructor
         */
        virtual ~Node();

        /**
         * Sets the text of this node to @p text.
         */
        void setText(const QString& text);

        /**
         * Returns the text of this node.
         */
        QString text() const;

        /**
         * Get the Style object of this node.
         */
        NodeStyle* style() const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever this node changes, either due to
         * a change of its position, or changes in its style.
         * This signal is not emitted when the text changed.
         */
        void changed();

        /**
         * This signal is emitted whenever this node's text changed.
         */
        void textChanged(const QString& text);

    private:
        NodePrivate * const d;
};

}

#endif // TIKZ_NODE_H

// kate: indent-width 4; replace-tabs on;
