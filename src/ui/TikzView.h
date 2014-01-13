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

#ifndef TIKZ_UI_TIKZ_VIEW_H
#define TIKZ_UI_TIKZ_VIEW_H

#include <QGraphicsView>

#include "tikzgui_export.h"


namespace tikz {
namespace ui {

class TikzDocument;
class TikzViewPrivate;

class TIKZGUI_EXPORT TikzView : public QGraphicsView
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        TikzView(TikzDocument * document = 0, QWidget * parent = 0);

        /**
         * Destructor.
         */
        virtual ~TikzView();

        /**
         * Returns the associated TikzDocument.
         */
        TikzDocument * document() const;

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        bool viewportEvent(QEvent * event) override;


    private:
        TikzViewPrivate * const d;
};

}
}

#endif // TIKZ_UI_TIKZ_VIEW_H

// kate: indent-width 4; replace-tabs on;
