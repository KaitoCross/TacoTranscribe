
#include <QMainWindow>
#include "vcontroller.h"
#pragma once


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addListener(vController  &controller);
    void setJpnText(std::string jpnText);
    void setRomajiView(std::string romaji);
    void setRomajiEdit(std::string romaji);
    void setAudiofileLabel(std::string file);
    void enableAudioBtns();
    void enableTextBtns();

private slots:
    void on_replayBtn_clicked();

    void on_actionLoad_Script_triggered();

    void on_actionLoad_Audio_triggered();

    void on_actionLoad_Savept_triggered();

    void on_NextBtn_clicked();

    void on_playNextBtn_clicked();

    void on_shiftNegBut_clicked();

    void on_shiftPosBut_clicked();

    void on_reassignAudioBtn_clicked();

    void on_audioMissingBtn_clicked();

    void on_audioUsuitedSkip_clicked();

    void on_actionSaveScript_triggered();

    void on_actionSaveSavepoint_triggered();

private:
    vController * _controller;
    Ui::MainWindow *ui;
};