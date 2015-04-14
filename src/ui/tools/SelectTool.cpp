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

#include "SelectTool.h"

#include "NodeItem.h"
#include "EdgePathItem.h"
#include <tikz/core/EdgePath.h>
#include <tikz/core/Transaction.h>
#include "Document.h"

#include <QGraphicsScene>
#include <QKeyEvent>

#include <QDebug>

namespace tikz {
namespace ui {

SelectTool::SelectTool(tikz::ui::Document * doc, QGraphicsScene * scene)
    : AbstractTool(doc, scene)
{
}

SelectTool::~SelectTool()
{
}

void SelectTool::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "mouse move event";
}

void SelectTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "mouse press event";
}

void SelectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "mouse release event";
}

void SelectTool::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_N) {
        tikz::core::Transaction(document(), "Create Node");
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        node->node()->setText("x");
    }

    if (event->key() == Qt::Key_E) {
        tikz::core::Transaction(document(), "Create Edge");
        auto path = dynamic_cast<EdgePathItem *>(document()->createPathItem(tikz::core::Path::Line));
        path->edgePath()->setStartPos(tikz::Pos(-1, 0, tikz::Unit::Centimeter));
        path->edgePath()->setEndPos(tikz::Pos(1, 0, tikz::Unit::Centimeter));
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
