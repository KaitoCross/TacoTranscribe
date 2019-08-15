class scriptmodel;
#pragma once
class MainWindow;
#include <string>
#include <filesystem>

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
private:
    MainWindow* _window;
    scriptmodel* _scriptModel;
    std::string _audiodir;
    std::filesystem::path savedProgressFile;
    std::filesystem::path savedScriptFile;
};
