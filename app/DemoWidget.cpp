/* This file is part of the TikZKit project
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */
#include "DemoWidget.h"

#include "TikzDocument.h"
#include "TikzNode.h"
#include "TikzEdge.h"
#include "TikzScene.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"
#include "TikzToolBox.h"
#include "tikz.h"
#include <Edge.h>

#include <ArrowComboBox.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QSlider>
#include <QTimer>
#include <QtOpenGL/QGLWidget>
#include <QUndoStack>
#include <QToolButton>

#include <QDebug>

static TikzNode* a;

DemoWidget::DemoWidget()
    : QWidget()
{
    TikzDocument * doc = new TikzDocument(this);

    QHBoxLayout * hbox = new QHBoxLayout(this);
    setLayout(hbox);

    TikzToolBox * toolBox = new TikzToolBox(doc, this);
    hbox->addWidget(toolBox);

    QVBoxLayout* v = new QVBoxLayout();
    hbox->addLayout(v);

    QHBoxLayout* h = new QHBoxLayout();
    v->addLayout(h);

    QToolButton * undoButton = new QToolButton(this);
    undoButton->setText("undo");
    connect(undoButton, SIGNAL(clicked()), doc->undoManager(), SLOT(undo()));
    QToolButton * redoButton = new QToolButton(this);
    redoButton->setText("redo");
    connect(redoButton, SIGNAL(clicked()), doc->undoManager(), SLOT(redo()));

    h->addWidget(undoButton);
    h->addWidget(redoButton);

    // add arrow head/tail combos
    ArrowComboBox * arrowTailCombo = new ArrowComboBox(false, this);
    ArrowComboBox * arrowHeadCombo = new ArrowComboBox(true, this);
    h->addWidget(arrowTailCombo);
    h->addWidget(arrowHeadCombo);
    h->addStretch();
    arrowTailCombo->show();
    arrowHeadCombo->show();

    QHBoxLayout* l = new QHBoxLayout();
    v->addLayout(l);


    m_view = doc->createView(this);
    l->addWidget(m_view);

    m_zoomSlider = new QSlider(this);
    m_zoomSlider->setRange(100, 1000);
    l->addWidget(m_zoomSlider);

    m_rotSlider = new QSlider(this);
    m_rotSlider->setRange(0, 360);
    l->addWidget(m_rotSlider);

    connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransform()));
    connect(m_rotSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransform()));

