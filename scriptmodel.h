#include "csvlist.h"
#include <QMediaPlayer>
#include <QObject>
#include <fstream>
#include <string>
#include <sstream>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1

#ifdef __unix__
#include <filesystem>
#define filesystem_os_specific std::filesystem

#elif defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64)
#include <experimental/filesystem>
#define filesystem_os_specific std::experimental::filesystem

#endif

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
    void loadNextLine(std::list<std::string>);
    void setAudioDir(std::string dir);
    void playCurrentAudio(qint64 position = 0, bool redraw = false, bool jump = false);
    void playNextAudio();
    void shiftAudioAlignmentUp();
    void shiftAudioAlignmentDown();
    void replaceAudiofile(std::string);
    void markCurrentAudioMissing();
    void removeCurrLine();
    void loadProgressFile(std::string fileName, short *data_standard);
    void loadLine(int line, bool forward);
    int loadFile(std::string filePath, csvlist<std::string>& targetObject, bool display = true, short *data_std = nullptr);
    void saveFile(csvlist<std::string>& targetObject, filesystem_os_specific::path filename,std::string seperator,bool overwrite,short *data_std);
    void saveProgress(filesystem_os_specific::path filename,short *data_std);
    void saveOrigScript(filesystem_os_specific::path filename,short *data_std);
    void enableButtons();

public slots:
    void audioError();

private:
    void playAudio(filesystem_os_specific::path a_path, std::string _audiofile, qint64 position = 0, bool redraw = false);
    vController *_my_control;
    std::string _audiodir;
    MainWindow *_main_win;
    QMediaPlayer *mediaPlayer;
    csvlist<std::string> _workingList;
    csvlist<std::string> _resultList;
    std::list<std::string> _currentLine;
    void refreshView(std::list<std::string>);
    std::string audiofile;
};
