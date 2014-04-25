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

#include "Pos.h"

namespace tikz {

QString Pos::toString() const
{
    // we require a valid number
    Q_ASSERT(isValid());

    return QLatin1Char('(') + m_x.toString()
      + QLatin1String(", ") + m_y.toString()
      + QLatin1Char(')');
}

Pos Pos::fromString(const QString & str)
{
    // format example: (12.50cm, -13.5pt)
    
    // 1. find index of '(', ',', and ')'
    const int indexOfOpen = str.indexOf('(');
    const int indexOfComma = str.indexOf(',');
    const int indexOfClose = str.indexOf(')');

    // for now: strict sanity check
    // FIXME: In a releas, this should be more tolerant
    Q_ASSERT(indexOfOpen >= 0);
    Q_ASSERT(indexOfComma >= 0);
    Q_ASSERT(indexOfClose >= 0);
    Q_ASSERT(indexOfComma > indexOfOpen);
    Q_ASSERT(indexOfClose > indexOfOpen);
    Q_ASSERT(indexOfClose > indexOfComma);

    // 2. extract 
    const QString p1 = str.mid(indexOfOpen + 1, indexOfComma - (indexOfOpen + 1)).trimmed();
    const QString p2 = str.mid(indexOfComma + 1, indexOfClose - (indexOfComma + 1)).trimmed();

//     qDebug() << "reading Pos from" << str << "results in:" << p1 << p2;

    return Pos(Value::fromString(p1), Value::fromString(p2));
}

}

namespace QTest {
    // Value: template specialization for QTest::toString()
    template<>
    char *toString(const tikz::Pos & pos)
    {
        const QString str = "Pos[" + pos.convertTo(tikz::Unit::Point).toString() + "]";
        const QByteArray ba = str.toLatin1();
        return qstrdup(ba.data());
    }
}

// kate: indent-width 4; replace-tabs on;
