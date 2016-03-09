/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014-2016 Dominik Haumann <dhaumann@kde.org>
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

#ifndef ENTITY_SSTEM_HISTORY_TRANSACTION_H
#define ENTITY_SSTEM_HISTORY_TRANSACTION_H

#include "es_export.h"

#include <QString>

namespace es {

class Document;

/**
 * Undo/redo transaction support.
 *
 * Document modifications may consist of a sequence of changes, such as moving a
 * node and changing its text. The HistoryTransaction class allows to bunch together such
 * a sequence to a single undo/redo step.
 *
 * Using this class typically looks as follows:
 * @code
 * void foo()
 * {
 *     es::HistoryTransaction transaction(document, "Modify Node");
 *     // now call editing functions
 *     node->setPos(...);
 *     node->setText(...);
 * }
 * @endcode
 *
 * Although usually not required, a HistoryTransaction additionally allows to manually
 * call finish(). This way, the transaction is finished before the destructor,
 * and the destructor does nothing.
 *
 * Further, if a running transaction should be aborted, just call cancel().
 * This is handy whenever the user hits Escape during a modification.
 */
class ES_EXPORT HistoryTransaction
{
    public:
        /**
         * Constructor. Will automatically start an editing transaction.
         *
         * @param document document for the transaction
         */
        explicit HistoryTransaction(Document * document);

        /**
         * Constructs the object and starts an editing transaction.
         *
         * @param document document for the transaction
         * @param name the transaction name
         */
        explicit HistoryTransaction(Document * document, const QString & name);

        /**
         * Destructs the object and, if needed, finishes a running editing
         * transaction by calling finish().
         *
         * @see finish()
         */
        ~HistoryTransaction();

        /**
         * Tells the history transaction to roolback on finish().
         */
        void cancel();

        /**
         * By calling finish(), the editing transaction can be finished
         * already before destruction of this instance.
         *
         * @see cancel()
         */
        void finish();

        /**
         * Check whether this transaction currently is active.
         */
        bool isRunning() const;

    private:
        // disallow copy constructor
        HistoryTransaction(const HistoryTransaction &) = delete;
        HistoryTransaction() = delete;

        /**
         * Document pointer
         */
        Document * m_document = nullptr;
};

}

#endif // ENTITY_SSTEM_HISTORY_TRANSACTION_H

// kate: indent-width 4; replace-tabs on;
