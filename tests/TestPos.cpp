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
#include "TestPos.h"
#include "TestPos.moc"

#include <QtTest/QTest>

#include <tikz/core/Pos.h>

QTEST_MAIN(PosTest)

void PosTest::initTestCase()
{
}

void PosTest::cleanupTestCase()
{
}

void PosTest::testPos()
{
    tikz::Pos pos;

    // test initial value
    QCOMPARE(pos.x(), tikz::Value(0.0));
    QCOMPARE(pos.y(), tikz::Value(0.0));
    QCOMPARE(static_cast<int>(pos.x().unit()), static_cast<int>(tikz::Point));
    QCOMPARE(static_cast<int>(pos.y().unit()), static_cast<int>(tikz::Point));
    QCOMPARE(pos.toString(), QString("(0pt, 0pt)"));
}

void PosTest::testFromString()
{
    tikz::Pos pos;

    // test "0pt"
    QCOMPARE(pos, tikz::Pos::fromString("(0pt, 0pt)"));
    QCOMPARE(pos, tikz::Pos::fromString("(0pt,0pt)"));
    QCOMPARE(pos, tikz::Pos::fromString("(0, 0)"));
    QCOMPARE(pos, tikz::Pos::fromString("(0,0)"));

    pos = tikz::Pos(tikz::Value(3, tikz::Centimeter),
                    tikz::Value(5, tikz::Centimeter));
    QCOMPARE(pos.x(), tikz::Value(3, tikz::Centimeter));
    QCOMPARE(pos.y(), tikz::Value(5, tikz::Centimeter));
    QCOMPARE(pos, tikz::Pos::fromString("(3cm, 5cm)"));
    QCOMPARE(pos, tikz::Pos::fromString("(3cm, 5cm)").convertTo(tikz::Point));
    QCOMPARE(pos, tikz::Pos::fromString("(3cm, 5cm)").convertTo(tikz::Millimeter));
    QCOMPARE(pos, tikz::Pos::fromString("(3cm, 5cm)").convertTo(tikz::Centimeter));
    QCOMPARE(pos, tikz::Pos::fromString("(3cm, 5cm)").convertTo(tikz::Inch));
}

// kate: indent-width 4; replace-tabs on;
