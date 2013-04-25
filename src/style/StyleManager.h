#ifndef TIKZ_STYLEMANAGER_H
#define TIKZ_STYLEMANAGER_H

#include <QVector>

namespace tikz {

class Style;
class NodeStyle;
class EdgeStyle;

class StyleManager
{
    private:
        static StyleManager* s_self;

    public:
        ~StyleManager();

        static StyleManager* self();

//         Style* documentStyle();

        void registerNodeStyle(NodeStyle* style);
        void unregisterNodeStyle(NodeStyle* style);

        void registerEdgeStyle(EdgeStyle* style);
        void unregisterEdgeStyle(EdgeStyle* style);

    //
    // private constructor, as it is a singleton
    //
    private:
        /**
         * Constructor. Called in ::self() accessor.
         */
        StyleManager();

    private:
//         Style* m_documentStyle;
        QVector<NodeStyle*> m_nodeStyles;
        QVector<EdgeStyle*> m_edgeStyles;
};

}

#endif // TIKZ_STYLEMANAGER_H

// kate: indent-width 4; replace-tabs on;
