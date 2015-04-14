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
 *     tikz::core::Transaction transaction(document);
 *     // now call editing functions
 *     node->setPos(...);
 *     node->setText(...);
 * }
 * @endcode
 *
 * Although usually not required, a Transaction additionally allows to manually
 * call finish() and start() in between.
 *
 * Further, if a running transaction should be aborted, just call cancel().
 * This is handy whenever the user hits Escape during a modification.
 */
class TIKZCORE_EXPORT Transaction
{
    public:
        /**
         * Constructs the object and starts an editing transaction by
         * calling start().
         *
         * @param document document for the transaction
         * @see start()
         */
        explicit Transaction(Document * document, bool autoStart = true);

        /**
         * Constructs the object and starts an editing transaction.
         *
         * @param document document for the transaction
         * @param name the transaction name
         */
        explicit Transaction(Document * document, const QString & name);

        /**
         * Destructs the object and, if needed, finishs a running editing
         * transaction by calling finish().
         *
         * @see finish()
         */
        ~Transaction();

        /**
         * By calling start(), the editing transaction can be started again.
         * This function only is of use in combination with finish().
         *
         * @note start() is called in the constructor of Transaction.
         *
         * @see finish()
         */
        void start(const QString & action = QString());

        /**
         * Cancels the running transaction.
         * If the transaction is canceled, finish() is not automatically
         * called in the destructor.
         */
        void cancel();

        /**
         * By calling finish(), the editing transaction can be finished
         * already before destruction of this instance.
         *
         * @see start(), cancel()
         */
        void finish();

        /**
         * Check whether this transaction currently is active.
         */
        bool isRunning() const;

    private:
        /**
         * no copying allowed
         */
        Q_DISABLE_COPY(Transaction)

        /**
         * private d-pointer
         */
        TransactionPrivate *const d;
};

}
}

#endif // TIKZ_TRANSACTION_H

// kate: indent-width 4; replace-tabs on;