//     m_view->setSceneRect(0, 0, m_view->size().width() / 200.0, m_view->size().height() / 200.0);

    m_view->show();

    TikzNode* item1 = doc->createTikzNode();
    doc->setNodePos(item1->node(), QPointF(-3, 3));
    item1->node()->style()->setLineWidthType(tikz::VeryThin);
    item1->node()->style()->setShape(tikz::ShapeCircle);
    item1->node()->style()->setInnerSep(0.2);
    doc->setNodeText(item1->node(), "$\\int f(x) dx$");

    TikzNode* item2 = doc->createTikzNode();
    doc->setNodePos(item2->node(), QPointF(3, 3));
    item2->node()->style()->setLineWidthType(tikz::Thin);
    item2->node()->style()->setShape(tikz::ShapeCircle);
    item2->node()->style()->setInnerSep(0.2);
    doc->setNodeText(item2->node(), "$\\Leftrightarrow$");

    // an edge
    TikzEdge* edge = doc->createTikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge()->style()->setLineWidthType(tikz::SemiThick);



    item1 = doc->createTikzNode();
    doc->setNodePos(item1->node(), QPointF(-3, 1));
    item1->node()->style()->setLineWidthType(tikz::Thin);
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(0.2);
    doc->setNodeText(item1->node(), "$a$");

    item2 = doc->createTikzNode();
    doc->setNodePos(item2->node(), QPointF(3, 1));
    item2->node()->style()->setLineWidthType(tikz::SemiThick);
    item2->node()->style()->setShape(tikz::ShapeCircle);
    item2->node()->style()->setInnerSep(0.2);
    doc->setNodeText(item2->node(), "a");

    // an edge
    edge = doc->createTikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge()->style()->setLineWidthType(tikz::SemiThick);


    item1 = doc->createTikzNode();
    doc->setNodePos(item1->node(), QPointF(-3, -1));
    item1->node()->style()->setLineWidthType(tikz::VeryThick);
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(0.2);
    doc->setNodeText(item1->node(), "$\\iiint \\max$");

    item2 = doc->createTikzNode();
    doc->setNodePos(item2->node(), QPointF(3, -1));
    item2->node()->style()->setLineWidthType(tikz::UltraThick);
    item2->node()->style()->setShape(tikz::ShapeRectangle);
    item2->node()->style()->setInnerSep(0.2);
    doc->setNodeText(item2->node(), "A long text\\\\which is wrapped");

    item2->style()->setParentStyle(item1->style());

    // an edge
    edge = doc->createTikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge()->style()->setLineWidthType(tikz::UltraThick);
    edge->edge()->style()->setArrowTail(tikz::LatexArrow);
    edge->edge()->style()->setArrowHead(tikz::PipeArrow);


    item1 = doc->createTikzNode();
    doc->setNodePos(item1->node(), QPointF(-3, -3));
    item1->node()->style()->setLineWidthType(tikz::VeryThick);
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(0.2);
    doc->setNodeText(item1->node(), "c");

    a = item1;

    item2 = doc->createTikzNode();
    doc->setNodePos(item2->node(), QPointF(3, -3));
    item2->node()->style()->setDoubleLine(true);
    item2->node()->style()->setLineWidthType(tikz::SemiThick);
    item2->node()->style()->setShape(tikz::ShapeEllipse);
    item2->node()->style()->setInnerSep(0.2);
    item2->node()->style()->setMinimumWidth(2);
    doc->setNodeText(item2->node(), "8");

    // an edge
    edge = doc->createTikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge()->style()->setLineWidthType(tikz::UltraThick);
    edge->edge()->style()->setBendAngle(30);
    edge->edge()->style()->setArrowTail(tikz::StealthArrow);
    edge->edge()->style()->setArrowHead(tikz::ToArrow);


    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        edge = doc->createTikzEdge();
        edge->edge()->setStartPos(QPointF(-6, i - 4));
        edge->edge()->setEndPos(QPointF(-4, i - 4));
        edge->edge()->style()->beginConfig();
        edge->edge()->style()->setLineWidthType(tikz::UltraThick);
        edge->edge()->style()->setArrowTail(tikz::Arrow(i));
        edge->edge()->style()->setArrowHead(tikz::Arrow(i));
        edge->edge()->style()->endConfig();
    }

    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        edge = doc->createTikzEdge();
        edge->edge()->setStartPos(QPointF(4, i - 4));
        edge->edge()->setEndPos(QPointF(6, i - 4));
        edge->edge()->style()->beginConfig();
        edge->edge()->style()->setDoubleLine(true);
        edge->edge()->style()->setInnerLineWidth(tikz::VeryThick);
        edge->edge()->style()->setLineWidthType(tikz::UltraThick);
        edge->edge()->style()->setArrowTail(tikz::Arrow(i));
        edge->edge()->style()->setArrowHead(tikz::Arrow(i));
        edge->edge()->style()->endConfig();
    }

//     QTimer::singleShot(3000, item1, SLOT(deleteLater()));
}

DemoWidget::~DemoWidget()
{
}

void DemoWidget::updateTransform()
{
    qreal rot = m_rotSlider->value();
    qreal scale = m_zoomSlider->value() / 10.0;

    a->node()->style()->setRotation(rot);

    QTransform trans;
//     trans.rotate(rot);
    trans.scale(scale, -scale);

    m_view->setTransform(trans);
}

// kate: indent-width 4; replace-tabs on;
