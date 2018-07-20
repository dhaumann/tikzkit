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

#include <QtTest/QTest>

#include <tikz/core/Value.h>

QTEST_MAIN(ValueTest)

void ValueTest::initTestCase()
{
}

void ValueTest::cleanupTestCase()
{
}

void ValueTest::testNaN()
{
    tikz::Value val = tikz::Value::invalid();

    // toString
    QCOMPARE(val.toString(), QString("nan"));
    QCOMPARE(val, tikz::Value::invalid());

    // fromString
    val = tikz::Value::fromString("nan");
    QCOMPARE(val, tikz::Value::invalid());
}

void ValueTest::testPoint()
{
    tikz::Value val;

    // test initial value
    QCOMPARE(val.value(), 0.0);
    QCOMPARE(tikz::toString(val.unit()), tikz::toString(tikz::Unit::Point));
    QCOMPARE(val.toString(), QString("0pt"));

    // test floating point value
    val = 3.567;

    QCOMPARE(val.value(), 3.567);
    QCOMPARE(tikz::toString(val.unit()), tikz::toString(tikz::Unit::Point));
    QCOMPARE(val.toString(), QString("3.567pt"));

    // operator += and -=
    val = 0;
    val += 3.567;
    QCOMPARE(val.value(), 3.567);
    val -= 3.567;
    QCOMPARE(val.value(), 0.0);

    // test convertTo() identity
    QCOMPARE(val.convertTo(tikz::Unit::Point), val);

    // test Value::invalid()
    val = tikz::Value::invalid();
    QVERIFY(! val.isValid());
}

void ValueTest::testFromString()
{
    tikz::Value val;

    //
    // unit: point
    //
    QCOMPARE(tikz::toString(val.unit()), tikz::toString(tikz::Unit::Point));

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
    QCOMPARE(tikz::toString(val.unit()), tikz::toString(tikz::Unit::Millimeter));

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

void ValueTest::testStringAccuracy()
{
    // test numbers from 0.01 to 99.99
    for (int i = 0; i < 100; ++i) {
        for (int j = 1; j < 100; ++j) {
            if (j % 10 == 0) continue;
            QCOMPARE(tikz::Value(i + j * 0.01).toString(), QString("%1.%2pt").arg(i).arg(j, 2, 10, QLatin1Char('0')));
        }
    }

    // test negative numbers from -0.01 to -99.99
    for (int i = 0; i < 100; ++i) {
        for (int j = 1; j < 100; ++j) {
            if (j % 10 == 0) continue;
            QCOMPARE(tikz::Value(-i - j * 0.01).toString(), QString("-%1.%2pt").arg(i).arg(j, 2, 10, QLatin1Char('0')));
        }
    }
}

void ValueTest::testLiteralOperators()
{
    QCOMPARE(1.01_pt, tikz::Value(1.01, tikz::Unit::Point));
    QCOMPARE(2.34_mm, tikz::Value(2.34, tikz::Unit::Millimeter));
    QCOMPARE(5.67_cm, tikz::Value(5.67, tikz::Unit::Centimeter));
    QCOMPARE(8.99_in, tikz::Value(8.99, tikz::Unit::Inch));
}

// kate: indent-width 4; replace-tabs on;
