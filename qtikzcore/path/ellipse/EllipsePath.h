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

#ifndef TIKZ_ELLIPSE_PATH_H
#define TIKZ_ELLIPSE_PATH_H

#include <Path.h>

#include <QPointF>

namespace tikz
{

class Node;
class Coord;
class EllipsePathPrivate;

/**
 * This class represents a TikZ ellipse path.
 * For instance, a typical edge looks like this:
 * @code
 * \draw (1, 1) ellipse (2cm and 1cm);
 * @endcode
 */
class TIKZCORE_EXPORT EllipsePath : public Path
{
    Q_OBJECT

    public:
        /**
         * Virtual destructor.
         */
        virtual ~EllipsePath();

    //
    // Node manipulation
    //
    public:

        /**
         * Returns the Coord.
         */
        Coord& coord() const;

        /**
         * Get the node, which was set with setNode(Node*).
         */
        Node* node() const;

    public Q_SLOTS:
        /**
         * Attach the ellipse to @p node;
         */
        void setNode(Node* node);

    //
    // x/y-position methods
    //
    public:
        /**
         * Get the position of the ellipse.
         * @note This is the same as node()->pos().
         */
        const QPointF & pos() const;

    public Q_SLOTS:
        /**
         * Set the position of the ellipse to @p pos.
         * @param pos the new position
         */
        void setPos(const QPointF& pos);

    //
    // anchor methods
    //
    public:
        /**
         * Get the anchor of the ellipse.
         */
        tikz::Anchor anchor() const;


    public Q_SLOTS:
        /**
         * Set the anchor of the ellipse to @p anchor.
         */
        void setAnchor(tikz::Anchor anchor);

    //
    // signals
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the node of this ellipse changes.
         * @param node may be 0
         */
        void nodeChanged(tikz::Node * node);

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Constructor that associates this path with the tikz Document @p doc.
         * @param type Path type
         * @param id unique id of the path
         * @param doc associated document
         */
        EllipsePath(qint64 id, Document* doc);

        /**
         * Destruct the ellipse by saving the position or node connection.
         */
        void deconstruct() override;

        /**
         * Detach the ellipse from @p node, since @p node is about to be deleted.
         */
        void detachFromNode(Node * node) override;

    //
    // internal
    //
    private:
        /**
         * Private default constructor, not implemented
         */
        EllipsePath();
    private:
        EllipsePathPrivate * const d;
};

}

#endif // TIKZ_ELLIPSE_PATH_H

// kate: indent-width 4; replace-tabs on;
