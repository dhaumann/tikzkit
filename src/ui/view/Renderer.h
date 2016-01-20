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

#ifndef TIKZ_UI_RENDERER_H
#define TIKZ_UI_RENDERER_H

#include <QGraphicsView>

#include "tikzgui_export.h"

#include <tikz/core/Value.h>
#include <tikz/core/Pos.h>

namespace tikz {
namespace ui {

class DocumentPrivate;
class Ruler;
class Grid;
class ZoomController;

class Renderer : public QGraphicsView
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        Renderer(DocumentPrivate * document = 0, QWidget * parent = 0);

        /**
         * Destructor.
         */
        virtual ~Renderer();

        /**
         * Returns the associated tikz Document.
         */
        DocumentPrivate * document() const;

        /**
         * Snap @p value to the grid.
         */
        tikz::Value snapValue(const tikz::Value & value) const;

        /**
         * Snap x/y components of @p pos to the grid.
         */
        tikz::Pos snapPos(const tikz::Pos & pos) const;

        /**
         * Snap @p angle in degrees to a 15° raster.
         */
        qreal snapAngle(qreal angle) const;

        /**
         * Returns the ZoomController object that handles the zoom operations.
         */
        ZoomController * zoomController() const;

    public Q_SLOTS:
        /**
         * Sets the zoom factor. 1.0 maps to 100%.
         */
        void setZoom(qreal zoomFactor);

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the mouse moved on the view.
         */
        void mousePositionChanged(const tikz::Pos & pos);

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        bool viewportEvent(QEvent * event) override;

        void drawBackground(QPainter * painter, const QRectF & rect) override;

    private:
        DocumentPrivate * m_doc = nullptr;
        tikz::ui::Grid * m_grid = nullptr;
        tikz::ui::Ruler * m_hRuler = nullptr;
        tikz::ui::Ruler * m_vRuler = nullptr;
        tikz::ui::ZoomController * m_zoomController = nullptr;
        QPointF m_lastMousePos;
        bool m_handTool = false;
};

}
}

#endif // TIKZ_UI_RENDERER_H

// kate: indent-width 4; replace-tabs on;
