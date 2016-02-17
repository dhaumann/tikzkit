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

Transaction::Transaction(Document *document)
    : Transaction(document, QString())
{
}

Transaction::Transaction(Document * document, const QString & name)
    : m_document(document)
{
    // Alghouth it works in release-mode, we usually want a valid document
    Q_ASSERT(document != nullptr);

    // start the editing transaction
    if (m_document) {
        m_document->beginTransaction(name);
    }
}

void Transaction::cancel()
{
    if (m_document) {
        m_document->cancelTransaction();
    }
}

void Transaction::finish()
{
    if (m_document) {
        m_document->finishTransaction();
        m_document = nullptr;
    }
}

bool Transaction::isRunning() const
{
    return m_document != nullptr;
}

Transaction::~Transaction()
{
    // finish the editing transaction, if applicable
    finish();
}

}
}

// kate: indent-width 4; replace-tabs on;
