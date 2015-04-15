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

#include "ProxyTool.h"
#include "SelectTool.h"
#include "NodeTool.h"
#include "EllipseTool.h"
#include "LineTool.h"
#include "NodeItem.h"
#include "PathItem.h"

#include <tikz/core/Path.h>

#include <QGraphicsScene>
#include <QKeyEvent>

#include <QDebug>

namespace tikz {
namespace ui {

ProxyTool::ProxyTool(tikz::ui::Document * doc, QGraphicsScene * graphicsScene)
    : AbstractTool(doc, graphicsScene)
    , m_tool(nullptr)
{
    // keep track of selected items to always select correct tool
    connect(scene(), SIGNAL(selectionChanged()), this, SLOT(updateTool()));

    // update tool once now
    updateTool();
}

ProxyTool::~ProxyTool()
{
}

void ProxyTool::mouseEnteredScene()
{
    m_tool->mouseEnteredScene();
}

void ProxyTool::mouseLeftScene()
{
    m_tool->mouseLeftScene();
}

void ProxyTool::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    m_tool->mouseMoveEvent(event);
}

void ProxyTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    m_tool->mousePressEvent(event);
}

void ProxyTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    m_tool->mouseReleaseEvent(event);
}

void ProxyTool::keyPressEvent(QKeyEvent * event)
{
    m_tool->keyPressEvent(event);
}

void ProxyTool::updateTool()
{
    qDebug() << "ProxyTool::updateTool(): Changing tool";
    if (m_tool) {
        delete m_tool;
        m_tool = nullptr;
    }

    QList<QGraphicsItem *> items = scene()->selectedItems();
    if (items.size() == 1) {
        if (dynamic_cast<NodeItem *>(items[0])) {
            m_tool = new NodeTool(static_cast<NodeItem *>(items[0]), scene());
        } else if (dynamic_cast<tikz::ui::PathItem*>(items[0])) {
            tikz::ui::PathItem * path = static_cast<tikz::ui::PathItem *>(items[0]);
            const tikz::PathType type = path->path()->type();
            switch (type) {
                case tikz::PathType::Line: m_tool = new LineTool(path, scene()); break;
                case tikz::PathType::HVLine: break;
                case tikz::PathType::VHLine: break;
                case tikz::PathType::BendCurve: break;
                case tikz::PathType::InOutCurve: break;
                case tikz::PathType::BezierCurve: break;
                case tikz::PathType::Ellipse: m_tool = new EllipseTool(path, scene()); break;
                case tikz::PathType::Rectangle: break;
                case tikz::PathType::Grid: break;
                case tikz::PathType::Invalid: break;
                default: Q_ASSERT(false);
            }
        }
    }

    //
    // make sure we always have a valid tool
    //
    if (! m_tool) {
        m_tool = new SelectTool(document(), scene());
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
