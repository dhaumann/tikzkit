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

#include "DocumentPrivate.h"


#include <tikz/core/Node.h>
#include <tikz/core/Path.h>
#include <tikz/core/Style.h>
#include <tikz/core/Document.h>

//BEGIN DEBUG
#include <tikz/core/NodeStyle.h>
#include <tikz/core/EdgeStyle.h>
#include <tikz/core/EdgePath.h>
#include <tikz/core/EllipsePath.h>
//END DEBUG

#include "EditorPrivate.h"
#include "ViewPrivate.h"
#include "Renderer.h"

#include "NodeItem.h"
#include "PathItem.h"
#include "EllipsePathItem.h"
#include "EdgePathItem.h"
#include "TikzScene.h"

#include <QDebug>
#include <QGraphicsView>

namespace tikz {
namespace ui {

DocumentPrivate::DocumentPrivate(QObject * parent)
    : tikz::ui::Document(this, parent)
{
    // register document
    tikz::ui::EditorPrivate::self()->registerDocument(this);

    // create scene
    m_scene = new TikzScene(this);

    connect(m_scene, SIGNAL(editModeChanged(TikzEditMode)), this, SIGNAL(editModeChanged(TikzEditMode)));
    connect(this, SIGNAL(aboutToClear()), this, SLOT(clearDocumentPrivate()));
}

DocumentPrivate::~DocumentPrivate()
{
    // purge all Nodes and Paths
    clearDocumentPrivate();

    // make sure they are gone
    Q_ASSERT(m_nodeMap.isEmpty());
    Q_ASSERT(m_nodes.isEmpty());
    Q_ASSERT(m_pathMap.isEmpty());
    Q_ASSERT(m_paths.isEmpty());

    // finally unregister document
    tikz::ui::EditorPrivate::self()->unregisterDocument(this);
}

void DocumentPrivate::clearDocumentPrivate()
{
    // free UI part of nodes and paths
    qDeleteAll(m_paths);
    m_pathMap.clear();
    m_paths.clear();

    qDeleteAll(m_nodes);
    m_nodeMap.clear();
    m_nodes.clear();
}

void DocumentPrivate::setEditMode(TikzEditMode mode)
{
    m_scene->setEditMode(mode);
}

TikzEditMode DocumentPrivate::editMode() const
{
    return m_scene->editMode();
}

tikz::Pos DocumentPrivate::scenePos(const tikz::core::MetaPos & pos) const
{
    const auto node = pos.node();
    if (node) {
        const NodeItem * nodeItem = nodeItemFromId(node->id());
        Q_ASSERT(nodeItem != nullptr);
        return nodeItem->anchor(pos.anchor());
    }

    return Document::scenePos(pos);
}

TikzScene * DocumentPrivate::scene() const
{
    return m_scene;
}

View * DocumentPrivate::createView(QWidget * parent,
                                   tikz::ui::MainWindow * mainWindow)
{
    // create view
    auto view = new ViewPrivate(this, parent, mainWindow);
    Q_ASSERT(m_views.contains(view));

//BEGIN DEBUG
    beginTransaction("Create Nodes");
    tikz::ui::NodeItem* item1 = createNodeItem();
    item1->node()->setPos(tikz::Pos(-3, 3, tikz::Centimeter));
    tikz::core::NodeStyle ns;
    ns.setStyle(item1->node()->style());
    ns.setLineWidth(tikz::Value::veryThin());
    ns.setShape(tikz::ShapeRectangle);
    ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
    item1->node()->setStyle(ns);
    item1->node()->setText("$\\int f(x) dx$");

    tikz::ui::NodeItem* item2 = createNodeItem();
    item2->node()->setPos(tikz::Pos(3, 3, tikz::Centimeter));
    ns.setStyle(item2->node()->style());
    ns.setLineWidth(tikz::Value::thin());
    ns.setShape(tikz::ShapeCircle);
    ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
    item2->node()->setStyle(ns);
    item2->node()->setText("$\\Leftrightarrow$");
    finishTransaction();

    beginTransaction("Create Paths");
    // an ellipse path
    tikz::ui::PathItem* path = createPathItem(tikz::core::Path::Ellipse);
    auto ellipse = qobject_cast<tikz::core::EllipsePath*>(path->path());
    tikz::core::EdgeStyle es;
    es.setStyle(ellipse->style());
    es.setRadiusX(tikz::Value(2.0, tikz::Centimeter));
    es.setRadiusY(tikz::Value(1.0, tikz::Centimeter));
    es.setLineWidth(tikz::Value::semiThick());
    ellipse->setStyle(es);

    // add a line path
    path = createPathItem(tikz::core::Path::Line);
    auto edge = qobject_cast<tikz::core::EdgePath*>(path->path());
    es.setStyle(edge->style());
    es.setLineWidth(tikz::Value::semiThick());
//     es.setDoubleLine(true);
    es.setArrowTail(tikz::LatexArrow);
    es.setArrowHead(tikz::ToArrow);
    edge->setStyle(es);
    edge->setStartNode(item1->node());
    edge->setEndNode(item2->node());
    finishTransaction();

#if 0
    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, 1, tikz::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::thin());
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item1->node()->setText("$a$");

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, 1, tikz::Centimeter));
    item2->node()->style()->setLineWidth(tikz::Value::semiThick());
    item2->node()->style()->setShape(tikz::ShapeCircle);
    item2->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item2->node()->setText("a");

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::semiThick());


    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, -1, tikz::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::veryThick());
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item1->node()->setText("$\\iiint \\max$");

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, -1, tikz::Centimeter));
    item2->node()->style()->setLineWidth(tikz::Value::ultraThick());
    item2->node()->style()->setShape(tikz::ShapeRectangle);
    item2->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item2->node()->setText("A long text\\\\which is wrapped");

