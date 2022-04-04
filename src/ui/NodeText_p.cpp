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

#include "NodeText_p.h"
#include "NodeText.h"
#include "NodeItem.h"

#include "TexGenerator.h"

#include <tikz/core/Node.h>

#include <QDebug>

namespace tikz {
namespace ui {

NodeTextPrivate::NodeTextPrivate(NodeItem* nodeItem, NodeText* nodeText)
    : QObject()
    , svgRenderer(this)
    , texGenerator(this)
{
    q = nodeText;
    node = nodeItem;

    connect(&texGenerator, SIGNAL(svgReady(QString)), this, SLOT(readSvgFile(QString)));
    connect(this, SIGNAL(svgChanged()), nodeItem->node(), SIGNAL(changed()));
}

void NodeTextPrivate::updateCache()
{
}

void NodeTextPrivate::readSvgFile(const QString& file)
{
    q->prepareGeometryChange();
    svgRenderer.load(file);
    Q_EMIT svgChanged();
}

}
}

// kate: indent-width 4; replace-tabs on;
