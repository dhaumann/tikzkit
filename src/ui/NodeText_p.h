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

#ifndef TIKZ_UI_NODE_TEXT_ITEM_PRIVATE_H
#define TIKZ_UI_NODE_TEXT_ITEM_PRIVATE_H

#include <QObject>
#include <QSvgRenderer>

#include "TexGenerator.h"

namespace tex {
    class TexGenerator;
    class PdfRenderer;
}

namespace tikz {
namespace ui {

class TikzNode;
class NodeText;

class NodeTextPrivate : public QObject
{
    Q_OBJECT

    public:
        NodeTextPrivate(TikzNode* tikzNode, NodeText* nodeText);

    public:
        NodeText* q;
        TikzNode* node;

        QSvgRenderer svgRenderer;
        tex::TexGenerator texGenerator;

    public:
        void updateCache();

    Q_SIGNALS:
        void svgChanged();

    public Q_SLOTS:
        void readSvgFile(const QString& file);
};

}
}

#endif // TIKZ_UI_NODE_TEXT_ITEM_PRIVATE_H

// kate: indent-width 4; replace-tabs on;
