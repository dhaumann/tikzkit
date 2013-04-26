#include "Path.h"

namespace tikz {

class PathPrivate
{
    public:
        bool closed;
};

Path::Path()
    : d(new PathPrivate())
{
    d->closed = false;
}

Path::~Path()
{
    delete d;
}

bool Path::isClosed() const
{
    return d->closed;
}

void Path::setClosed(bool closed)
{
    d->closed = closed;
}

}

// kate: indent-width 4; replace-tabs on;