//     item2->style()->setParentStyle(item1->style());

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::ultraThick());
    path->path()->style()->setArrowTail(tikz::LatexArrow);
    path->path()->style()->setArrowHead(tikz::PipeArrow);


    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, -3, tikz::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::veryThick());
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item1->node()->setText("c");

    a = item1;

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, -3, tikz::Centimeter));
    item2->node()->style()->setDoubleLine(true);
    item2->node()->style()->setLineWidth(tikz::Value::semiThick());
    item2->node()->style()->setShape(tikz::ShapeEllipse);
    item2->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item2->node()->style()->setMinimumWidth(2);
    item2->node()->setText("8");

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::ultraThick());
    path->path()->style()->setBendAngle(30);
//     path->path()->style()->setCurveMode(tikz::HVLineTo);
    path->path()->style()->setArrowTail(tikz::StealthArrow);
    path->path()->style()->setArrowHead(tikz::ToArrow);


    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        path = m_doc->createPathItem();
//         path->path()->setStartPos(QPointF(-6, i - 4));
//         path->path()->setEndPos(QPointF(-4, i - 4));
        path->path()->style()->beginConfig();
        path->path()->style()->setLineWidth(tikz::Value::ultraThick());
        path->path()->style()->setArrowTail(tikz::Arrow(i));
        path->path()->style()->setArrowHead(tikz::Arrow(i));
        path->path()->style()->endConfig();
    }

    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        path = m_doc->createPathItem();
//         path->path()->setStartPos(QPointF(4, i - 4));
//         path->path()->setEndPos(QPointF(6, i - 4));
        path->path()->style()->beginConfig();
        path->path()->style()->setDoubleLine(true);
        path->path()->style()->setInnerLineWidth(tikz::Value::veryThick());
        path->path()->style()->setLineWidth(tikz::UltraThick);
        path->path()->style()->setArrowTail(tikz::Arrow(i));
        path->path()->style()->setArrowHead(tikz::Arrow(i));
        path->path()->style()->endConfig();
    }
