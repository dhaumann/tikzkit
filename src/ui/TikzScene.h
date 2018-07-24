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

#ifndef TIKZ_UI_TIKZ_SCENE_H
#define TIKZ_UI_TIKZ_SCENE_H

#include <QGraphicsScene>

#include "tikzui_export.h"
#include "tikzui.h"

namespace tikz {
namespace ui {

class DocumentPrivate;
class TikzScenePrivate;

class TIKZUI_EXPORT TikzScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        TikzScene(DocumentPrivate * document = nullptr);

        /**
         * Destructor.
         */
        virtual ~TikzScene();

        /**
         * Returns the associated DocumentPrivate.
         */
        DocumentPrivate * document() const;

    //
    // Edit mode
    //
    public Q_SLOTS:
        /**
         * Set the edit mode to @p mode.
         */
        void setEditMode(TikzEditMode mode);

    public:
        /**
         * Get the edit mode.
         */
        TikzEditMode editMode() const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the edit mode changed through setEditMode.
         */
        void editModeChanged(TikzEditMode mode) const;

    protected:
        /**
         * Reimplemented to pass mouse press events to tool handler.
         */
        void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent) override;

        /**
         * Reimplemented to pass mouse move events to tool handler.
         */
        void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent) override;

        /**
         * Reimplemented to pass mouse release events to tool handler.
         */
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent) override;

        /**
         * Reimplemented to pass key press events to tool handler.
         */
        void keyPressEvent(QKeyEvent * keyEvent) override;

    private Q_SLOTS:
        /**
         * Updates the scene rect such that the entire contents is visible.
         */
        void updateSceneRect();

    private:
        TikzScenePrivate * const d;
};

}
}

#endif // TIKZ_UI_TIKZ_SCENE_H

// kate: indent-width 4; replace-tabs on;
