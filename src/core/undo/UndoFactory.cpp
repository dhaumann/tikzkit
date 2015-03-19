/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015 Dominik Haumann <dhaumann@kde.org>
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

#include "UndoFactory.h"
#include "Document.h"

#include "UndoCreateNode.h"
#include "UndoDeleteNode.h"
#include "UndoSetNodePos.h"
#include "UndoSetNodeStyle.h"
#include "UndoSetNodeStyle.h"
#include "UndoSetNodeText.h"
#include "UndoSetEdgePos.h"
#include "UndoSetEllipsePos.h"
#include "UndoCreatePath.h"
#include "UndoDeletePath.h"
#include "UndoSetPathStyle.h"

namespace tikz {
namespace core {

class UndoFactoryPrivate {
public:
    /**
     * Pointer to the document of this undo/redo item.
     */
    Document* doc = nullptr;

    /**
     * Description of the undo item.
     */
    QString text;
};

UndoFactory::UndoFactory(Document* doc)
    : d(new UndoFactoryPrivate())
{
    d->doc = doc;
}

UndoFactory::~UndoFactory()
{
    delete d;
}

Document* UndoFactory::document()
{
    return d->doc;
}

UndoItem * UndoFactory::createItem(const QJsonObject & json)
{
    const QString type = json["type"].toString();
    if (type.isEmpty()) {
        Q_ASSERT(false);
        return nullptr;
    }

    if (type == "node-create") {
        return new UndoCreateNode(json, document());
    } else if (type == "node-delete") {
        return new UndoDeleteNode(json, document());
    } else if (type == "node-set-pos") {
        return new UndoSetNodePos(json, document());
    } else if (type == "node-set-style") {
        return new UndoSetNodeStyle(json, document());
    } else if (type == "node-set-text") {
        return new UndoSetNodeText(json, document());
    } else if (type == "edge-set-pos") {
        return new UndoSetEdgePos(json, document());
    } else if (type == "ellipse-set-pos") {
        return new UndoSetEllipsePos(json, document());
    } else if (type == "path-create") {
        return new UndoCreatePath(json, document());
    } else if (type == "path-delete") {
        return new UndoDeletePath(json, document());
    } else if (type == "path-set-style") {
        return new UndoSetPathStyle(json, document());
    }

    Q_ASSERT(false);
    return nullptr;
}

}
}

// kate: indent-width 4; replace-tabs on;
