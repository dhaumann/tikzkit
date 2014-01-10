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
#include "TikzNode.h"
#include "TikzPath.h"

#include <Path.h>

#include <QGraphicsScene>
#include <QKeyEvent>

#include <QDebug>

ProxyTool::ProxyTool(QGraphicsScene * graphicsScene)
    : AbstractTool(graphicsScene)
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
        if (dynamic_cast<TikzNode *>(items[0])) {
            m_tool = new NodeTool(static_cast<TikzNode *>(items[0]), scene());
        } else if (dynamic_cast<TikzPath*>(items[0])) {
            TikzPath * path = static_cast<TikzPath *>(items[0]);
            const tikz::core::Path::Type type = path->path()->type();
            switch (type) {
                case tikz::core::Path::Line: break;
                case tikz::core::Path::HVLine: break;
                case tikz::core::Path::VHLine: break;
                case tikz::core::Path::BendCurve: break;
                case tikz::core::Path::InOutCurve: break;
                case tikz::core::Path::BezierCurve: break;
                case tikz::core::Path::Ellipse: m_tool = new EllipseTool(static_cast<TikzPath *>(items[0]), scene()); break;
                case tikz::core::Path::Rectangle: break;
                case tikz::core::Path::Grid: break;
                case tikz::core::Path::Invalid: break;
                default: Q_ASSERT(false);
            }
        }
    }

    //
    // make sure we always have a valid tool
    //
    if (! m_tool) {
        m_tool = new SelectTool(scene());
    }
}

// kate: indent-width 4; replace-tabs on;
