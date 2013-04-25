#include "StyleManager.h"

#include "NodeStyle.h"
#include "EdgeStyle.h"

namespace tikz {

StyleManager* StyleManager::s_self = 0;

StyleManager* StyleManager::self()
{
    // if the singleton does not exist, create it
    if (!s_self) {
        s_self = new StyleManager();
    }

    return s_self;
}

StyleManager::StyleManager()
{
//     m_documentStyle = new Style();
}

StyleManager::~StyleManager()
{
    while (!m_nodeStyles.isEmpty()) {
        delete m_nodeStyles[0];
        m_nodeStyles.remove(0);
    }

    while (!m_edgeStyles.isEmpty()) {
        delete m_edgeStyles[0];
        m_edgeStyles.remove(0);
    }

    // last, invalidate s_self pointer again
    s_self = 0;
}

// Style* StyleManager::documentStyle()
// {
//     return m_documentStyle;
// }

void StyleManager::registerNodeStyle(NodeStyle* style)
{
    Q_ASSERT(!m_nodeStyles.contains(style));

    m_nodeStyles.append(style);
}

void StyleManager::unregisterNodeStyle(NodeStyle* style)
{
    Q_ASSERT(m_nodeStyles.contains(style));

    // fix parent relationship, if required
    foreach (NodeStyle* s, m_nodeStyles) {
        if (s == style) continue;
        if (s->parent() == style) {
            s->setParent(style->parent());
        }
    }

    // now it's save to remove the style
    m_nodeStyles.remove(m_nodeStyles.indexOf(style));
}

void StyleManager::registerEdgeStyle(EdgeStyle* style)
{
    Q_ASSERT(!m_edgeStyles.contains(style));

    m_edgeStyles.append(style);
}

void StyleManager::unregisterEdgeStyle(EdgeStyle* style)
{
    Q_ASSERT(m_edgeStyles.contains(style));

    // fix parent relationship, if required
    foreach (EdgeStyle* s, m_edgeStyles) {
        if (s == style) continue;
        if (s->parent() == style) {
            s->setParent(style->parent());
        }
    }

    // now it's save to remove the style
    m_edgeStyles.remove(m_edgeStyles.indexOf(style));
}

}

// kate: indent-width 4; replace-tabs on;
