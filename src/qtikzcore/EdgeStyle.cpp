#include "EdgeStyle.h"

namespace tikz {

/**
 * Private data and helper functions of class EdgeStyle.
 */
class EdgeStylePrivate
{
public:
};

EdgeStyle::EdgeStyle()
    : Style()
    , d(new EdgeStylePrivate())
{
}

EdgeStyle::EdgeStyle(Document* tikzDocument)
    : Style(tikzDocument)
    , d(new EdgeStylePrivate())
{
}

EdgeStyle::~EdgeStyle()
{
    delete d;
}

}

// kate: indent-width 4; replace-tabs on;
