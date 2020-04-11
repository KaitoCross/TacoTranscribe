#include "vcontroller.h"
#include "scriptmodel.h"
#include <QString>
#include <QFileDialog>
#include <iostream>

#define SUPPORTEDFILETYPES "LJSpeech-compliant Text Files (*.txt);;LJSpeech-compliant CSV (*.csv);;All LJSpeech-compliant Files (*);;non-compliant Text Files (*.txt);;non-compliant CSV (*.csv);;All non-compliant Files (*)"

vController::vController()
{

}

vController::vController(scriptmodel& lemodel, MainWindow& lawindow)
{
_scriptModel = &lemodel;
_window = &lawindow;
savepoint_file_data_std = 0;
shortened_script_file_data_std = 0;
}

void vController::onLoadOrigScript()
{
    short data_standard = 0;
    string script_filename = loadTextFile("Load Base Voice Line Script",&data_standard);
    if (!script_filename.empty())
        _scriptModel->loadWorkingFile(script_filename,&data_standard);
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
    _scriptModel->playCurrentAudio(0,false);
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
    short data_standard = 0;
    string savepoint_filename = loadTextFile("Load Savepoint File",&data_standard);
    if (!savepoint_filename.empty())
        _scriptModel->loadProgressFile(savepoint_filename, &data_standard);
}

void vController::onSave(bool progress)
{
    if (progress)
    {
        if (savedProgressFile.empty())
        {
            /*QString fileName = QFileDialog::getSaveFileName(nullptr,
                   "Save edit progress in file", "",
                   "Text Files (*.txt);;All Files (*)");
            savedProgressFile = std::filesystem::path(fileName.toStdString());*/
            savedProgressFile = saveFileDialogue("Save edit progress in file",&savepoint_file_data_std);
        }
        _scriptModel->saveProgress(savedProgressFile,&savepoint_file_data_std);
    }
    else {
        if (savedScriptFile.empty())
        {
            /*QString fileName = QFileDialog::getSaveFileName(nullptr,
                   "Save working copy of Voice Line File", "",
                   "Text Files (*.txt);;All Files (*)");
            savedScriptFile = std::filesystem::path(fileName.toStdString());*/
            savedScriptFile = saveFileDialogue("Save working copy of Voice Line File",&shortened_script_file_data_std);
        }
        _scriptModel->saveOrigScript(savedScriptFile,&shortened_script_file_data_std);
    }
}

void vController::jumpInAudio(qint64 position)
{
    _scriptModel->playCurrentAudio(position,false,true);
}

string vController::loadTextFile(QString dia_title, short *data_std)
{
    QString chosenFilter = "";
    //*data_std = 0; //Default LJSpeech-compliant data. 1 = noncompliant
    QString fileName = QFileDialog::getOpenFileName(nullptr,
           dia_title, "",
           SUPPORTEDFILETYPES,&chosenFilter);
    std::string script_filename = fileName.toStdString();
    if (chosenFilter != "")
    {
        std::cout << "chosenFilter is " << chosenFilter.toStdString() << endl;
        if (chosenFilter.contains("non-compliant"))
        {
            *data_std = 1;
            //std::cout << "chosenFilter is detected noncompliant" << endl;
        }
    }
    return script_filename;
}

std::filesystem::path vController::saveFileDialogue(QString dialogueTitle, short *data_std)
{
    QString chosenFilter = "";
    QString fileName = QFileDialog::getSaveFileName(nullptr,
           dialogueTitle, "",
           SUPPORTEDFILETYPES,&chosenFilter);
    if (chosenFilter != "")
    {
        //std::cout << "chosenFilter is " << chosenFilter.toStdString() << endl;
        if (chosenFilter.contains("non-compliant"))
        {
            *data_std = 1;
            std::cout << "chosenFilter is detected noncompliant" << endl;
        }
    }
    return std::filesystem::path(fileName.toStdString());
}
