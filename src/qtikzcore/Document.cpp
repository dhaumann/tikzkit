/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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

#include "Document.h"
#include "Node.h"
#include "Edge.h"
#include "Style.h"

#include <QDebug>

namespace tikz {

class DocumentPrivate
{
    public:
        Style style;

        QVector<Style*> customStyles;
};

Document::Document(QObject * parent)
    : QObject(parent)
    , d(new DocumentPrivate())
{
}

Document::~Document()
{
    delete d;
}

Style& Document::style() const
{
    return d->style;
}

int Document::customStyleCount() const
{
    return d->customStyles.count();
}

Style* Document::customStyle(int i) const
{
    Q_ASSERT(i >= 0 && i < d->customStyles.count());
    return d->customStyles[i];
}

}

// kate: indent-width 4; replace-tabs on;
