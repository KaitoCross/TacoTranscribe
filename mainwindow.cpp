#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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
