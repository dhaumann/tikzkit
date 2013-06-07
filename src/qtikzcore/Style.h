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
    // pen style
    //
    public:
        /**
         * Get the PenStyle of this style.
         * @see PenStyle
         */
        PenStyle penStyle() const;

    public Q_SLOTS:
        /**
         * Set the PenStyle of this style to @p style.
         * @see PenStyle
         */
        void setPenStyle(tikz::PenStyle style);

        /**
         * Unset the pen style.
         * After this, calling penStyle() returns the parent()->penStyle()
         * if a parent() style exists, otherwise penStyle() returns the
         * default pen style @e SolidLine.
         * @see PenStyle, penStyle()
         */
        void unsetPenStyle();

    //
    // pen style / line width
    //
    public:
        /**
         * Get the line width.
         * @see LineWidth
         */
        LineWidth lineWidthType() const;

        /**
         * Get the lineWidth() in cm.
         */
        qreal lineWidth() const;

        /**
         * Get the pen width for drawing in cm.
         * The pen with equals lineWidth(), if double lines are disabled.
         * If double lines are enabled, the pen width is set to
         * 2 * lineWidth() + innerLineWidth().
         */
        qreal penWidth() const;

    public Q_SLOTS:
        /**
         * Set the line width type to @p type.
         * A cusom line width is achieved by alling setLineWidth().
         * @p width the line width type
         * @see LineWidth
         */
        void setLineWidthType(tikz::LineWidth type);

        /**
         * Set the line width to @p width cm.
         * Calling this function automatically sets the lineWidth()
         * to LineWidth::CustomLineWidth.
         * @p width the line width [cm]
         * @see Linewidth
         */
        void setLineWidth(qreal width);

        /**
         * Unset the line width.
         * After this, calling lineWidth() or lineWidthType() return the
         * respective values of the parent() style, if the parent() style exists,
         * otherwise the returned line width is @e SemiThick.
         * @see LineWidth, lineWidth(), lineWidthType()
         */
        void unsetLineWidth();

    //
    // double lines
    //
    public:
        /**
         * Get whether this line is a double line.
         * By default, returns @e false.
         */
        bool isDoubleLine() const;

    public Q_SLOTS:
        /**
         * Draw double lines if @p enabled is @e true.
         */
        void setDoubleLine(bool enabled);

    public:
        /**
         * Get the inner line width in cm.
         * The inner line width is used for double lines in paths and shapes.
         */
        qreal innerLineWidth() const;


        /**
         * Get the line width type.
         * If the type is LineWidth::CustomLineWidth, use innerLineWidth()
         * to get the width in cm.
         * @see LineWidth
         */
        LineWidth innerLineWidthType() const;

    public Q_SLOTS:
        /**
         * Set the inner line width to @p width cm.
         * Calling this function automatically sets the innerLineWidth()
         * to LineWidth::CustomLineWidth.
         * @p width the line width [cm]
         * @see LineWidth
         */
        void setInnerLineWidth(qreal width);

        /**
         * Set the inner line width type to @p type.
         * A cusom line width is achieved by alling setInnerLineWidth().
         * @p width the line width
         * @see LineWidth
         */
        void setInnerLineWidthType(tikz::LineWidth type);

    //
    // draw & fill opacity
    //
    public:
        /**
         * Get the opacity for drawing primitives in the interval [0.0; 1.0].
         */
        qreal penOpacity() const;

        /**
         * Get the opacity for filling operations in the interval [0.0; 1.0].
         */
        qreal fillOpacity() const;

    public Q_SLOTS:
        /**
         * Set the opacity for drawing primitives to @p opacity.
         * @param opacity the opacity in the interval [0.0; 1.0]
         */
        void setPenOpacity(qreal opacity);

        /**
         * Unsets the draw opacity of the pen.
         * Afterwards, penOpacity() returns either the opacity of the parent()
         * style, or the default value @e 1.0.
         */
        void unsetPenOpacity();

        /**
         * Set the opacity for filling primitives to @p opacity.
         * @param opacity the opacity in the interval [0.0; 1.0]
         */
        void setFillOpacity(qreal opacity);

        /**
         * Unsets the fill opacity of fill operations.
         * Afterwards, fillOpacity() returns either the opacity of the parent()
         * style, or the default value @e 1.0.
         */
        void unsetFillOpacity();

    //
    // pen color and fill color/brush
    //
    public:
        QColor penColor() const;
        QColor fillColor() const;

    // TODO: pen color
    //  fill color

    private:
        StylePrivate * const d;
};

}

#endif // TIKZ_STYLE_H

// kate: indent-width 4; replace-tabs on;
