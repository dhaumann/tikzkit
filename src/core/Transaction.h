/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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
 * You should \e never return control to the event loop while it has an
 * unterminated (i.e. this object is not destructed) transaction sequence, so
 * try always to do all of your work in one go!
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
         * By calling finish(), the editing transaction can be finished
         * already before destruction of this instance.
         *
         * @see start()
         */
        void finish();

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
