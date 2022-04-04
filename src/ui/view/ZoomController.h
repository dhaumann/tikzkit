/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2016 Dominik Haumann <dhaumann@kde.org>
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
#ifndef TIKZUI_ZOOM_CONTROLLER_H
#define TIKZUI_ZOOM_CONTROLLER_H

#include "tikzui_export.h"

#include <QObject>
#include <QPointer>
#include <QRegExp>
#include <QVector>

class QComboBox;
class QPinchGesture;
class QRegExpValidator;
class QTimeLine;

namespace tikz {
namespace ui {

/**
 * Helper class for zooming.
 *
 * This class implements all logic needed to control zooming. Predefined zoom
 * factors make sure that the set zooming levels are sane (and human radable).
 *
 * A combo box can be attached by calling attachToComboBox(). The attached
 * combo box is automatically filled with the defined zoom factors.
 */
class TIKZKITUI_EXPORT ZoomController : public QObject
{
    Q_OBJECT

public:
    ZoomController(QObject *parent = nullptr);
    virtual ~ZoomController();

    void setZoom(qreal z);
    qreal currentZoom() const;

    void setZoomFactors(const QVector<qreal>& factors);
    void attachToComboBox(QComboBox *comboBox);

    bool canZoomIn() const;
    bool canZoomOut() const;

    /**
     * Changes the current zoom level depending on the given @p delta.
     * A high precision delta is supported, for fine-resolution mice.
     */
    void processWheelEvent(int delta);

public Q_SLOTS:
    void zoomIn(bool animated = false);
    void zoomOut(bool animated = false);
    void resetZoom();

private Q_SLOTS:
    void syncComboBox();
    void comboBoxActivated(int index);
    void comboBoxEdited();
    void setZoomAnimated(qreal value);

Q_SIGNALS:
    void zoomChanged(qreal currentZoom);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void startZoomAnimation(qreal startZoom, qreal endZoom);

private:
    QTimeLine *m_animationTimeLine = nullptr;
    qreal m_startZoom = 1.0;
    qreal m_endZoom = 1.0;
    qreal m_currentZoom = 1.0;
    qreal m_gestureStartScale = 0.0;
    QVector<qreal> m_zoomFactors;
    QPointer<QComboBox> m_comboBox;
    QRegExp m_inputRegExp;
    QRegExpValidator *m_inputValidator = nullptr;
};

}
}

#endif // TIKZUI_ZOOM_CONTROLLER_H
