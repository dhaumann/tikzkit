/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZUI_METAPOS_H
#define TIKZUI_METAPOS_H

#include "tikzgui_export.h"
#include <tikz/ui/TikzDocument.h>
#include <tikz/core/MetaPos.h>

namespace tikz {
namespace ui {

class MetaPosPrivate;

/**
 * MetaPos describes a position in the tikz scene.
 * This position may either be a simple coordinate, or a node.
 * In case of a node, the anchor additionally takes effect.
 */
class TIKZUI_EXPORT MetaPos : public tikz::core::MetaPos
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        MetaPos(TikzDocument * document);

        /**
         * Destructor
         */
        virtual ~MetaPos();

        /**
         * Get the associated Document.
         */
        TikzDocument * tikzDocument() const;

    //
    // convenience functions
    //
    public:
        /**
         * Get a clone of this MetaPos as shared pointer.
         *
         * @note This method is provided for convenience.
         *       The returned shared pointer is a copy. Hence, modifying the
         *       returned MetaPos::Ptr does not change this MetaPos.
         */
        tikz::core::MetaPos::Ptr copy() const override;

    //
    // x/y-position methods
    //
    public:
        /**
         * Get the coordinate of this node.
         * If no Node is associated, the position set with setPos() is returned.
         * If node() is non-null, node()->pos() is returned.
         */
        QPointF pos() const override;

    private:
        /**
         * Disable default constructor.
         */
        MetaPos();

        /**
         * Private data pointer.
         */
        MetaPosPrivate * const d;
};

}
}

#endif // TIKZUI_METAPOS_H

// kate: indent-width 4; replace-tabs on;