#endif

    // test example
    {
        beginTransaction("Create Diagram");

        tikz::ui::NodeItem *n1 = createNodeItem();
        n1->node()->setPos(tikz::Pos(0, 6, tikz::Centimeter));
        ns.setStyle(n1->node()->style());
        ns.setShape(tikz::ShapeRectangle);
        ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
        ns.setMinimumWidth(tikz::Value(2, tikz::Centimeter));
        ns.setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        ns.setPenColor(QColor(0, 0, 204));
        ns.setFillColor(QColor(204, 204, 255));
        n1->node()->setStyle(ns);
        n1->node()->setText("Kate Part\\\\(Backend library)");

        tikz::ui::NodeItem *n2 = createNodeItem();
        n2->node()->setPos(tikz::Pos(0, 8, tikz::Centimeter));
        ns.setStyle(n2->node()->style());
        ns.setShape(tikz::ShapeRectangle);
        ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
        ns.setMinimumWidth(tikz::Value(2, tikz::Centimeter));
        ns.setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        ns.setPenColor(QColor(255, 153, 51));
        ns.setFillColor(QColor(255, 230, 204));
        n2->node()->setStyle(ns);
        n2->node()->setText("KTextEditor\\\\(Interfaces)");

        tikz::ui::NodeItem *n3 = createNodeItem();
        n3->node()->setPos(tikz::Pos(-5.8, 10, tikz::Centimeter));
        ns.setStyle(n3->node()->style());
        ns.setShape(tikz::ShapeRectangle);
        ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
        ns.setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        ns.setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        ns.setPenColor(QColor(255, 51, 51));
        ns.setFillColor(QColor(255, 204, 204));
        n3->node()->setStyle(ns);
        n3->node()->setText("Kate\\\\(Application)");

        tikz::ui::NodeItem *n4 = createNodeItem();
        n4->node()->setPos(tikz::Pos(-2.9, 10, tikz::Centimeter));
        ns.setStyle(n4->node()->style());
        ns.setShape(tikz::ShapeRectangle);
        ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
        ns.setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        ns.setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        ns.setPenColor(QColor(255, 51, 51));
        ns.setFillColor(QColor(255, 204, 204));
        n4->node()->setStyle(ns);
        n4->node()->setText("KWrite\\\\(Application)");

        tikz::ui::NodeItem *n5 = createNodeItem();
        n5->node()->setPos(tikz::Pos(0, 10, tikz::Centimeter));
        ns.setStyle(n5->node()->style());
        ns.setShape(tikz::ShapeRectangle);
        ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
        ns.setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        ns.setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        ns.setPenColor(QColor(255, 51, 51));
        ns.setFillColor(QColor(255, 204, 204));
        n5->node()->setStyle(ns);
        n5->node()->setText("KDevelop\\\\(Application)");

        tikz::ui::NodeItem *n6 = createNodeItem();
        n6->node()->setPos(tikz::Pos(2.9, 10, tikz::Centimeter));
        ns.setStyle(n5->node()->style());
        ns.setShape(tikz::ShapeRectangle);
        ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
        ns.setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        ns.setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        ns.setPenColor(QColor(255, 51, 51));
        ns.setFillColor(QColor(255, 204, 204));
        n6->node()->setStyle(ns);
        n6->node()->setText("Kile\\\\(Application)");

        tikz::ui::NodeItem *n7 = createNodeItem();
        n7->node()->setPos(tikz::Pos(5.8, 10, tikz::Centimeter));
        ns.setStyle(n7->node()->style());
        ns.setShape(tikz::ShapeRectangle);
        ns.setInnerSep(tikz::Value(2, tikz::Millimeter));
        ns.setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        ns.setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        ns.setPenColor(QColor(255, 51, 51));
        ns.setFillColor(QColor(255, 204, 204));
        n7->node()->setStyle(ns);
        n7->node()->setText("3rd party\\\\(Application)");

        //
        // paths
        //
        tikz::ui::PathItem* path = createPathItem();
        auto edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n1->node());
        edge->setEndNode(n2->node());
        es.setStyle(path->path()->style());
        es.setArrowTail(tikz::LatexArrow);
        es.setArrowHead(tikz::LatexArrow);
        es.setPenColor(QColor(128, 128, 128));
        edge->setStyle(es);

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n3->node());
        es.setStyle(edge->style());
        es.setArrowTail(tikz::LatexArrow);
        es.setArrowHead(tikz::LatexArrow);
        es.setPenColor(QColor(128, 128, 128));
        edge->setStyle(es);
        edge->setStartAnchor(tikz::West);

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n4->node());
        es.setStyle(edge->style());
        es.setArrowTail(tikz::LatexArrow);
        es.setArrowHead(tikz::LatexArrow);
        es.setPenColor(QColor(128, 128, 128));
        edge->setStyle(es);

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n5->node());
        es.setStyle(edge->style());
        es.setArrowTail(tikz::LatexArrow);
        es.setArrowHead(tikz::LatexArrow);
        es.setPenColor(QColor(128, 128, 128));
        edge->setStyle(es);

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n6->node());
        es.setStyle(edge->style());
        es.setArrowTail(tikz::LatexArrow);
        es.setArrowHead(tikz::LatexArrow);
        es.setPenColor(QColor(128, 128, 128));
        edge->setStyle(es);

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n7->node());
        es.setStyle(edge->style());
        es.setArrowTail(tikz::LatexArrow);
        es.setArrowHead(tikz::LatexArrow);
        es.setPenColor(QColor(128, 128, 128));
        edge->setStyle(es);
        edge->setStartAnchor(tikz::East);

        finishTransaction();
    }
//END DEBUG

    // return view
    return view;
}

QVector<View *> DocumentPrivate::views() const
{
    return m_views;
}

void DocumentPrivate::registerView(tikz::ui::View * view)
{
    Q_ASSERT(! m_views.contains(view));
    m_views.append(view);
}

void DocumentPrivate::unregisterView(tikz::ui::View * view)
{
    Q_ASSERT(m_views.contains(view));
    m_views.remove(m_views.indexOf(view));
}

