#include "vcontroller.h"
#include "scriptmodel.h"
#include <QString>
#include <QFileDialog>

vController::vController()
{

}

vController::vController(scriptmodel& lemodel, MainWindow& lawindow)
{
_scriptModel = &lemodel;
_window = &lawindow;
}

void vController::onLoadOrigScript()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,
           "Open Voice Line File", "",
           "Text Files (*.txt);;All Files (*)");
    std::string test = fileName.toStdString();
    if (!test.empty())
        _scriptModel->loadWorkingFile(test);

}

void vController::onNextLine(string japLine, string editedLine, string audiofile)
{
    list<string> args;
    args.push_back(japLine);
    args.push_back(editedLine);
    args.push_back(audiofile);
    _scriptModel->loadNextLine(args);
    onSave(true);
}

void vController::onLoadAudioDir()
{
    QString fileName = QFileDialog::getExistingDirectory();
    _audiodir = fileName.toStdString();
    if (!_audiodir.empty())
        _scriptModel->setAudioDir(_audiodir);
}

void vController::onReplay()
{
    _scriptModel->playCurrentAudio(0);
}

void vController::onNextAudioBtn()
{
    _scriptModel->playNextAudio();
}

void vController::onShiftUpBtn()
{
    _scriptModel->shiftAudioAlignmentUp();
}

void vController::onShiftDownBtn()
{
    _scriptModel->shiftAudioAlignmentDown();
}

void vController::onReassignAudio()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,
           "Open Voice File", "",
           "Audio Files (*.ogg,*.wav,*.mp3);;All Files (*)");
    std::string s_path = fileName.toStdString();
    if (!s_path.empty())
    {
        filesystem::path realpath(s_path);
        _scriptModel->replaceAudiofile(realpath.filename().string());
    }
    //_scriptModel->loadWorkingFile(test);
}

void vController::onMarkMissing()
{
    _scriptModel->markCurrentAudioMissing();
}

void vController::onUnsuitableAudio()
{
    _scriptModel->removeCurrLine();
}

void vController::onLoadSavedProgress()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,
           "Open Voice Line File", "",
           "Text Files (*.txt);;All Files (*)");
    std::string test = fileName.toStdString();
    if (!test.empty())
        _scriptModel->loadProgressFile(test);
}

void vController::onSave(bool progress)
{
    if (progress)
    {
        if (savedProgressFile.empty())
        {
            QString fileName = QFileDialog::getSaveFileName(nullptr,
                   "Save edit progress in file", "",
                   "Text Files (*.txt);;All Files (*)");
            savedProgressFile = std::filesystem::path(fileName.toStdString());
        }
        _scriptModel->saveProgress(savedProgressFile);
    }
    else {
        if (savedScriptFile.empty())
        {
            QString fileName = QFileDialog::getSaveFileName(nullptr,
                   "Save working copy of Voice Line File", "",
                   "Text Files (*.txt);;All Files (*)");
            savedScriptFile = std::filesystem::path(fileName.toStdString());
        }
        _scriptModel->saveOrigScript(savedScriptFile);
    }
}

void vController::jumpInAudio(qint64 position)
{
    _scriptModel->playCurrentAudio(position);
}
