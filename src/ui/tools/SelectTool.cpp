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
#include <tikz/core/NodeStyle.h>
#include <tikz/core/EdgeStyle.h>
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
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Node"));
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        tikz::core::NodeStyle ns;
        ns.setStyle(node->node()->style());
        ns.setShape(tikz::ShapeRectangle);
        ns.setMinimumWidth(tikz::Value(4, Millimeter));
        ns.setMinimumHeight(tikz::Value(4, Millimeter));
        node->node()->setStyle(ns);
    }

    if (event->key() == Qt::Key_M) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Node"));
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        tikz::core::NodeStyle ns;
        ns.setStyle(node->node()->style());
        ns.setMinimumWidth(tikz::Value(4, Millimeter));
        ns.setMinimumHeight(tikz::Value(4, Millimeter));
        node->node()->setStyle(ns);
        node->node()->setText("x");
    }

    if (event->key() == Qt::Key_E) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Edge"));
        auto path = dynamic_cast<EdgePathItem *>(document()->createPathItem(tikz::core::Path::Line));
        path->edgePath()->setStartPos(tikz::Pos(-1, 0, tikz::Unit::Centimeter));
        path->edgePath()->setEndPos(tikz::Pos(1, 0, tikz::Unit::Centimeter));

        tikz::core::EdgeStyle es;
        es.setArrowHead(tikz::ToArrow);
        path->path()->setStyle(es);
    }

    if (event->key() == Qt::Key_C) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Ellipse"));
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        tikz::core::NodeStyle ns;
        ns.setStyle(node->node()->style());
        ns.setShape(tikz::ShapeEllipse);
        ns.setMinimumWidth(tikz::Value(4, Millimeter));
        ns.setMinimumHeight(tikz::Value(4, Millimeter));
        node->node()->setStyle(ns);
    }

    if (event->key() == Qt::Key_V) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Ellipse"));
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        tikz::core::NodeStyle ns;
        ns.setStyle(node->node()->style());
        ns.setFillColor(Qt::black);
        ns.setShape(tikz::ShapeEllipse);
        ns.setMinimumWidth(tikz::Value(1, Millimeter));
        ns.setMinimumHeight(tikz::Value(1, Millimeter));
        ns.setInnerSep(tikz::Value(0, Millimeter));
        node->node()->setStyle(ns);
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
