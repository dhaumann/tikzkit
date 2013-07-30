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
#include "TestCoord.h"
#include "TestCoord.moc"

#include <QtTest/QTest>

#include "Coord.h"

QTEST_MAIN(CoordTest)

void CoordTest::initTestCase()
{
}

void CoordTest::cleanupTestCase()
{
}

void CoordTest::testCoord()
{
    tikz::Coord c;

    // initially (0, 0)
    QCOMPARE(c.pos(), QPointF(0, 0));

    // test changed signal
    connect(&c, SIGNAL(changed(const QPointF & )), this, SLOT(newCoord(const QPointF & )));

    m_pos = QPointF(-1, -1);

    c.setPos(QPointF(1, 1));
    QCOMPARE(c.pos(), QPointF(1, 1));
    QCOMPARE(m_pos, QPointF(1, 1));

    // test operators
    tikz::Coord c2;
    QVERIFY(c != c2);

    c2.setPos(c.pos());
    QVERIFY(c == c2);
}

void CoordTest::newCoord(const QPointF & coord)
{
    m_pos = coord;
}

// kate: indent-width 4; replace-tabs on;
