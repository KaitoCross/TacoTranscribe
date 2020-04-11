#ifndef AUDIOGRAPH_H
#define AUDIOGRAPH_H

#include "qcustomplot.h"
#include <QAudioBuffer>


//From thibsc:
//https://stackoverflow.com/a/50294040


class QAudioDecoder;

class AudioGraph : public QCustomPlot
{
    Q_OBJECT

    public:
        AudioGraph(QWidget *parent = Q_NULLPTR);
        ~AudioGraph();
        void setSource(const QString &fileName);

    public slots:
        void setBuffer();
        void plot();

    private:
        qreal getPeakValue(const QAudioFormat& format);

        QAudioDecoder *decoder;
        QAudioBuffer buffer;
        QVector<double> samples;
        QCPGraph *wavePlot;

};

#endif // AUDIOGRAPH_H
