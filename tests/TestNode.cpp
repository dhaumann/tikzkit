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
#include "TestNode.h"
#include "TestNode.moc"

#include <QtTest/QTest>

#include <tikz/core/Document.h>
#include <tikz/core/Node.h>

QTEST_MAIN(NodeTest)

void NodeTest::initTestCase()
{
}

void NodeTest::cleanupTestCase()
{
}

void NodeTest::testPos()
{
    tikz::core::Document doc;
    tikz::core::Node * n = doc.createNode();

    // initially (0, 0)
    QCOMPARE(n->pos(), tikz::Pos(0, 0));

    n->setPos(tikz::Pos(1, 1));
    QCOMPARE(n->pos(), tikz::Pos(1, 1));
}

// kate: indent-width 4; replace-tabs on;
