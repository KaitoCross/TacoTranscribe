class scriptmodel;
#pragma once
class MainWindow;
#include <string>
#include <filesystem>
#include <QObject>

class vController
{
public:
    vController();
    vController(scriptmodel&, MainWindow&);
    void onLoadOrigScript();
    void onNextLine(std::string,std::string,std::string);
    void onLoadAudioDir();
    void onReplay();
    void onNextAudioBtn();
    void onShiftUpBtn();
    void onShiftDownBtn();
    void onReassignAudio();
    void onMarkMissing();
    void onUnsuitableAudio();
    void onLoadSavedProgress();
    void onSave(bool progress);
    void jumpInAudio(qint64 position);
private:
    MainWindow* _window;
    scriptmodel* _scriptModel;
    std::string _audiodir;
    std::filesystem::path savedProgressFile;
    short savepoint_file_data_std;
    std::filesystem::path savedScriptFile;
    short shortened_script_file_data_std;
    std::string loadTextFile(QString dia_title, short *data_std);
    std::filesystem::path saveFileDialogue(QString dialogueTitle, short *data_std);
};
