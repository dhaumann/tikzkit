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

#include "Uid.h"

#include <QRegularExpression>
#include <QDebug>

namespace tikz {

QString Uid::toString() const
{
    return QString("%1 (%2)").arg(m_id)
                             .arg(tikz::toString(m_type));
}

Uid Uid::fromString(const QString & str)
{
    // format example: 21 (NodeStyle)
    static QRegularExpression re("([-+]?\\d+)\\s*\\((\\s*\\w+\\s*)\\)");

    QRegularExpressionMatch match = re.match(str);

    if (! match.hasMatch()) {
        Q_ASSERT(false);
        return Uid();
    }

    const QString number = match.captured(1);
    const QString type = match.captured(2);

//     qDebug() << str << "converts to:" << number << type;

    bool ok = false;
    qint64 uid = number.toLongLong(&ok);
    Q_ASSERT(ok);

    return ok ? Uid(uid, tikz::toEntityType(type))
              : Uid();
}

}

namespace QTest {
    // Uid: template specialization for QTest::toString()
    template<>
    char *toString(const tikz::Uid & uid)
    {
        const QString str = "Uid[" + uid.toString() + "]";
        const QByteArray ba = str.toLatin1();
        return qstrdup(ba.data());
    }
}

// kate: indent-width 4; replace-tabs on;
