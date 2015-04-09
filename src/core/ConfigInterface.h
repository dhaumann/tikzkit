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

#ifndef TIKZ_CORE_CONFIG_INTERFACE_H
#define TIKZ_CORE_CONFIG_INTERFACE_H

#include "tikz_export.h"

#include <QObject>

namespace tikz {
namespace core {

/**
 * Base class that signals change events.
 *
 * The ConfigInterface contains a changed() signal.
 * @TODO FIXME
 */
class TIKZCORE_EXPORT ConfigInterface : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor with optional parent.
         */
        explicit ConfigInterface(QObject * parent = nullptr);

        /**
         * Virtual destructor.
         */
        virtual ~ConfigInterface();

    //
    // config methods
    //
    public:
        /**
         * Start changing properties.
         * This call is ref-counted. For each beginConfig() you finally
         * have to call endConfig().
         */
        void beginConfig();

        /**
         * End of changing properties.
         * This will emit changed(), if the number of calls of endConfig()
         * matches the calls the one of beginConfig(), i.e. the ref-counter is zero.
         *
         * Using beginConfig() and endConfig() allows to change multiple
         * config values, while still only emitting the changed() signal only once.
         */
        void endConfig();

        /**
         * Returns whether beginConfig() was called without an endConfig() yet.
         */
        bool configActive() const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the style changes.
         * This includes changes in the parent style that possibly influence
         * the appearance of this style.
         */
        void changed();

    public Q_SLOTS:
        /**
         * Emits changed() if the config reference counter is 0.
         * Otherwise, emitting changed() is delayed until the reference
         * counter is 0 after a call of endConfig().
         */
        void emitChangedIfNeeded();

    private:
        int m_refCounter = 0;
};

}
}

#endif // TIKZ_CORE_CONFIG_INTERFACE_H

// kate: indent-width 4; replace-tabs on;
