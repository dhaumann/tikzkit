#ifndef TIKZ_PDF_RENDERER_H
#define TIKZ_PDF_RENDERER_H

#include <QObject>
#include <QPixmap>
#include <QProcess>

class PdfRendererPrivate;
class TikzNode;

class PdfRenderer : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        PdfRenderer(QObject* parent);

        /**
         * Destructor
         */
        virtual ~PdfRenderer();

    public Q_SLOTS:
        /**
         * Load the pdf file from @p filename.
         */
        void loadPdf(const QString& filename);

    public:
        /**
         * Returns a valid pixmap with desired dpi on success. The dpi values
         * @p physicalDpiX and @p physicalDpiY should include scaling factors.
         * On failure, returns a null pixmap: QPixmap::isNull() is true.
         */
        QPixmap render(qreal physicalDpiX, qreal physicalDpiY);

    private:
        PdfRendererPrivate * const d;
};

#endif // TIKZ_PDF_RENDERER_H

// kate: indent-width 4; replace-tabs on;
