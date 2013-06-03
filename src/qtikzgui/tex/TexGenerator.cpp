#include "TexGenerator.h"
#include "TikzNode.h"

#include <QPointer>
#include <QPainter>
#include <QGraphicsView>
#include <QProcess>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QTextStream>

#include <QDebug>

class TexGeneratorPrivate
{
    public:
        QProcess * process;
        QTemporaryFile * tempFile;
        bool postProcessRunning;
};

TexGenerator::TexGenerator(QObject * parent)
: QObject(parent)
    , d(new TexGeneratorPrivate())
{
    d->process = 0;
    d->tempFile = 0;
    d->postProcessRunning = false;
}

TexGenerator::~TexGenerator()
{
    delete d;
}

QString TexGenerator::pdfFile()
{
    if (d->tempFile) {
        QFileInfo fi(*d->tempFile);
        return fi.baseName() + ".pdf";
    }
    return QString();
}

QString TexGenerator::svgFile()
{
    if (d->tempFile) {
        QFileInfo fi(*d->tempFile);
        return fi.baseName() + ".svg";
    }
    return QString();
}

void TexGenerator::generateImage(const QString& texCode)
{
    if (!d->process) {
        d->process = new QProcess(this);

        connect(d->process, SIGNAL(readyReadStandardError()),
                this, SLOT(outputReady()));
        connect(d->process, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(processFinished(int, QProcess::ExitStatus)));
        connect(d->process, SIGNAL(error(QProcess::ProcessError)),
                this, SLOT(processError(QProcess::ProcessError)));
    } else {
        if (d->process->state() == QProcess::Running) {
            d->process->terminate();
        }
    }

    if (d->tempFile) {
        delete d->tempFile;
    }

    d->tempFile = new QTemporaryFile("XXXXXX", this);
    if (!d->tempFile->open()) {
        qWarning() << "Could not create temporary tex file";
        return;
    } else {
        QTextStream ts(d->tempFile);
        ts << "\\documentclass[paper=a5,fontsize=12pt]{scrbook}\n"
        "\\usepackage[pdftex,active,tightpage]{preview}\n"
        "\\usepackage{amsmath}\n"
        "\\usepackage{amssymb}\n"
        "\\usepackage{amsfonts}\n"
        "\\usepackage{tikz}\n"
        "\\begin{document}\n"
        "\\begin{preview}\n"
        "\\begin{tikzpicture}[inner sep=0pt, outer sep=0pt]\n"
        "\\node at (0, 0) {"+ texCode + "};\n"
        "\\end{tikzpicture}\n"
        "\\end{preview}\n"
        "\\end{document}";
        d->tempFile->close();
    }

    QStringList args;
    args << "-halt-on-error" << d->tempFile->fileName();

    qDebug() << "launching process: 'latex'";
    d->process->start("latex", args);
}

void TexGenerator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "process finished with exitCode" << exitCode;
    if (exitCode != 0) {
        return;
    }

    if (!d->postProcessRunning) {
        QFileInfo fi(*d->tempFile);
        QStringList args;
        args << "--no-fonts"<<(fi.baseName() + ".dvi")
             << (fi.baseName() + ".svg");

        qDebug() << "launching process: 'dvisvgm'";
        d->postProcessRunning = true;
        d->process->start("dvisvgm", args);
    } else {
        d->postProcessRunning = false;

        QFileInfo fi(*d->tempFile);
        emit svgReady(fi.baseName() + ".svg");
    }
}

void TexGenerator::processError(QProcess::ProcessError error)
{
//     qDebug() << "process error" << error;
}

void TexGenerator::outputReady()
{
//     qDebug() << "output ready" << d->process->readAllStandardError();
}

// kate: indent-width 4; replace-tabs on;
