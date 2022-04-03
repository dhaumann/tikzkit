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
#include "ZoomController.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPinchGesture>
#include <QTimeLine>
#include <QValidator>
#include <QWheelEvent>

#include <cmath>

namespace {
    QString zoomToString(qreal zoom)
    {
        const qreal z = qRound(zoom * 10000.0) / 100.0;
        return QString(QLatin1String("%1 %")).arg(z);
    }
}

namespace tikz {
namespace ui {

ZoomController::ZoomController(QObject *parent)
    : QObject(parent)
    // NOTE: add pre-defined zoom entries here:
    , m_zoomFactors({
        5.0,
        4.0,
        3.0,
        2.0,
        1.5,
        1.0,
        0.75,
        0.5,
        0.33,
        0.25,
        0.10})
    , m_inputRegExp(QLatin1String("^\\s*((\\d+)(\\.\\d+)?)\\s*%?\\s*$"))
{
    // zoom animation should take 100ms
    m_animationTimeLine = new QTimeLine(100, this);
    connect(m_animationTimeLine, &QTimeLine::valueChanged, this, &ZoomController::setZoomAnimated);
    connect(m_animationTimeLine, &QTimeLine::finished, this, &ZoomController::syncComboBox);
}

ZoomController::~ZoomController()
{
}

void ZoomController::setZoom(qreal z)
{
    if (z == m_currentZoom) {
        return;
    }

    m_currentZoom = z;

    syncComboBox();

    emit zoomChanged(m_currentZoom);
}

qreal ZoomController::currentZoom() const
{
    return m_currentZoom;
}

bool ZoomController::canZoomIn() const
{
    return m_currentZoom < m_zoomFactors.first();
}

bool ZoomController::canZoomOut() const
{
    return m_currentZoom > m_zoomFactors.back();
}

void ZoomController::processWheelEvent(int delta)
{
    if (delta < 0) {
        zoomOut(true);
    } else if (delta >= 0) {
        zoomIn(true);
    }
}

void ZoomController::zoomIn(bool animated)
{
    if (m_animationTimeLine->state() == QTimeLine::Running) {
        return;
    }

    for (int i = m_zoomFactors.count() - 1; i >= 0; --i) {
        if (m_zoomFactors[i] > m_currentZoom) {
            if (animated) {
                startZoomAnimation(m_currentZoom, m_zoomFactors[i]);
            } else {
                setZoom(m_zoomFactors[i]);
            }
            break;
        }
    }
}

void ZoomController::zoomOut(bool animated)
{
    if (m_animationTimeLine->state() == QTimeLine::Running) {
        return;
    }

    for (qreal z : m_zoomFactors) {
        if (z < m_currentZoom) {
            if (animated) {
                startZoomAnimation(m_currentZoom, z);
            } else {
                setZoom(z);
            }
            break;
        }
    }
}

void ZoomController::startZoomAnimation(qreal startZoom, qreal endZoom)
{
    if (m_animationTimeLine->state() == QTimeLine::Running) {
        return;
    }

    m_startZoom = startZoom;
    m_endZoom = endZoom;
    m_animationTimeLine->start();
}

void ZoomController::setZoomAnimated(qreal value)
{
    setZoom((1.0 - value) * m_startZoom + value * m_endZoom);
}

void ZoomController::resetZoom()
{
    setZoom(1);
}

void ZoomController::setZoomFactors(const QVector<qreal>& factors)
{
    m_zoomFactors = factors;
}

void ZoomController::attachToComboBox(QComboBox *comboBox)
{
    if (m_comboBox == comboBox) {
        return;
    }

    if (m_comboBox) {
        m_comboBox->removeEventFilter(this);
        m_comboBox->disconnect(this);
        if (m_comboBox->lineEdit()) {
            m_comboBox->lineEdit()->disconnect(this);
        }
        m_comboBox->setValidator(nullptr);
    }

    m_comboBox = comboBox;

    if (m_comboBox) {
        m_comboBox->clear();
        for (qreal z : m_zoomFactors) {
            m_comboBox->addItem(zoomToString(z), z);
        }
        syncComboBox();
        connect(m_comboBox, SIGNAL(activated(int)),
                this, SLOT(comboBoxActivated(int)));

        m_comboBox->setEditable(true);
        m_comboBox->setInsertPolicy(QComboBox::NoInsert);
        connect(m_comboBox->lineEdit(), SIGNAL(editingFinished()),
                this, SLOT(comboBoxEdited()));

        if (!m_inputValidator) {
            m_inputValidator = new QRegExpValidator(m_inputRegExp, this);
        }
        m_comboBox->setValidator(m_inputValidator);
        m_comboBox->installEventFilter(this);
    }
}

void ZoomController::comboBoxActivated(int index)
{
    startZoomAnimation(m_currentZoom, m_comboBox->itemData(index).toReal());
}

void ZoomController::comboBoxEdited()
{
    if (m_animationTimeLine->state() == QTimeLine::Running) {
        return;
    }

    const int index = m_inputRegExp.indexIn(m_comboBox->currentText());
    Q_ASSERT(index >= 0); // validator ensures this

    if (index >= 0) {
        const qreal z = qBound(m_zoomFactors.back(),
                               qreal(m_inputRegExp.cap(1).toDouble() / 100.0),
                               m_zoomFactors.first());

        setZoom(z);
    }
}

void ZoomController::syncComboBox()
{
    if (!m_comboBox || m_animationTimeLine->state() == QTimeLine::Running) {
        return;
    }

    // Set current index to -1 for custom zooms
    const int index = m_comboBox->findData(m_currentZoom);
    m_comboBox->setCurrentIndex(index);
    m_comboBox->setEditText(zoomToString(m_currentZoom));
}

bool ZoomController::eventFilter(QObject *watched, QEvent *event)
{
    // filter correct watched object
    if (!m_comboBox || m_comboBox != watched) {
        return QObject::eventFilter(watched, event);
    }

    // we only want wheel events
    if (event->type() != QEvent::Wheel) {
        return QObject::eventFilter(watched, event);
    }

    // if currentIndex() is != -1, then scrolling works as expected
    if (m_comboBox->currentIndex() != -1) {
        return QObject::eventFilter(watched, event);
    }

    // we want vertical wheel events
    auto evWheel = static_cast<QWheelEvent*>(event);
    if (!evWheel || evWheel->angleDelta().y() > 0) {
        return QObject::eventFilter(watched, event);
    }

    // zoom in/out
    if (evWheel->angleDelta().y() < 0) {
        zoomOut(true);
    } else if (evWheel->angleDelta().y() > 0) {
        zoomIn(true);
    }

    return true;
}

}
}