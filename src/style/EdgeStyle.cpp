#include "EdgeStyle.h"

#include "StyleManager.h"

namespace tikz {

class EdgeStylePrivate
{
public:
};

EdgeStyle::EdgeStyle()
    : d(new EdgeStylePrivate())
{
    // last: register ourself as style
    StyleManager::self()->registerEdgeStyle(this);
}

EdgeStyle::~EdgeStyle()
{
    // first, unregister style
    StyleManager::self()->unregisterEdgeStyle(this);
}

}

// kate: indent-width 4; replace-tabs on;
