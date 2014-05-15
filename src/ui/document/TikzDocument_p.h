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

#ifndef TIKZ_UI_DOCUMENT_PRIVATE_D_H
#define TIKZ_UI_DOCUMENT_PRIVATE_D_H

#include <QObject>
#include <QVector>
#include <QHash>

class NodeItem;
class QGraphicsView;

namespace tikz {
namespace core {
    class Document;
    class Node;
    class Path;
}

namespace ui {

class PathItem;

class DocumentPrivate;
class TikzScene;


class TikzDocumentPrivate
{
    //
    // data types
    //
    public:
        /**
         * List of NodeItem%s.
         */
        QVector<NodeItem*> nodes;

        /**
         * List of tikz::ui::PathItem%s.
         */
        QVector<tikz::ui::PathItem *> paths;

        /**
         * Node lookup map
         */
        QHash<qint64, NodeItem*> nodeMap;

        /**
         * Edge lookup map
         */
        QHash<qint64, tikz::ui::PathItem *> pathMap;

        /**
         * Graphics scene for the document.
         */
        TikzScene * scene;

        /**
         * List of graphics views.
         */
        QVector<QGraphicsView*> views;
};

}
}

#endif // TIKZ_UI_DOCUMENT_PRIVATE_D_H

// kate: indent-width 4; replace-tabs on;
