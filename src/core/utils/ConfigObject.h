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

#ifndef TIKZ_CORE_CONFIG_OBJECT_H
#define TIKZ_CORE_CONFIG_OBJECT_H

#include "tikz_export.h"

#include <QObject>

namespace tikz {
namespace core {

/**
 * Base class that signals change events.
 *
 * The ConfigObject contains a changed() signal.
 * @TODO FIXME
 */
class TIKZCORE_EXPORT ConfigObject : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor with optional parent.
         */
        explicit ConfigObject(QObject * parent = nullptr);

        /**
         * Virtual destructor.
         */
        virtual ~ConfigObject();

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

    //
    // For derived classes to override.
    //
    protected:
        /**
         * This function is called whenever this object is about to be changed.
         * The default implementation is empty.
         */
        virtual void notifyAboutToChange();

        /**
         * This function is called whenever this object was changed.
         * The default implementation is empty.
         */
        virtual void notifyChanged();

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the object is about to change.
         * At this point, no changes have been made, yet.
         */
        void aboutToChange();

        /**
         * This signal is emitted whenever either this object's data changed,
         * or when a related object's data changed and this data somehow depends
         * on it.
         *
         * That is, this signal could be emitted, even though notifyChanged()
         * was not called.
         *
         * If you need to rely on all changes, for instance, to keep the GUI
         * up-to-date, then use this signal.
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

class TIKZCORE_EXPORT ConfigTransaction
{
public:
    // Disable some constructors.
    ConfigTransaction() = delete;
    ConfigTransaction(const ConfigTransaction & other) = delete;

    /**
     * Constructor that immediately calls configObject->beginConfig().
     */
    ConfigTransaction(ConfigObject * configObject);

    /**
     * Destructor that calls configObject->endConfig(), if required.
     */
    ~ConfigTransaction();

    /**
     * Call endConfig() on the ConfigObject passed in the constructor.
     * You may call this only once.
     */
    void endConfig();

private:
    ConfigObject * m_configObject = nullptr;
};

}
}

#endif // TIKZ_CORE_CONFIG_OBJECT_H

// kate: indent-width 4; replace-tabs on;
