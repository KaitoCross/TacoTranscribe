#include "csvlist.h"
#include <QMediaPlayer>
#include <QObject>
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
    void loadWorkingFile(std::string, short *data_std);
    void loadNextLine(list<string>);
    void setAudioDir(string dir);
    void playCurrentAudio(qint64 position = 0, bool redraw = false, bool jump = false);
    void playNextAudio();
    void shiftAudioAlignmentUp();
    void shiftAudioAlignmentDown();
    void replaceAudiofile(string);
    void markCurrentAudioMissing();
    void removeCurrLine();
    void loadProgressFile(string fileName, short *data_standard);
    void loadLine(int line, bool forward);
    int loadFile(string filePath, csvlist<string>& targetObject, bool display = true, short *data_std = nullptr);
    void saveFile(csvlist<string>& targetObject, std::filesystem::path filename,string seperator,bool overwrite,short *data_std);
    void saveProgress(std::filesystem::path filename,short *data_std);
    void saveOrigScript(std::filesystem::path filename,short *data_std);
    void enableButtons();
private:
    void playAudio(filesystem::path a_path, string _audiofile, qint64 position = 0, bool redraw = false);
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
