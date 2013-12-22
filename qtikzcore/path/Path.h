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

#ifndef TIKZ_PATH_H
#define TIKZ_PATH_H

#include <QObject>

#include "tikz_export.h"
#include "tikz.h"

namespace tikz
{

class Document;
class Coord;
class Edge;
class EdgeStyle;
class Visitor;
class PathPrivate;

class TIKZCORE_EXPORT Path : public QObject
{
    Q_OBJECT

    //
    // Path type
    //
    public:
        enum Type {
            Line = 0, // (a) -- (b)
            HVLine, // (a) -| (b)
            VHLine, // (a) |- (b)
            BendCurve, // (a) to[bend left=20, looseness=1.2] (b)
            InOutCurve, // (a) to[in=20, out=30] (b)
            BezierCurve, // (a) .. controls (b) and (c) .. (d)
            Ellipse, // (a) ellipse[x radius=1cm, y radius=2cm]
            Rectangle, // (a) rectangle (b)
            Grid, // (a) grid (b)
        };

        /**
         * Returns the element type of this edge.
         */
        virtual Path::Type type() const;

    public:
        /**
         * Virtual destructor.
         */
        virtual ~Path();

        /**
         * Returns the associated document.
         */
        Document * document() const;

        /**
         * Document wide unique id.
         */
        qint64 id() const;

    //
    // reference counted config
    //
    public:
        /**
         * Increase config reference counter.
         * For beginConfig() call must have a matching endConfig() call.
         * When the reference counter is 0, changed() is emitted.
         */
        void beginConfig();

        /**
         * Decrease config reference counter.
         * For beginConfig() call must have a matching endConfig() call.
         */
        void endConfig();

    public Q_SLOTS:
        /**
         * Emits changed() if reference counter is 0.
         * Otherwise, emitting changed() is delayed until the reference
         * counter is 0 after a call of endConfig().
         */
        void emitChangedIfNeeded();

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the Path changes.
         * This includes the style, the number of edges, edge types etc.
         */
        void changed();

    //
    // visitor pattern
    //
    public:
        /**
         * Visitor pattern.
         * Visits all elements of the document.
         */
        bool accept(tikz::Visitor & visitor);

    //
    // path properties
    //
    public:
        /**
         * Get the Style object of this path.
         */
        EdgeStyle* style() const;

        /**
         * Set the Style of this path to @p style.
         */
        void setStyle(const EdgeStyle & style);

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Constructor that associates this path with the tikz Document @p doc.
         * @param id unique id of the path
         * @param doc associated document
         */
        Path(qint64 id, Document* doc);

        /**
         * This function is called by Document::deletePath() right before the
         * Path is deleted. Invoking the undo action will construct the path
         * again. Therefore, this function needs to add all undo items so that
         * the respective call of undo() will add properties of the Path again.
         *
         * @note: the EdgeStyle of this path is taken care of. No need to put
         *        this into the undo stack again.
         *
         * The default implementation is empty.
         */
        virtual void deconstruct();

    //
    // internal
    //
    private:
        /**
         * Private default constructor, not implemented
         */
        Path();
    private:
        PathPrivate * const d;
};

}

#endif // TIKZ_PATH_H

// kate: indent-width 4; replace-tabs on;
