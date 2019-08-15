#include "csvlist.h"
#include <QMediaPlayer>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

class vController;
class MainWindow;
#pragma once

class scriptmodel
{
public:
    scriptmodel();
    ~scriptmodel();
    scriptmodel(vController&, MainWindow&);
    void loadWorkingFile(std::string);
    void loadNextLine(list<string>);
    void setAudioDir(string dir);
    void playCurrentAudio();
    void playNextAudio();
    void shiftAudioAlignmentUp();
    void shiftAudioAlignmentDown();
    void replaceAudiofile(string);
    void markCurrentAudioMissing();
    void removeCurrLine();
    void loadProgressFile(string fileName);
    void loadLine(int line, bool forward);
    int loadFile(string filePath, csvlist<string>& targetObject, bool display = true);
    void saveFile(csvlist<string>& targetObject, std::filesystem::path filename,string seperator,bool overwrite);
    void saveProgress(std::filesystem::path filename);
    void saveOrigScript(std::filesystem::path filename);
private:
    void playAudio(filesystem::path a_path, string _audiofile);
    vController *_my_control;
    std::string _audiodir;
    MainWindow *_main_win;
    QMediaPlayer *mediaPlayer;
    csvlist<std::string> _workingList;
    csvlist<std::string> _resultList;
    list<std::string> _currentLine;
    void refreshView(list<string>);
    string audiofile;
};
