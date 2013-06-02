#ifndef TIKZ_TEX_GENERATOR_H
#define TIKZ_TEX_GENERATOR_H

#include <QObject>
#include <QPixmap>
#include <QProcess>

class TexGeneratorPrivate;

class TexGenerator : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        TexGenerator(QObject * parent);

        /**
         * Destructor
         */
        virtual ~TexGenerator();

        QPixmap image();

        QString pdfFile();
        QString svgFile();

    public Q_SLOTS:
        void generateImage(const QString& texCode);

    protected Q_SLOTS:
        void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
        void processError(QProcess::ProcessError error);
        void outputReady();

    Q_SIGNALS:
        void svgReady(const QString& path);

    private:
        TexGeneratorPrivate * const d;
};

#endif // TIKZ_TEX_GENERATOR_H

// kate: indent-width 4; replace-tabs on;
