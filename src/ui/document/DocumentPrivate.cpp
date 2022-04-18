/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2016 Dominik Haumann <dhaumann@kde.org>
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
#include <tikz/core/UndoSetProperty.h>

//BEGIN DEBUG
#include <tikz/core/Style.h>
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

static void setProp(const tikz::core::Uid & entity, const QString & key, const QVariant & value)
{
    entity.document()->addUndoItem(new tikz::core::UndoSetProperty(entity, key, value));
}

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
        const NodeItem * nodeItem = nodeItemFromId(node->uid());
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
    // construct new view, we use a dummyMainWindow interface if no mainWindow is there
    auto view = new ViewPrivate(this, parent, mainWindow ? mainWindow : tikz::ui::EditorPrivate::self()->dummyMainWindow());
    Q_ASSERT(m_views.contains(view));

//BEGIN DEBUG
    beginTransaction("Create Nodes");
    tikz::ui::NodeItem* item1 = createNodeItem();
    item1->node()->setPos(tikz::Pos(-3, 3, tikz::Unit::Centimeter));
    qDebug() << " ____" << item1->style()->uid();
    qDebug() << " ____" << item1->style()->property("style");
    setProp(item1->style()->uid(), "lineWidth", tikz::Value::veryThin());
    setProp(item1->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
    setProp(item1->style()->uid(), "innerSep", 2.0_mm);
    setProp(item1->node()->uid(), "text", "$\\int f(x) dx$");

    tikz::ui::NodeItem* item2 = createNodeItem();
    item2->node()->setPos(tikz::Pos(3, 3, tikz::Unit::Centimeter));
    setProp(item2->style()->uid(), "lineWidth", tikz::Value::thin());
    setProp(item2->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
    setProp(item2->style()->uid(), "innerSep", 2.0_mm);
    setProp(item2->node()->uid(), "text", "$\\Leftrightarrow$");
    finishTransaction();

    beginTransaction("Create Paths");

    // an ellipse path
//     tikz::ui::PathItem* path = createPathItem(tikz::PathType::Ellipse);
//     auto ellipse = qobject_cast<tikz::core::EllipsePath*>(path->path());
//     tikz::core::Style es;
//     es.setStyle(ellipse->style());
//     es.setRadiusX(2.0_cm);
//     es.setRadiusY(1.0_cm);
//     es.setLineWidth(tikz::Value::semiThick());
//     ellipse->setStyle(es);

    // add a line path
    auto path = createPathItem(tikz::PathType::Line);
    auto edge = qobject_cast<tikz::core::EdgePath*>(path->path());
    setProp(edge->style()->uid(), "lineWidth", tikz::Value::semiThick());
    //setProp(edge->style()->uid(), "doubleLine", true);
    setProp(edge->style()->uid(), "arrowTail", QVariant::fromValue(tikz::Arrow::LatexArrow));
    setProp(edge->style()->uid(), "arrowHead", QVariant::fromValue(tikz::Arrow::ToArrow));
    edge->setStartNode(item1->node());
    edge->setEndNode(item2->node());
    finishTransaction();

#if 0
    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, 1, tikz::Unit::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::thin());
    item1->node()->style()->setShape(tikz::Shape::ShapeRectangle);
    item1->node()->style()->setInnerSep(2.0_mm);
    item1->node()->setText("$a$");

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, 1, tikz::Unit::Centimeter));
    item2->node()->style()->setLineWidth(tikz::Value::semiThick());
    item2->node()->style()->setShape(tikz::Shape::ShapeCircle);
    item2->node()->style()->setInnerSep(2.0_mm);
    item2->node()->setText("a");

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::semiThick());


    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, -1, tikz::Unit::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::veryThick());
    item1->node()->style()->setShape(tikz::Shape::ShapeRectangle);
    item1->node()->style()->setInnerSep(2.0_mm);
    item1->node()->setText("$\\iiint \\max$");

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, -1, tikz::Unit::Centimeter));
    item2->node()->style()->setLineWidth(tikz::Value::ultraThick());
    item2->node()->style()->setShape(tikz::Shape::ShapeRectangle);
    item2->node()->style()->setInnerSep(2.0_mm);
    item2->node()->setText("A long text\\\\which is wrapped");

