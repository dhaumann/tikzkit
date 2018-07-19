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

#ifndef TIKZ_UI_NODE_TEXT_ITEM_H
#define TIKZ_UI_NODE_TEXT_ITEM_H

#include "tikzgui_export.h"

#include <QGraphicsItem>

class QPainter;

namespace tikz {
namespace ui {

class NodeItem;
class NodeTextPrivate;

class NodeText : public QGraphicsItem
{
    friend NodeTextPrivate;
    public:
        NodeText(NodeItem* node);
        virtual ~NodeText();

    public:
        QRectF boundingRect() const override;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;

        QRectF textRect() const;

     private:
        NodeTextPrivate * const d;
};

}
}

#endif // TIKZ_UI_NODE_TEXT_ITEM_H

// kate: indent-width 4; replace-tabs on;
