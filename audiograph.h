#ifndef AUDIOGRAPH_H
#define AUDIOGRAPH_H

#include "qcustomplot.h"
#include <QAudioBuffer>
#include <QAudioDecoder>

//Based code off thibsc's code:
//https://stackoverflow.com/a/50294040

class vController;

class AudioGraph : public QCustomPlot
{
    Q_OBJECT

    public:
        AudioGraph(QWidget *parent = Q_NULLPTR, vController* controller = nullptr);
        ~AudioGraph();
        void setSource(const QString &fileName);
        int getSampleRate();
        void plotProgress(qint64 pos);

    public slots:
        void setBuffer();
        void plot();
        void errsig(QAudioDecoder::Error error);

    private:
        qreal getPeakValue(const QAudioFormat& format);

        QAudioDecoder *decoder;
        QAudioBuffer buffer;
        QVector<double> samples;
        QCPGraph *wavePlot;
        QCPGraph *progressPlot;
        QString currentFile;
        vController *base_control;
};

#endif // AUDIOGRAPH_H
