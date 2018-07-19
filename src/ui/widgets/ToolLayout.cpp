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

#include "ToolLayout.h"

#include <cmath>

#include <QWidget>
#include <QDebug>

ToolLayout::ToolLayout(QWidget *parent)
    : QLayout(parent)
{
    // by default, set margins to 0
    setContentsMargins(0, 0, 0, 0);
}

ToolLayout::~ToolLayout()
{
    qDeleteAll(m_items);
    m_items.clear();
}

void ToolLayout::addItem(QLayoutItem *item)
{
    m_items.append(item);
}

int ToolLayout::count() const
{
    return m_items.size();
}

QLayoutItem *ToolLayout::itemAt(int index) const
{
    return m_items.value(index);
}

QLayoutItem *ToolLayout::takeAt(int index)
{
    if (index < 0 || index >= m_items.size()) {
        return nullptr;
    }

    return m_items.takeAt(index);
}

Qt::Orientations ToolLayout::expandingDirections() const
{
    return nullptr;
}

bool ToolLayout::hasHeightForWidth() const
{
    return true;
}

int ToolLayout::heightForWidth(int width) const
{
    return doLayout(QRect(0, 0, width, 0), true);
}

// find size containing all widgets
static QSize sizeForItems(const QList<QLayoutItem *> items)
{
    QSize size;
    foreach (QLayoutItem * item, items) {
        size = size.expandedTo(item->minimumSize());
    }
    return size;
}

QSize ToolLayout::minimumSize() const
{
    // in case the user set margins
    return sizeForItems(m_items) + QSize(2 * margin(), 2 * margin());
}

QSize ToolLayout::sizeHint() const
{
    return minimumSize();
}

void ToolLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect);
}

int ToolLayout::doLayout(const QRect &rect, bool dryRun) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    const QRect contentsRect = rect.adjusted(left, top, -right, -bottom);
    const QSize itemSize = sizeForItems(m_items);
    const int itemsPerLine = qMax(1, contentsRect.width() / itemSize.width());
    const qreal itemWidth = static_cast<qreal>(contentsRect.width()) / itemsPerLine;

    if (!dryRun) {
        qreal x = contentsRect.left();
        int y = contentsRect.top();
        int i = 0;
        while (i < m_items.size()) {

            m_items[i]->widget()->setGeometry(QRect(QPoint(x, y), QSize(itemWidth, itemSize.height())));
            x += itemWidth;

            ++i;

            if (i % itemsPerLine == 0) {
                y += itemSize.height();
                x = contentsRect.left();
            }
        }
    }

    if (m_items.size()) {
        return top + bottom + itemSize.height() * std::ceil(static_cast<qreal>(m_items.size()) / itemsPerLine);
    }
    return sizeHint().height();
}

// kate: indent-width 4; replace-tabs on;
