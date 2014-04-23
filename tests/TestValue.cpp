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
#include "TestValue.h"
#include "TestValue.moc"

#include <QtTest/QTest>

#include <tikz/core/Value.h>

QTEST_MAIN(ValueTest)

void ValueTest::initTestCase()
{
}

void ValueTest::cleanupTestCase()
{
}

void ValueTest::testPoint()
{
    tikz::Value val;

    // test initial value
    QCOMPARE(val.value(), 0.0);
    QCOMPARE(val.unit(), tikz::Unit::Point);
    QCOMPARE(val.toString(), QString("0pt"));

    // test floating point value
    val = 3.567;

    QCOMPARE(val.value(), 3.567);
    QCOMPARE(val.unit(), tikz::Unit::Point);
    QCOMPARE(val.toString(), QString("3.567pt"));

    // operator += and -=
    val = 0;
    val += 3.567;
    QCOMPARE(val.value(), 3.567);
    val -= 3.567;
    QCOMPARE(val.value(), 0.0);

    // test convertTo() identity
    QCOMPARE(val.convertTo(tikz::Unit::Point), val);
}

void ValueTest::testFromString()
{
    tikz::Value val;

    //
    // unit: point
    //
    QCOMPARE(val.unit(), tikz::Unit::Point);

    // test "0pt"
    QCOMPARE(val.value(), tikz::Value::fromString("0pt").value());
    QCOMPARE(val.value(), tikz::Value::fromString("0.pt").value());
    QCOMPARE(val.value(), tikz::Value::fromString("0.0pt").value());
    QCOMPARE(val.value(), tikz::Value::fromString("0.00pt").value());
    QCOMPARE(val.value(), tikz::Value::fromString("0.000pt").value());
    QCOMPARE(val, tikz::Value::fromString("0pt"));

    // test "0 pt" with space
    QCOMPARE(val.value(), tikz::Value::fromString("0 pt").value());
    QCOMPARE(val.value(), tikz::Value::fromString("0. pt").value());
    QCOMPARE(val.value(), tikz::Value::fromString("0.0 pt").value());
    QCOMPARE(val.value(), tikz::Value::fromString("0.00 pt").value());
    QCOMPARE(val.value(), tikz::Value::fromString("0.000 pt").value());
    QCOMPARE(val, tikz::Value::fromString("0 pt"));

    // test static Value::fromString
    val = 3.567;
    QCOMPARE(val.value(), tikz::Value::fromString("3.567pt").value());
    QCOMPARE(val, tikz::Value::fromString("3.567pt"));

    // allow a space after the number
    QCOMPARE(val.value(), tikz::Value::fromString("3.567 pt").value());
    QCOMPARE(val, tikz::Value::fromString("3.567 pt"));

    //
    // unit: Millimeter
    //
    val = tikz::Value(0.0, tikz::Unit::Millimeter);
    QCOMPARE(val.unit(), tikz::Unit::Millimeter);

    // test "0pt"
    QCOMPARE(val.value(), tikz::Value::fromString("0mm").value());
    QCOMPARE(val, tikz::Value::fromString("0mm"));

    // test "0 pt" with space
    QCOMPARE(val.value(), tikz::Value::fromString("0 mm").value());
    QCOMPARE(val, tikz::Value::fromString("0 mm"));

    // test static Value::fromString
    val += 3.567;
    QCOMPARE(val.value(), tikz::Value::fromString("3.567mm").value());
    QCOMPARE(val, tikz::Value::fromString("3.567mm"));

    // allow a space after the number
    QCOMPARE(val.value(), tikz::Value::fromString("3.567 mm").value());
    QCOMPARE(val, tikz::Value::fromString("3.567 mm"));
}

// kate: indent-width 4; replace-tabs on;