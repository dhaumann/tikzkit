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
#include "TikzNode.h"

#include <QGraphicsScene>
#include <QKeyEvent>

#include <QDebug>

ProxyTool::ProxyTool(QGraphicsScene * graphicsScene)
    : AbstractTool(graphicsScene)
    , m_tool(0)
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
    qDebug() << "proxy: mouse entered scene";
    if (m_tool) {
        m_tool->mouseEnteredScene();
    }
}

void ProxyTool::mouseLeftScene()
{
    qDebug() << "proxy: mouse left scene";
    if (m_tool) {
        m_tool->mouseLeftScene();
    }
}

void ProxyTool::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "proxy: mouse move event";
    if (m_tool) {
        m_tool->mouseMoveEvent(event);
    }
}

void ProxyTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "proxy: mouse press event";
    if (m_tool) {
        m_tool->mousePressEvent(event);
    }
}

void ProxyTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "proxy: mouse release event";
    if (m_tool) {
        m_tool->mouseReleaseEvent(event);
    }
}

void ProxyTool::keyPressEvent(QKeyEvent * event)
{
    qDebug() << "proxy: key press event";
    if (m_tool) {
        m_tool->keyPressEvent(event);
    }
}

void ProxyTool::updateTool()
{
    if (m_tool) {
        delete m_tool;
    }

    QList<QGraphicsItem *> items = scene()->selectedItems();
    if (items.size() == 1 && dynamic_cast<TikzNode *>(items[0])) {
        m_tool = new NodeTool(static_cast<TikzNode *>(items[0]), scene());
    } else {
        m_tool = new SelectTool(scene());
    }
}

// kate: indent-width 4; replace-tabs on;
