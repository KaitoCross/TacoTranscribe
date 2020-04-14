class scriptmodel;
#pragma once
class MainWindow;
#include <string>
#include <QObject>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1

#ifdef __unix__
#include <filesystem>
#define filesystem_os_specific std::filesystem

#elif defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64)
#include <experimental/filesystem>
#define filesystem_os_specific std::experimental::filesystem

#endif


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
    void displayError(const QString & message, const QString & type);
private:
    MainWindow* _window;
    scriptmodel* _scriptModel;
    std::string _audiodir;
    filesystem_os_specific::path savedProgressFile;
    short savepoint_file_data_std;
    filesystem_os_specific::path savedScriptFile;
    short shortened_script_file_data_std;
    bool unexpected_redraw;
    std::string loadTextFile(QString dia_title, short *data_std);
    filesystem_os_specific::path saveFileDialogue(QString dialogueTitle, short *data_std);
};
