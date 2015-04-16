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

#include "Transaction.h"
#include "Document.h"

namespace tikz {
namespace core {

/**
 * Private d-pointer type for Transaction
 */
class TransactionPrivate {
    public:
        /**
         * real document implementation
         */
        Document *document = nullptr;

        /**
         * Indicator for running editing transaction
         */
        bool transactionRunning = false;
};

Transaction::Transaction(Document *document)
    : d (new TransactionPrivate())
{
    // Alghouth it works in release-mode, we usually want a valid document
    Q_ASSERT(document != nullptr);

    // initialize d-pointer
    d->document = document;
}

Transaction::Transaction(Document * document, const QString & name)
    : d (new TransactionPrivate())
{
    // Alghouth it works in release-mode, we usually want a valid document
    Q_ASSERT(document != nullptr);

    // initialize d-pointer
    d->document = document;

    // start the editing transaction
    start(name);
}

void Transaction::start(const QString & action)
{
    Q_ASSERT(! d->transactionRunning);

    if (d->document && !d->transactionRunning) {
        d->document->beginTransaction(action);
        d->transactionRunning = true;
    }
}

void Transaction::cancel()
{
    Q_ASSERT(d->transactionRunning);

    if (d->document && d->transactionRunning) {
        d->document->cancelTransaction();
    }
}

void Transaction::finish()
{
    Q_ASSERT(d->transactionRunning);

    if (d->document && d->transactionRunning) {
        d->document->finishTransaction();
        d->transactionRunning = false;
    }
}

bool Transaction::isRunning() const
{
    return d->transactionRunning;
}

Transaction::~Transaction()
{
    /**
     * finish the editing transaction
     */
    if (d->transactionRunning) {
        finish();
    }

    /**
     * delete our d-pointer
     */
    delete d;
}

}
}

// kate: indent-width 4; replace-tabs on;
