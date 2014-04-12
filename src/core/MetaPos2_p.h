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

#include <QPointF>
#include <QSharedPointer>

namespace tikz {
namespace core {

class Document;
class MetaPos2;
class MetaPosHelper;
class MetaPosFeedback;

/**
 * Private data structure for MetaPos.
 */
class MetaPos2Private
{
    public:
        MetaPos2 * q;
        Document * doc;
        MetaPosHelper * help;
        MetaPosFeedback * feedback;

        QPointF pos;
        qint64 nodeId;
        Anchor anchor;

    public:
        MetaPos2Private(MetaPos2 * metaPos);
        ~MetaPos2Private();
        MetaPosHelper * helper();
        void freeHelper();

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

    protected:
        /**
         * Call this function to tell the registered MetaPosFeedback
         * object that this MetaPos changed.
         */
        void changed();

        /**
         * Change ref counter;
         */
        int changeRefCounter;
};

/**
 * QObject-derived helper class to propagate node changes
 * to the associated MetaPos.
 *
 * @note This class exists only to use Qt signals/slots.
 *       Deriving MetaPos from QObject is not an option, since then the
 *       value semantics would be lost.
 */
class MetaPosHelper : public QObject
{
    Q_OBJECT

    public:
        MetaPosHelper(MetaPos2 * metaPos);

    public Q_SLOTS:
        void propagateNodeChange();

    private:
        MetaPos2 * m_metaPos;
};

}
}

#endif // TIKZ_METAPOS_P_H

// kate: indent-width 4; replace-tabs on;