QVector<NodeItem*> DocumentPrivate::nodeItems() const
{
    return m_nodes;
}

QVector<PathItem*> DocumentPrivate::pathItems() const
{
    return m_paths;
}

NodeItem * DocumentPrivate::createNodeItem()
{
    // create node
    tikz::core::Node * node = Document::createNode();
    Q_ASSERT(m_nodeMap.contains(node->id()));

    return m_nodeMap[node->id()];
}

tikz::ui::PathItem * DocumentPrivate::createPathItem(tikz::core::Path::Type type)
{
    // create path
    tikz::core::Path * path = Document::createPath(type);
    Q_ASSERT(m_pathMap.contains(path->id()));

    return m_pathMap[path->id()];
}

void DocumentPrivate::deleteNodeItem(NodeItem * node)
{
    // delete node from id
    const int id = node->id();
    Q_ASSERT(m_nodeMap.contains(id));
    Document::deleteNode(node->node());
    Q_ASSERT(! m_nodeMap.contains(id));
}

void DocumentPrivate::deletePathItem(tikz::ui::PathItem * path)
{
    // delete path from id
    const int id = path->id();
    Q_ASSERT(m_pathMap.contains(id));
    Document::deletePath(path->path());
    Q_ASSERT(! m_pathMap.contains(id));
}

tikz::core::Node * DocumentPrivate::createNode(qint64 id)
{
    // create node by tikz::core::Document
    tikz::core::Node * node = Document::createNode(id);
    Q_ASSERT(id == node->id());
    Q_ASSERT(! m_nodeMap.contains(id));

    // create GUI item
    NodeItem * nodeItem = new NodeItem(node);
    m_nodes.append(nodeItem);
    m_nodeMap.insert(id, nodeItem);

    // add to graphics scene
    m_scene->addItem(nodeItem);

    return node;
}

void DocumentPrivate::deleteNode(qint64 id)
{
    Q_ASSERT(m_nodeMap.contains(id));

    // get NodeItem
    NodeItem * nodeItem = m_nodeMap[id];

    // remove from scene
    m_scene->removeItem(nodeItem);

    const int index = m_nodes.indexOf(nodeItem);
    Q_ASSERT(index >= 0);

    // delete item
    m_nodeMap.remove(id);
    m_nodes.remove(index);
    delete nodeItem;

    tikz::core::Document::deleteNode(id);
}

tikz::core::Path * DocumentPrivate::createPath(tikz::core::Path::Type type, qint64 id)
{
    tikz::core::Path * path = Document::createPath(type, id);
    Q_ASSERT(id == path->id());
    Q_ASSERT(! m_pathMap.contains(id));

    // create GUI item
    tikz::ui::PathItem * pathItem = nullptr;
    switch (type) {
        case tikz::core::Path::Line: {
            pathItem = new tikz::ui::EdgePathItem(path);
            break;
        }
        case tikz::core::Path::HVLine: break;
        case tikz::core::Path::VHLine: break;
        case tikz::core::Path::BendCurve: break;
        case tikz::core::Path::InOutCurve: break;
        case tikz::core::Path::BezierCurve: break;
        case tikz::core::Path::Ellipse: {
            pathItem = new tikz::ui::EllipsePathItem(path);
            break;
        }
        case tikz::core::Path::Rectangle: break;
        case tikz::core::Path::Grid: break;
        case tikz::core::Path::Invalid:
        default: break;
    }

    // we should always have a valid ui tikz path
    Q_ASSERT(pathItem);

    // register path
    m_paths.append(pathItem);
    m_pathMap.insert(id, pathItem);

    // add to graphics scene
    m_scene->addItem(pathItem);

    return path;
}

void DocumentPrivate::deletePath(qint64 id)
{
    Q_ASSERT(m_pathMap.contains(id));

    // get tikz::ui::PathItem
    tikz::ui::PathItem * pathItem = m_pathMap[id];

    // remove from scene
    m_scene->removeItem(pathItem);

    const int index = m_paths.indexOf(pathItem);
    Q_ASSERT(index >= 0);

    // delete item
    m_pathMap.remove(id);
    m_paths.remove(index);
    delete pathItem;

    tikz::core::Document::deletePath(id);
}

NodeItem * DocumentPrivate::nodeItemFromId(qint64 id) const
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(m_nodeMap.contains(id));
    return m_nodeMap[id];
}

tikz::ui::PathItem * DocumentPrivate::pathItemFromId(qint64 id) const
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(m_pathMap.contains(id));
    return m_pathMap[id];
}

}
}

// kate: indent-width 4; replace-tabs on;
