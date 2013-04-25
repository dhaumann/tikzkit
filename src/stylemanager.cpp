#include "stylemanager.h"

#include "style.h"

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
}

StyleManager::~StyleManager()
{
    while (!m_styles.isEmpty()) {
        delete m_styles[0];
        m_styles.remove(0);
    }

    // last, invalidate s_self pointer again
    s_self = 0;
}

void StyleManager::registerStyle(Style* style)
{
    Q_ASSERT(!m_styles.contains(style));

    m_styles.append(style);
}

void StyleManager::unregisterStyle(Style* style)
{
    Q_ASSERT(m_styles.contains(style));

    // fix parent relationship, if required
    foreach (Style* s, m_styles) {
        if (s == style) continue;
        if (s->parent() == style) {
            s->setParent(style->parent());
        }
    }

    // now it's save to remove the style
    m_styles.remove(m_styles.indexOf(style));
}

}

// kate: indent-width 4; replace-tabs on;
