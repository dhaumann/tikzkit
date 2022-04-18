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
#include <tikz/core/Style.h>
#include <tikz/core/UndoSetProperty.h>
#include "Document.h"

#include <QGraphicsScene>
#include <QKeyEvent>

#include <QDebug>

namespace tikz {
namespace ui {

static void setProp(const tikz::core::Uid & entity, const QString & key, const QVariant & value)
{
    entity.document()->addUndoItem(new tikz::core::UndoSetProperty(entity, key, value));
}

SelectTool::SelectTool(tikz::ui::Document * doc, QGraphicsScene * scene)
    : AbstractTool(doc, scene)
{
}

SelectTool::~SelectTool()
{
}

void SelectTool::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
}

void SelectTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
}

void SelectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
}

void SelectTool::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_N) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Node"));
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        setProp(node->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
        setProp(node->style()->uid(), "minimumWidth", 4.0_mm);
        setProp(node->style()->uid(), "minimumHeight", 4.0_mm);
    }

    if (event->key() == Qt::Key_M) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Node"));
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        setProp(node->style()->uid(), "minimumWidth", 4.0_mm);
        setProp(node->style()->uid(), "minimumHeight", 4.0_mm);
        setProp(node->node()->uid(), "text", "x");
    }

    if (event->key() == Qt::Key_E) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Edge"));
        auto path = dynamic_cast<EdgePathItem *>(document()->createPathItem(tikz::PathType::Line));
        path->edgePath()->setEndPos(tikz::Pos(1, 0, tikz::Unit::Centimeter));
        setProp(path->path()->styleUid(), "arrowHead", QVariant::fromValue(tikz::Arrow::ToArrow));
    }

    if (event->key() == Qt::Key_C) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Ellipse"));
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        setProp(node->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeEllipse));
        setProp(node->style()->uid(), "minimumWidth", 4.0_mm);
        setProp(node->style()->uid(), "minimumHeight", 4.0_mm);
    }

    if (event->key() == Qt::Key_V) {
        tikz::core::Transaction transaction(document(), QStringLiteral("Create Ellipse"));
        tikz::ui::NodeItem * node = document()->createNodeItem();
        node->setPos(tikz::Pos(0, 0));
        tikz::core::Style ns;
        ns.setStyle(node->node()->style());
        ns.setFillColor(Qt::black);
        setProp(node->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeEllipse));
        setProp(node->style()->uid(), "minimumWidth", 1.0_mm);
        setProp(node->style()->uid(), "minimumHeight", 1.0_mm);
        setProp(node->style()->uid(), "innerSep", 0.0_mm);
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
