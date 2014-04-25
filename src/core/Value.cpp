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

#include "Value.h"

#include <QRegularExpression>
#include <QDebug>

namespace tikz {

QString Value::toString() const
{
    // we require a valid number
    Q_ASSERT(isValid());

    QString number = QLocale::c().toString(m_value, 'f', 20);

    // TikZ doesn't allow commas as separator
    Q_ASSERT(! number.contains(QLatin1Char(',')));

    // allow only a single '.' as floating point separator
    Q_ASSERT(number.count(QLatin1Char('.')) <= 1);

    // we have 20 digits after the '.', which is usually too much.
    // Therefore, search for '00000' after the '.', and if we find these
    // 5 consecutive zeros, just kill the rest.
    // Example: the number 3.567 turns into 3.56700000000000017053. After
    //          the truncation, it'll be '3.5670000000'
    const int dotIndex = number.indexOf(QLatin1Char('.'));
    if (dotIndex >= 0) {
        const int indexOfZeros = number.lastIndexOf(QLatin1String("00000"));
        if (indexOfZeros > dotIndex) {
            number.truncate(indexOfZeros);
        }
    }

    // beautify step I: remove trailing zeros, e.g. 3.5670000000 -> 3.567
    while (number.length() && number[number.length() - 1] == QLatin1Char('0')) {
        number.truncate(number.length() - 1);
    }
    
    // beautify step II: remove trailing dot, if applicable, e.g. 3. -> 3
    if (number.length() && number[number.length() - 1] == QLatin1Char('.')) {
        number.truncate(number.length() - 1);
    }

    QString suffix;
    switch (m_unit) {
        case Unit::Point: suffix = "pt"; break;
        case Unit::Millimeter: suffix = "mm"; break;
        case Unit::Centimeter: suffix = "cm"; break;
        case Unit::Inch: suffix = "in"; break;
        default: Q_ASSERT(false);
    }

    return number + suffix;
}

Value Value::fromString(const QString & str)
{
    // format example: 12.50cm
    static QRegularExpression re("(-?\\d*\\.?\\d*)\\s*(\\w*)");

    QRegularExpressionMatch match = re.match(str);

    if (! match.hasMatch()) {
        Q_ASSERT(false);
        return Value();
    }

    const QString number = match.captured(1);
    const QString suffix = match.captured(2);

//     qDebug() << str << "converts to:" << number << suffix;

    Unit u;
    if (suffix.isEmpty()) {
        // assume pt
        qDebug() << "no unit given, implicit conversion to pt: " << str;
        u = Unit::Point;
    } else if (suffix == "pt") {
        u = Unit::Point;
    } else if (suffix == "mm") {
        u = Unit::Millimeter;
    } else if (suffix == "cm") {
        u = Unit::Centimeter;
    } else if (suffix == "in") {
        u = Unit::Inch;
    } else {
        Q_ASSERT(false);
    }

    bool ok;
    const double val = number.toDouble(&ok);
    Q_ASSERT(ok);

    return Value(val, u);
}

}

namespace QTest {
    // Value: template specialization for QTest::toString()
    template<>
    char *toString(const tikz::Value & value)
    {
        const QString str = "Value[" + value.convertTo(tikz::Unit::Point).toString() + "]";
        const QByteArray ba = str.toLatin1();
        return qstrdup(ba.data());
    }
}

// kate: indent-width 4; replace-tabs on;
