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

#ifndef TIKZ_METAPOS_P_H
#define TIKZ_METAPOS_P_H

#include "tikz.h"

#include "Pos.h"
#include <QSharedPointer>

namespace tikz {
namespace core {

class Document;
class MetaPos;
class MetaPosHelper;

/**
 * Private data structure for MetaPos.
 */
class MetaPosPrivate : public QObject
{
    Q_OBJECT

    public:
        MetaPos * q;
        Document * doc;

        tikz::Pos pos;
        qint64 nodeId;
        Anchor anchor;

    public:
        MetaPosPrivate(MetaPos * metaPos);

    public:
        /**
         * Start changing the MetaPos.
         * This call is ref-counted. For each beginChange() you finally
         * have to call endChange().
         */
        void beginChange();

        /**
         * End of changing the MetaPos.
         * This will call MetaPos::changed(), if the number of calls of endChange()
         * matches the calls the one of beginConfig(), i.e. the ref-counter is zero.
         *
         * Using beginConfig() and endChange() allows to change multiple
         * config values, while still only emitting the changed() signal only once.
         */
        void endChange();

    public Q_SLOTS:
        /**
         * This slot is called whenever attached Node%s changed.
         */
        void changeRequest();

    Q_SIGNALS:
        /**
         * This signal is emitted either when a new node is set with setNode(),
         * when the x/y position of the node changed, or when the associated
         * NodeStyle of the node changed.
         */
        void changed(tikz::core::MetaPos * metaPos);

    protected:
        /**
         * Change ref counter;
         */
        int changeRefCounter;
};

}
}

#endif // TIKZ_METAPOS_P_H

// kate: indent-width 4; replace-tabs on;
