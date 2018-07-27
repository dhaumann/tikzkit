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

#include "UndoCreateEntity.h"
#include "UndoDeleteEntity.h"
#include "UndoSetNodePos.h"
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

UndoItem * UndoFactory::createItem(const QString & type)
{
    if (type.isEmpty()) {
        Q_ASSERT(false);
        return nullptr;
    }

    if (type == "entity-create") {
        return new UndoCreateEntity(document());
    } else if (type == "entity-delete") {
        return new UndoDeleteEntity(document());
    } else if (type == "node-set-pos") {
        return new UndoSetNodePos(document());
    } else if (type == "node-set-style") {
        return new UndoSetNodeStyle(document());
    } else if (type == "node-set-text") {
        return new UndoSetNodeText(document());
    } else if (type == "edge-set-pos") {
        return new UndoSetEdgePos(document());
    } else if (type == "ellipse-set-pos") {
        return new UndoSetEllipsePos(document());
    } else if (type == "path-create") {
        return new UndoCreatePath(document());
    } else if (type == "path-delete") {
        return new UndoDeletePath(document());
    } else if (type == "path-set-style") {
        return new UndoSetPathStyle(document());
    }

    Q_ASSERT(false);
    return nullptr;
}

}
}

// kate: indent-width 4; replace-tabs on;
