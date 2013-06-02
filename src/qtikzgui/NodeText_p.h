#ifndef TIKZ_NODE_TEXT_ITEM_PRIVATE_H
#define TIKZ_NODE_TEXT_ITEM_PRIVATE_H

#include <QObject>
#include <QSvgRenderer>

#include "TexGenerator.h"

class TikzNode;
class NodeText;
class TexGenerator;
class PdfRenderer;

class NodeTextPrivate : public QObject
{
    Q_OBJECT

    public:
        NodeTextPrivate(TikzNode* tikzNode, NodeText* nodeText);

    public:
        NodeText* q;
        TikzNode* node;

        QSvgRenderer svgRenderer;
        TexGenerator texGenerator;

    public:
        void updateCache();

    Q_SIGNALS:
        void svgChanged();

    public Q_SLOTS:
        void readSvgFile(const QString& file);
};


#endif // TIKZ_NODE_TEXT_ITEM_PRIVATE_H

// kate: indent-width 4; replace-tabs on;
