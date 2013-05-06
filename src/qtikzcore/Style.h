#ifndef TIKZ_STYLE_H
#define TIKZ_STYLE_H

#include "tikz.h"
#include "tikz_export.h"

#include <QColor>
#include <QObject>

namespace tikz {

class Document;
class StylePrivate;

class TIKZCORE_EXPORT Style : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Style();

        /**
         * Associate this style with the document @p tikzDocument.
         */
        Style(Document* tikzDocument);

        /**
         * Virtual destructor.
         */
        virtual ~Style();

    //
    // parent hierarchy
    //
    public:
        /**
         * Get the parent style this style inherits from.
         */
        Style *parent() const;

        /**
         * Set @p parent as new parent to inherit attributes from.
         */
        void setParent(Style *parent);

    //
    // common style attributes of nodes and paths
    //
    public:
        /**
         * Get the LineStyle of this style.
         * @see PenStyle
         */
        PenStyle penStyle() const;

        /**
         * Get the line width.
         * @see Linewidth
         */
        LineWidth lineWidth() const;

        /**
         * Set the line width.
         * @p width the line width
         * @see Linewidth
         */
        void setLineWidth(tikz::LineWidth lineWidth);

        /**
         * The the draw opacity.
         */
        double drawOpacity() const;
        double fillOpacity() const;

        QColor drawColor() const;
        QColor fillColor() const;

    // TODO: pen color
    //  fill color
    // Node: inner sep, minimum size (minimum width, minimum height)

    //
    // signals
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the style changes.
         */
        void changed();

    private:
        StylePrivate * const d;
};

}

#endif // TIKZ_STYLE_H

// kate: indent-width 4; replace-tabs on;
