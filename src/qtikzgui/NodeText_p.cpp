#include "NodeText_p.h"
#include "NodeText.h"
#include "TikzNode.h"

#include "TexGenerator.h"

#include <Node.h>

#include <QDebug>

NodeTextPrivate::NodeTextPrivate(TikzNode* tikzNode, NodeText* nodeText)
    : QObject()
    , svgRenderer(this)
    , texGenerator(this)
{
    q = nodeText;
    node = tikzNode;

    connect(&texGenerator, SIGNAL(svgReady(QString)), this, SLOT(readSvgFile(QString)));
    connect(this, SIGNAL(svgChanged()), &tikzNode->node(), SIGNAL(changed()));
}

void NodeTextPrivate::updateCache()
{
}

void NodeTextPrivate::readSvgFile(const QString& file)
{
    q->prepareGeometryChange();
    svgRenderer.load(file);
    emit svgChanged();
}

// kate: indent-width 4; replace-tabs on;
