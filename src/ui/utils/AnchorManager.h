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

#ifndef TIKZ_UI_ANCHOR_MANAGER_H
#define TIKZ_UI_ANCHOR_MANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>

#include <tikz/core/MetaPos.h>

class QGraphicsScene;
class QGraphicsView;

namespace tikz {
namespace core {
    class Document;
}

namespace ui {

class TikzNode;
class AnchorHandle;

class AnchorManager : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        AnchorManager(QGraphicsScene * scene, tikz::core::Document * document, QObject * parent = 0);

        /**
         * Destructor.
         */
        virtual ~AnchorManager();

        /**
         * Returns the associated QGraphicsScene.
         */
        QGraphicsScene * scene() const;

    //
    // show/hide Node anchor handles
    //
    public Q_SLOTS:
        /**
         * Hide all existing anchor handles.
         */
        void hideAnchors();

        /**
         * Show all existing anchor handles.
         */
        void showAnchors();

        /**
         * Show the anchor handles for the node located at @p scenePos,
         * if applicable.
         *
         * If anchors are located at @p scenePos, true is returned.
         */
        bool showAnchors(const QPointF & scenePos);

        /**
         * Add all Node anchors for @p node.
         */
        void addNodeAnchors(TikzNode * node);

        /**
         * Removes all Node anchors for @p node.
         */
        void removeNodeAnchors(TikzNode * node);

        /**
         * Remove all 
         */
        void clear();

    public:
        /**
         * Get the node with anchor for the scene position @p scenePos.
         * If a node/anchor qt @p scenePos exists, the returned MetaPos
         * contains the metaPos->node() and metaPos->anchor(), otherwise
         * the returned MetaPos points to @p scenePos.
         *
         * The QGraphicsView @p view is required, since @p scenePos must first
         * be transformed into view coordinates as the handles have the flag
         * QGraphicsItem::ItemIgnoresTransformations is set.
         */
        tikz::core::MetaPos anchorAt(const QPointF & scenePos,
                                     QGraphicsView * view);

    //
    // internal to the manager
    //
    protected Q_SLOTS:
        /**
         * This slot is called right before a node is destroyed. At this point,
         * the node is already half destroyed, i.e., only the QObject part is
         * still valid.
         */
        void nodeDestroyed(QObject * obj);

    private:
        tikz::core::Document * m_doc;
        QGraphicsScene * m_scene;
        QVector <TikzNode *> m_nodes;
        QHash <TikzNode *, QVector<AnchorHandle *>> m_handleMap;
};

}
}

#endif // TIKZ_UI_ANCHOR_MANAGER_H

// kate: indent-width 4; replace-tabs on;
