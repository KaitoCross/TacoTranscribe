#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mahslidesyle = new MySliderStyle("sliderfix");
    errDialog = QErrorMessage::qtHandler();
}

MainWindow::~MainWindow()
{
    delete mahslidesyle;
    delete ui;
}

void MainWindow::on_replayBtn_clicked()
{
_controller->onReplay();
}

void MainWindow::on_actionLoad_Script_triggered()
{
 _controller->onLoadOrigScript();
}

void MainWindow::on_actionLoad_Audio_triggered()
{
_controller->onLoadAudioDir();
}

void MainWindow::on_actionLoad_Savept_triggered()
{
_controller->onLoadSavedProgress();
}

void MainWindow::setJpnText(std::string jpnText)
{
    const QString qJpnText = QString::fromStdString(jpnText);
    QPlainTextEdit* jpnTextField = ui->jpnTextDisplay;
    jpnTextField->setPlainText(qJpnText);
}

void MainWindow::setRomajiView(std::string romaji)
{
    const QString qromaji = QString::fromStdString(romaji);
    ui->engTextDisplay->setPlainText(qromaji);
}

void MainWindow::setRomajiEdit(std::string romaji)
{
    const QString qromaji = QString::fromStdString(romaji);
    ui->newEngText->setPlainText(qromaji);
}

void MainWindow::setAudiofileLabel(std::string file)
{
    ui->OggFileName->setText(QString::fromStdString(file));
}

void MainWindow::enableAudioBtns()
{
    ui->replayBtn->setEnabled(true);
    ui->playNextBtn->setEnabled(true);
    if (mahslidesyle != nullptr)
    {
        ui->horizontalSlider->setStyle(mahslidesyle);
    }
}

void MainWindow::enableTextBtns()
{
    ui->NextBtn->setEnabled(true);
    ui->shiftNegBut->setEnabled(true);
    ui->shiftPosBut->setEnabled(true);
    ui->audioMissingBtn->setEnabled(true);
    ui->reassignAudioBtn->setEnabled(true);
    ui->audioUsuitedSkip->setEnabled(true);
}

void MainWindow::updateProgBar(int max, int current)
{
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(current);
}

void MainWindow::drawAudio(QString file)
{
    ui->audioGraphWidget->setSource(file);
    //ui->audioGraphWidget->setBuffer();
    //ui->audioGraphWidget->plot();
}

void MainWindow::showErrMsg(const QString &message, const QString &type)
{
    errDialog->showMessage(message,type);
}

void MainWindow::on_position_change(int position)
{
    //std::cout << position << std::endl;
    ui->horizontalSlider->setSliderPosition(position);
    ui->audioGraphWidget->plotProgress(position);
}

void MainWindow::on_duration_change(int duration)
{
    ui->horizontalSlider->setMaximum(duration);
}

void MainWindow::audioError(QMediaPlayer::Error error)
{
    short errnr=0;
    std::string errmsg="";
    switch (error){
    case QMediaPlayer::NoError: errmsg="No error has occurred.";
        break;
    case QMediaPlayer::ResourceError: errmsg="The media resource couldn't be resolved.";
        errnr=1;
        break;
    case QMediaPlayer::FormatError: errmsg="The format of a media resource isn't supported.";
        errnr=2;
        break;
    case QMediaPlayer::AccessDeniedError: errmsg="There are not the appropriate permissions to play the media resource.";
        errnr=3;
        break;
    case QMediaPlayer::ServiceMissingError: errmsg="A valid playback service was not found, playback cannot proceed.";
        errnr=4;
        break;
    case QMediaPlayer::MediaIsPlaylist: errmsg="Media resource is a playlist.";
        errnr=4;
        break;
    case QMediaPlayer::NetworkError: errmsg="Network Error.";
        errnr=4;
        break;
    }
    QString errstring = QString::fromStdString("Error Playback! Error no. "+std::to_string(errnr) + " " +errmsg);
    errDialog->showMessage(errstring,QString::fromStdString(errmsg));
    //setAudiofileLabel(errstring);
}

void MainWindow::addListener(vController &controller)
{
 _controller = &controller;
}

void MainWindow::on_NextBtn_clicked()
{
_controller->onNextLine(ui->jpnTextDisplay->toPlainText().toStdString(), ui->newEngText->toPlainText().toStdString(),ui->OggFileName->text().toStdString());
}

void MainWindow::on_playNextBtn_clicked()
{
    _controller->onNextAudioBtn();
}

void MainWindow::on_shiftNegBut_clicked()
{
_controller->onShiftDownBtn();
}

void MainWindow::on_shiftPosBut_clicked()
{
_controller->onShiftUpBtn();
}

void MainWindow::on_reassignAudioBtn_clicked()
{
_controller->onReassignAudio();
}

void MainWindow::on_audioMissingBtn_clicked()
{
    _controller->onMarkMissing();
}

void MainWindow::on_audioUsuitedSkip_clicked()
{
    _controller->onUnsuitableAudio();
}

void MainWindow::on_actionSaveScript_triggered()
{
    _controller->onSave(false);
}

void MainWindow::on_actionSaveSavepoint_triggered()
{
    _controller->onSave(true);
}


void MainWindow::on_horizontalSlider_sliderReleased()
{
    _controller->jumpInAudio(ui->horizontalSlider->sliderPosition());
}

void MainWindow::on_audioGraphWidget_plottableClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event)
{
    if (event->type()==QEvent::MouseButtonRelease)
    {
        double samples_ms = ui->audioGraphWidget->getSampleRate()/1000.0;
        qint64 newPos = (dataIndex*2)/samples_ms;
        _controller->jumpInAudio(newPos);
    }
}
