#include "audiograph.h"
#include <QAudioDecoder>
#include <algorithm>
#include <iostream>

//From thibsc:
//https://stackoverflow.com/a/50294040

AudioGraph::AudioGraph(QWidget *parent)
    : QCustomPlot(parent)
    , decoder(new QAudioDecoder(this))
{
    wavePlot = addGraph();
    setMinimumHeight(100);
    connect(decoder, SIGNAL(bufferReady()), this, SLOT(setBuffer()));
    connect(decoder, SIGNAL(finished()), this, SLOT(plot()));
}

AudioGraph::~AudioGraph()
{
    samples.clear();
    delete decoder;
    wavePlot->data()->clear();
    this->removePlottable(wavePlot);
}

void AudioGraph::setSource(const QString &fileName)
{
    wavePlot->data()->clear();
    samples.clear();
    decoder->setSourceFilename(fileName);
    decoder->start();
}

void AudioGraph::setBuffer()
{
    buffer = decoder->read();
    qreal peak = getPeakValue(buffer.format());
    //std::cout << "peak is " << peak << std::endl;
    if (buffer.format().sampleType() == QAudioFormat::Float)
    {
        const float *fldata = buffer.constData<float>();
        int count = buffer.sampleCount() / 2;
        for (int i=0; i<count; i++){
            double val = fldata[i]/peak;
            samples.append(val);
        }
    }
    else if (buffer.format().sampleType() == QAudioFormat::SignedInt)
    {
        const qint32 *sidata = buffer.constData<qint32>();
        int count = buffer.sampleCount() / 2;
        for (int i=0; i<count; i++){
            double val = sidata[i]/peak;
            samples.append(val);
        }
    }
    else if (buffer.format().sampleType() == QAudioFormat::UnSignedInt)
    {
        const quint32 *udata = buffer.constData<quint32>();
        int count = buffer.sampleCount() / 2;
        for (int i=0; i<count; i++){
            double val = udata[i]/peak;
            samples.append(val);
        }
    }
}

void AudioGraph::plot()
{
    QVector<double> x(samples.size());
    for (int i=0; i<x.size(); i++)
        x[i] = i;//buffer.format().sampleRate();
    wavePlot->addData(x, samples);
    yAxis->setRange(QCPRange(-1, 1));
    xAxis->setRange(QCPRange(0, samples.size()));
    replot();
}

/**
 * https://stackoverflow.com/questions/46947668/draw-waveform-from-raw-data-using-qaudioprobe
 * @brief Track::getPeakValue
 * @param format
 * @return The peak value
 */
qreal AudioGraph::getPeakValue(const QAudioFormat &format)
{
    qreal ret(0);
    if (format.isValid()){
        switch (format.sampleType()) {
            case QAudioFormat::Unknown:
            break;
            case QAudioFormat::Float:
                if (format.sampleSize() != 32) // other sample formats are not supported
                    ret = 0;
                else
                {
                    //float local_max=*std::max_element(buffer.constData<qint16>(),buffer.constData<qint16>()+buffer.sampleCount());
                    ret = 1.00003;//3.402823466e+38;
                }
            break;
            case QAudioFormat::SignedInt:
                if (format.sampleSize() == 32)
#ifdef Q_OS_WIN
                    ret = INT_MAX;
#endif
#ifdef Q_OS_UNIX
                    ret = SHRT_MAX;
#endif
                else if (format.sampleSize() == 16)
                    ret = SHRT_MAX;
                else if (format.sampleSize() == 8)
                    ret = CHAR_MAX;
                break;
            case QAudioFormat::UnSignedInt:
                if (format.sampleSize() == 32)
                    ret = UINT_MAX;
                else if (format.sampleSize() == 16)
                    ret = USHRT_MAX;
                else if (format.sampleSize() == 8)
                    ret = UCHAR_MAX;
            break;
            default:
            break;
        }
    }
    return ret;
}
