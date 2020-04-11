#ifndef AUDIOGRAPH_H
#define AUDIOGRAPH_H

#include "qcustomplot.h"
#include <QAudioBuffer>


//Based code off thibsc's code:
//https://stackoverflow.com/a/50294040


class QAudioDecoder;

class AudioGraph : public QCustomPlot
{
    Q_OBJECT

    public:
        AudioGraph(QWidget *parent = Q_NULLPTR);
        ~AudioGraph();
        void setSource(const QString &fileName);
        int getSampleRate();
        void plotProgress(qint64 pos);

    public slots:
        void setBuffer();
        void plot();

    private:
        qreal getPeakValue(const QAudioFormat& format);

        QAudioDecoder *decoder;
        QAudioBuffer buffer;
        QVector<double> samples;
        QCPGraph *wavePlot;
        QCPGraph *progressPlot;
        QString currentFile;

};

#endif // AUDIOGRAPH_H
