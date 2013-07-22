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

#ifndef TIKZGUI_DOCUMENT_H
#define TIKZGUI_DOCUMENT_H

#include "tikzgui_export.h"

#include <QObject>

class QGraphicsView;

class TikzNode;
class TikzEdge;

class TikzDocumentPrivate;

class TIKZGUI_EXPORT TikzDocument : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzDocument(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzDocument();

        /**
         * Create a graphics view for this document.
         */
        QGraphicsView * createView();

    public Q_SLOTS:
        /**
         * Create a node
         */
        TikzNode * createNode();

    private:
        TikzDocumentPrivate * d;
};

#endif // TIKZGUI_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;