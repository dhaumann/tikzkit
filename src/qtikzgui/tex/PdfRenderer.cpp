#include "PdfRenderer.h"

#include <QPainter>
#include <QPixmap>

#include <QDebug>

#include <poppler-qt4.h>

class PdfRendererPrivate
{
    public:
        QPixmap pixmap;
        Poppler::Document * document;
};

PdfRenderer::PdfRenderer(QObject* parent)
    : QObject(parent)
    , d(new PdfRendererPrivate())
{
    d->document = 0;
}

PdfRenderer::~PdfRenderer()
{
    delete d;
}

void PdfRenderer::loadPdf(const QString& filename)
{
    delete d->document;
    d->document = Poppler::Document::load(filename);
}

QPixmap PdfRenderer::render(qreal physicalDpiX, qreal physicalDpiY)
{
    if (!d->document) {
        return QPixmap();
    }

    const int page = 0;
    d->document->setRenderBackend(Poppler::Document::ArthurBackend);
    d->document->setRenderHint(Poppler::Document::Antialiasing, true);
    d->document->setRenderHint(Poppler::Document::TextAntialiasing, true);
    d->document->setRenderHint(Poppler::Document::TextHinting, true);
    d->document->setRenderHint(Poppler::Document::TextSlightHinting, true);
    d->document->setPaperColor(Qt::transparent);
    QImage image = d->document->page(page)->renderToImage(physicalDpiX, physicalDpiY);

    //BEGIN hack: convert white background to transparent
    struct ArgbData {
        unsigned char b, g, r, a;
    };
    const int imageWidth = image.width();
    const int imageHeight = image.height();

    for (int x=0; x<imageHeight; ++x) {
        ArgbData* scanLine = reinterpret_cast<ArgbData*>(image.scanLine(x));
        for (int y=0; y<imageWidth; ++y) {
            ArgbData* unit = &scanLine[y];
            unit->a = 0xff - unit->r;
            unit->r = unit->g = unit->b = 0;
        }
    }
    //END hack
    d->pixmap = QPixmap::fromImage(image);
    return d->pixmap;
}

// kate: indent-width 4; replace-tabs on;
