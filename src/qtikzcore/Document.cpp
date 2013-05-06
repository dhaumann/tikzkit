#include "Document.h"
#include "Node.h"
#include "Edge.h"
#include "Style.h"

#include <QDebug>

namespace tikz {

class DocumentPrivate
{
    public:
        Style style;

        QVector<Style*> customStyles;
};

Document::Document(QObject * parent)
    : QObject(parent)
    , d(new DocumentPrivate())
{
}

Document::~Document()
{
    delete d;
}

Style& Document::style() const
{
    return d->style;
}

int Document::customStyleCount() const
{
    return d->customStyles.count();
}

Style* Document::customStyle(int i) const
{
    Q_ASSERT(i >= 0 && i < d->customStyles.count());
    return d->customStyles[i];
}

}

// kate: indent-width 4; replace-tabs on;
