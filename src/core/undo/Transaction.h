/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014-2015 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_TRANSACTION_H
#define TIKZ_TRANSACTION_H

#include "tikz_export.h"

#include <QString>

namespace tikz {
namespace core {

class Document;
class TransactionPrivate;

/**
 * Undo/redo transaction support.
 *
 * Document modifications may consist of a sequence of changes, such as moving a
 * node and changing its text. The Transaction class allows to bunch together such
 * a sequence to a single undo/redo step.
 *
 * Using this class typically looks as follows:
 * @code
 * void foo()
 * {
 *     tikz::core::Transaction transaction(document, "Modify Node");
 *     // now call editing functions
 *     node->setPos(...);
 *     node->setText(...);
 * }
 * @endcode
 *
 * Although usually not required, a Transaction additionally allows to manually
 * call finish(). This way, the transaction is finished before the destructor,
 * and the destructor does nothing.
 *
 * Further, if a running transaction should be aborted, just call cancel().
 * This is handy whenever the user hits Escape during a modification.
 */
class TIKZKITCORE_EXPORT Transaction
{
    public:
        /**
         * Constructor. Will automatically start an editing transaction.
         *
         * @param document document for the transaction
         */
        explicit Transaction(Document * document);

        /**
         * Constructs the object and starts an editing transaction.
         *
         * @param document document for the transaction
         * @param name the transaction name
         */
        explicit Transaction(Document * document, const QString & name);

        /**
         * Destructs the object and, if needed, finishes a running editing
         * transaction by calling finish().
         *
         * @see finish()
         */
        ~Transaction();

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
        Transaction(const Transaction &) = delete;
        Transaction() = delete;

        /**
         * Document pointer
         */
        Document * m_document = nullptr;
};

}
}

#endif // TIKZ_TRANSACTION_H

// kate: indent-width 4; replace-tabs on;