//     item2->style()->setParentStyle(item1->style());

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::ultraThick());
    path->path()->style()->setArrowTail(tikz::Arrow::LatexArrow);
    path->path()->style()->setArrowHead(tikz::Arrow::PipeArrow);


    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, -3, tikz::Unit::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::veryThick());
    item1->node()->style()->setShape(tikz::Shape::ShapeRectangle);
    item1->node()->style()->setInnerSep(2.0_mm);
    item1->node()->setText("c");

    a = item1;

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, -3, tikz::Unit::Centimeter));
    item2->node()->style()->setDoubleLine(true);
    item2->node()->style()->setLineWidth(tikz::Value::semiThick());
    item2->node()->style()->setShape(tikz::Shape::ShapeEllipse);
    item2->node()->style()->setInnerSep(2.0_mm);
    item2->node()->style()->setMinimumWidth(2);
    item2->node()->setText("8");

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::ultraThick());
    path->path()->style()->setBendAngle(30);
//     path->path()->style()->setCurveMode(tikz::HVLineTo);
    path->path()->style()->setArrowTail(tikz::Arrow::StealthArrow);
    path->path()->style()->setArrowHead(tikz::Arrow::ToArrow);


    // arrow demo
    for (int i = 0; i < tikz::Arrow::ArrowCount; ++i) {
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
    for (int i = 0; i < tikz::Arrow::ArrowCount; ++i) {
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
        n1->node()->setPos(tikz::Pos(0, 6, tikz::Unit::Centimeter));
        setProp(n1->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
        setProp(n1->style()->uid(), "innerSep", 2.0_mm);
        setProp(n1->style()->uid(), "minimumWidth", 2.0_cm);
        setProp(n1->style()->uid(), "minimumHeight", 1.5_cm);
        setProp(n1->style()->uid(), "penColor", QColor(0, 0, 204));
        setProp(n1->style()->uid(), "fillColor", QColor(204, 204, 255));
        setProp(n1->node()->uid(), "text", "Kate Part\\\\(Backend library)");

        tikz::ui::NodeItem *n2 = createNodeItem();
        n2->node()->setPos(tikz::Pos(0, 8, tikz::Unit::Centimeter));
        setProp(n2->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
        setProp(n2->style()->uid(), "innerSep", 2.0_mm);
        setProp(n2->style()->uid(), "minimumWidth", 2.0_cm);
        setProp(n2->style()->uid(), "minimumHeight", 1.5_cm);
        setProp(n2->style()->uid(), "penColor", QColor(255, 153, 51));
        setProp(n2->style()->uid(), "fillColor", QColor(255, 230, 204));
        setProp(n2->node()->uid(), "text", "KTextEditor\\\\(Interfaces)");

        tikz::ui::NodeItem *n3 = createNodeItem();
        n3->node()->setPos(tikz::Pos(-5.8, 10, tikz::Unit::Centimeter));
        setProp(n3->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
        setProp(n3->style()->uid(), "innerSep", 2.0_mm);
        setProp(n3->style()->uid(), "minimumWidth", 2.5_cm);
        setProp(n3->style()->uid(), "minimumHeight", 1.5_cm);
        setProp(n3->style()->uid(), "penColor", QColor(255, 51, 51));
        setProp(n3->style()->uid(), "fillColor", QColor(255, 204, 204));
        setProp(n3->node()->uid(), "text", "Kate\\\\(Application)");

        tikz::ui::NodeItem *n4 = createNodeItem();
        n4->node()->setPos(tikz::Pos(-2.9, 10, tikz::Unit::Centimeter));
        setProp(n4->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
        setProp(n4->style()->uid(), "innerSep", 2.0_mm);
        setProp(n4->style()->uid(), "minimumWidth", 2.5_cm);
        setProp(n4->style()->uid(), "minimumHeight", 1.5_cm);
        setProp(n4->style()->uid(), "penColor", QColor(255, 51, 51));
        setProp(n4->style()->uid(), "fillColor", QColor(255, 204, 204));
        setProp(n4->node()->uid(), "text", "KWrite\\\\(Application)");

        tikz::ui::NodeItem *n5 = createNodeItem();
        n5->node()->setPos(tikz::Pos(0, 10, tikz::Unit::Centimeter));
        setProp(n5->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
        setProp(n5->style()->uid(), "innerSep", 2.0_mm);
        setProp(n5->style()->uid(), "minimumWidth", 2.5_cm);
        setProp(n5->style()->uid(), "minimumHeight", 1.5_cm);
        setProp(n5->style()->uid(), "penColor", QColor(255, 51, 51));
        setProp(n5->style()->uid(), "fillColor", QColor(255, 204, 204));
        setProp(n5->node()->uid(), "text", "KDevelop\\\\(Application)");

        tikz::ui::NodeItem *n6 = createNodeItem();
        n6->node()->setPos(tikz::Pos(2.9, 10, tikz::Unit::Centimeter));
        setProp(n6->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
        setProp(n6->style()->uid(), "innerSep", 2.0_mm);
        setProp(n6->style()->uid(), "minimumWidth", 2.5_cm);
        setProp(n6->style()->uid(), "minimumHeight", 1.5_cm);
        setProp(n6->style()->uid(), "penColor", QColor(255, 51, 51));
        setProp(n6->style()->uid(), "fillColor", QColor(255, 204, 204));
        setProp(n6->node()->uid(), "text", "Kile\\\\(Application)");

        tikz::ui::NodeItem *n7 = createNodeItem();
        n7->node()->setPos(tikz::Pos(5.8, 10, tikz::Unit::Centimeter));
        setProp(n7->style()->uid(), "shape", QVariant::fromValue(tikz::Shape::ShapeRectangle));
        setProp(n7->style()->uid(), "innerSep", 2.0_mm);
        setProp(n7->style()->uid(), "minimumWidth", 2.5_cm);
        setProp(n7->style()->uid(), "minimumHeight", 1.5_cm);
        setProp(n7->style()->uid(), "penColor", QColor(255, 51, 51));
        setProp(n7->style()->uid(), "fillColor", QColor(255, 204, 204));
        setProp(n7->node()->uid(), "text", "3rd party\\\\(Application)");

        //
        // paths
        //
        tikz::ui::PathItem* path = createPathItem();
        auto edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n1->node());
        edge->setEndNode(n2->node());
        setProp(edge->style()->uid(), "arrowTail", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "arrowHead", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "penColor", QColor(128, 128, 128));

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n3->node());
        setProp(edge->style()->uid(), "arrowTail", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "arrowHead", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "penColor", QColor(128, 128, 128));
        edge->setStartAnchor("west");

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n4->node());
        setProp(edge->style()->uid(), "arrowTail", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "arrowHead", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "penColor", QColor(128, 128, 128));

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n5->node());
        setProp(edge->style()->uid(), "arrowTail", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "arrowHead", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "penColor", QColor(128, 128, 128));

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n6->node());
        setProp(edge->style()->uid(), "arrowTail", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "arrowHead", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "penColor", QColor(128, 128, 128));

        path = createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n7->node());
        setProp(edge->style()->uid(), "arrowTail", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "arrowHead", QVariant::fromValue(tikz::Arrow::LatexArrow));
        setProp(edge->style()->uid(), "penColor", QColor(128, 128, 128));
        edge->setStartAnchor("east");

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
    tikz::core::Node * node = qobject_cast<tikz::core::Node*>(Document::createNode());
    Q_ASSERT(m_nodeMap.contains(node->uid()));

    return m_nodeMap[node->uid()];
}

tikz::ui::PathItem * DocumentPrivate::createPathItem(tikz::PathType type)
{
    // create path
    auto path = Document::createPath(); // FIXME: type
    Q_ASSERT(m_pathMap.contains(path->uid()));

    return m_pathMap[path->uid()];
}

void DocumentPrivate::deleteNodeItem(NodeItem * node)
{
    // delete node from id
    const auto uid = node->uid();
    Q_ASSERT(m_nodeMap.contains(uid));
    Document::deleteEntity(node->node());
    Q_ASSERT(! m_nodeMap.contains(uid));
}

void DocumentPrivate::deletePathItem(tikz::ui::PathItem * path)
{
    // delete path from id
    const auto uid = path->uid();
    Q_ASSERT(m_pathMap.contains(uid));
    Document::deleteEntity(path->path());
    Q_ASSERT(! m_pathMap.contains(uid));
}

tikz::core::Entity * DocumentPrivate::createEntity(const tikz::core::Uid & uid, EntityType type)
{
    // create entity by tikz::core::Document
    auto entity = Document::createEntity(uid, type);

    switch (type) {
        case tikz::EntityType::Node: {
            auto node = qobject_cast<tikz::core::Node *>(entity);
            Q_ASSERT(uid == node->uid());
            Q_ASSERT(! m_nodeMap.contains(uid));

            // create GUI item
            NodeItem * nodeItem = new NodeItem(node);
            m_nodes.append(nodeItem);
            m_nodeMap.insert(uid, nodeItem);

            // add to graphics scene
            m_scene->addItem(nodeItem);

            break;
        }
        case tikz::EntityType::Path: {
            auto path = qobject_cast<tikz::core::EdgePath *>(entity);
            Q_ASSERT(uid == path->uid());
            Q_ASSERT(! m_pathMap.contains(uid));

            // create GUI item
            auto * pathItem = new tikz::ui::EdgePathItem(path);
            m_paths.append(pathItem);
            m_pathMap.insert(uid, pathItem);

            // add to graphics scene
            m_scene->addItem(pathItem);

            break;
        }
        case tikz::EntityType::Style:
            // nothing to do
            break;
        default: Q_ASSERT(false);
    }

    return entity;
}

void DocumentPrivate::deleteEntity(const tikz::core::Uid & uid)
{
    switch (uid.entityType()) {
        case tikz::EntityType::Node: {
            Q_ASSERT(m_nodeMap.contains(uid));

            // get NodeItem
            NodeItem * nodeItem = m_nodeMap[uid];

            // remove from scene
            m_scene->removeItem(nodeItem);

            const int index = m_nodes.indexOf(nodeItem);
            Q_ASSERT(index >= 0);

            // delete item
            m_nodeMap.remove(uid);
            m_nodes.remove(index);
            delete nodeItem;
            break;
        }
        case tikz::EntityType::Path: {
            Q_ASSERT(m_pathMap.contains(uid));

            // get NodeItem
            PathItem * pathItem = m_pathMap[uid];

            // remove from scene
            m_scene->removeItem(pathItem);

            const int index = m_paths.indexOf(pathItem);
            Q_ASSERT(index >= 0);

            // delete item
            m_pathMap.remove(uid);
            m_paths.remove(index);
            delete pathItem;
            break;
        }
        case tikz::EntityType::Style:
            // nothing to do
            break;
        default: Q_ASSERT(false);
    }

    tikz::core::Document::deleteEntity(uid);
}

tikz::core::Path * DocumentPrivate::createPath(tikz::PathType type, const tikz::core::Uid & uid)
{
    auto path = Document::createPath(type, uid);
    Q_ASSERT(uid == path->uid());
    Q_ASSERT(! m_pathMap.contains(uid));

    // create GUI item
    tikz::ui::PathItem * pathItem = nullptr;
    switch (type) {
        case tikz::PathType::Line: {
            pathItem = new tikz::ui::EdgePathItem(path);
            break;
        }
        case tikz::PathType::HVLine: break;
        case tikz::PathType::VHLine: break;
        case tikz::PathType::BendCurve: break;
        case tikz::PathType::InOutCurve: break;
        case tikz::PathType::BezierCurve: break;
        case tikz::PathType::Ellipse: {
            pathItem = new tikz::ui::EllipsePathItem(path);
            break;
        }
        case tikz::PathType::Rectangle: break;
        case tikz::PathType::Grid: break;
        case tikz::PathType::Invalid:
        default: break;
    }

    // we should always have a valid ui tikz path
    Q_ASSERT(pathItem);

    // register path
    m_paths.append(pathItem);
    m_pathMap.insert(uid, pathItem);

    // add to graphics scene
    m_scene->addItem(pathItem);

    return path;
}

NodeItem * DocumentPrivate::nodeItemFromId(const tikz::core::Uid & uid) const
{
    if (! uid.isValid()) {
        return nullptr;
    }

    Q_ASSERT(m_nodeMap.contains(uid));
    return m_nodeMap[uid];
}

tikz::ui::PathItem * DocumentPrivate::pathItemFromId(const tikz::core::Uid & uid) const
{
    if (! uid.isValid()) {
        return nullptr;
    }

    Q_ASSERT(m_pathMap.contains(uid));
    return m_pathMap[uid];
}

}
}

// kate: indent-width 4; replace-tabs on;
