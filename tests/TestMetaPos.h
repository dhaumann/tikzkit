/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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
#ifndef TEST_META_NODE_H
#define TEST_META_NODE_H

#include <QObject>
#include <QPointF>

class MetaPosTest : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private Q_SLOTS:
    void testMetaPos();
    void testMetaPosWithNode();
    void testSet0();
    void testMetaPosPtr();
    void testToString();
    void testFromString();

public Q_SLOTS:
    void changedEmitted();

private:
    int m_changeCount;
};

#endif // TEST_META_NODE_H

// kate: indent-width 4; replace-tabs on;
