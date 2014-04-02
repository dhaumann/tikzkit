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

#ifndef TIKZUI_TOOL_LAYOUT_H
#define TIKZUI_TOOL_LAYOUT_H

#include <QRect>
#include <QLayout>
#include <QLayoutItem>

/**
 * The ToolLayout layouts the added widgets in a grid.
 * It places the child widgets such that as many widgets as possible fit into
 * a line. Typical applications are toolboxes of graphical programs like in
 * The Gimp or Krita.
 */
class ToolLayout : public QLayout
{
public:
    /**
     * Constructor with @p parent.
     */
    ToolLayout(QWidget *parent = nullptr);

    /**
     * Virtual destructor.
     */
    virtual ~ToolLayout();

    /**
     * Add @p item to the layout.
     */
    void addItem(QLayoutItem *item) override;

    /**
     * Returns the item with the given @p index.
     */
    QLayoutItem *itemAt(int index) const override;

    /**
     * Remove @p index from the layout. Afterwards, the returned QLayoutItem
     * is not managed by this layout anymore.
     */
    QLayoutItem *takeAt(int index) override;

    /**
     * Returns the number of items managed by this layout.
     */
    int count() const override;

    /**
     * Reimplement to return no expanding directions.
     */
    Qt::Orientations expandingDirections() const override;

    /**
     * Return @e true, since this layout supports heightForWidth().
     */
    bool hasHeightForWidth() const override;

    /**
     * Returns the required height for @p width, taking margins into account.
     */
    int heightForWidth(int width) const override;

    /**
     * Returns the minimum size of the layout, which equals the size of the
     * largest child widget extended by the contents margins.
     */
    QSize minimumSize() const override;

    /**
     * Returns the minimumSize().
     */
    QSize sizeHint() const override;

    /**
     * Relayouts the child widgets in the available @p rect.
     */
    void setGeometry(const QRect &rect) override;

private:
    int doLayout(const QRect &rect, bool dryRun = false) const;

    QList<QLayoutItem *> m_items;
};

#endif // TIKZUI_TOOL_LAYOUT_H

// kate: indent-width 4; replace-tabs on;
