#ifndef TIKZ_DOCUMENT_H
#define TIKZ_DOCUMENT_H

#include "tikz_export.h"
#include "tikz.h"

#include <QObject>

namespace tikz
{

class DocumentPrivate;
class Style;
class Node;
class Edge;

class TIKZCORE_EXPORT Document : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Document(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~Document();

    //
    // Style management
    //
    public:
        /**
         * Get the global document style.
         * This is the global document style. All other styles fall back to
         * this style, if a property is not set.
         */
        Style& style() const;

        /**
         * Get the number of custom styles in this document.
         * A document can hold arbitrary many custom styles.
         * @see customStyle()
         */
        int customStyleCount() const;

        /**
         * Returns the i-th custom style of this document.
         * @see customStyleCount()
         */
        Style* customStyle(int i) const;

    private:
        DocumentPrivate * const d;
};

}

#endif // TIKZ_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
