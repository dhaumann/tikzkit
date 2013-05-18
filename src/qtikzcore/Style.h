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
    // config methods
    //
    public:
        /**
         * Start changing config values.
         * This call is ref-counted. For each beginConfig() you finally
         * have to call endConfig().
         */
        void beginConfig();

        /**
         * End of changing config values.
         * This will emit changed(), if the number of calls of endConfig()
         * matches the calls the one of beginConfig(), i.e. the ref-counter is zero.
         *
         * Using beginConfig() and endConfig() allows to change multiple
         * config values, while still only emitting the changed() signal only once.
         */
        void endConfig();

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the style changes.
         * This includes changes in the parent style that possibly influence
         * the appearance of this style.
         */
        void changed();

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
         * Set the line width to @p width mm.
         * Calling this function automatically sets the lineWidth()
         * to LineWidth::CustomLineWidth.
         * @p width the line width [mm]
         * @see Linewidth
         */
        void setCustomLineWidth(qreal width);

        /**
         * Get the lineWidth() in mm.
         */
        qreal lineThickness() const;


        /**
         * The the draw opacity.
         */
        double penOpacity() const;
        double fillOpacity() const;

        QColor penColor() const;
        QColor fillColor() const;

    // TODO: pen color
    //  fill color
    // Node: inner sep, minimum size (minimum width, minimum height)

    private:
        StylePrivate * const d;
};

}

#endif // TIKZ_STYLE_H

// kate: indent-width 4; replace-tabs on;
