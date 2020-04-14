
#include "scriptmodel.h"
#include "mainwindow.h"
#include <iostream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1

scriptmodel::scriptmodel()
{
    _workingList = csvlist<std::string>(3);
    _resultList = csvlist<std::string>(3);
    _audiodir="";
    audiofile="";
    _currentLine = std::list<std::string>();
    mediaPlayer = new QMediaPlayer();
    //cout << "notifyInterval " <<mediaPlayer->notifyInterval() << std::endl;
    mediaPlayer->setNotifyInterval(100);
    //cout << "notifyInterval " <<mediaPlayer->notifyInterval() << std::endl;
}

scriptmodel::~scriptmodel()
{
    delete mediaPlayer;
}

scriptmodel::scriptmodel(vController& vcont, MainWindow& mwin)
{
    _workingList = csvlist<std::string>(3);
    _resultList = csvlist<std::string>(3);
    _my_control = &vcont;
    _main_win = &mwin;
    _audiodir="";
    audiofile="";
    _currentLine = std::list<std::string>();
    mediaPlayer = new QMediaPlayer();
    _main_win->updateProgBar(100,0);
    QObject::connect(mediaPlayer, &QMediaPlayer::positionChanged,&mwin,&MainWindow::on_position_change);
    QObject::connect(mediaPlayer, &QMediaPlayer::durationChanged,&mwin,&MainWindow::on_duration_change);
    QObject::connect(mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),&mwin,&MainWindow::audioError);
}

void scriptmodel::loadWorkingFile(std::string filePath, short *data_std)
{
    //_workingList = csvlist<string>(3);
    loadFile(filePath,_workingList,true,data_std);
    enableButtons();
}

void scriptmodel::loadNextLine(std::list<std::string> args)
{
    _resultList.push_back(args);
    _currentLine =_workingList.getline();
    refreshView(_currentLine);
}

void scriptmodel::setAudioDir(std::string dir)
{
    _audiodir = dir;
    if (!_currentLine.empty())
    {
        playCurrentAudio(0,true);
    }
    enableButtons();
}

void scriptmodel::enableButtons()
{
    if ((_audiodir != "") && (_workingList.getMaxRows() > 0))
    {
        _main_win->enableAudioBtns();
        _main_win->enableTextBtns();
    }
}

void scriptmodel::audioError()
{
    std::cout<<"Audio Error"<<std::endl;
}

void scriptmodel::playCurrentAudio(qint64 position, bool redraw, bool jump)
{
    filesystem_os_specific::path a_path = filesystem_os_specific::path(_audiodir);
    a_path.append(audiofile);
    //std::cout<<audiofile << " is the audio"<<std::endl;
    if (jump)
        mediaPlayer->pause();
    playAudio(a_path, audiofile, position,redraw);
}

void scriptmodel::playNextAudio()
{
    filesystem_os_specific::path a_path = filesystem_os_specific::path(_audiodir);
    std::string next_audio;
    auto next_line = _workingList[_workingList.getReadPos()];
    if (!next_line.empty())
    {
        next_audio = next_line.back();
        a_path.append(next_audio);
        playAudio(a_path, next_audio,0,false);
    }
}

void scriptmodel::shiftAudioAlignmentUp()
{
    _workingList.moveColContentUp(3,1);
    refreshView(std::list<std::string>());
}

void scriptmodel::shiftAudioAlignmentDown()
{
    _workingList.moveColContentDown(3,1);
    refreshView(std::list<std::string>());
}

void scriptmodel::replaceAudiofile(std::string n_audiofile)
{
    audiofile = n_audiofile;
    _workingList.editRowColContent(3,_workingList.getReadPos()-1,audiofile);
    refreshView(std::list<std::string>());
}

void scriptmodel::markCurrentAudioMissing()
{
    audiofile = "#missingaudio#";
    _workingList.editRowColContent(3,_workingList.getReadPos()-1,audiofile);
    refreshView(std::list<std::string>());
}

void scriptmodel::removeCurrLine()
{
    _workingList.deleteRow(_workingList.getReadPos()-1);
    refreshView(std::list<std::string>());
}

void scriptmodel::loadProgressFile(std::string fileName, short *data_standard)
{
    int lines_loaded = loadFile(fileName,_resultList,true,data_standard);
    loadLine(lines_loaded, true);
}

void scriptmodel::loadLine(int line = -1, bool forward = false)
{
    _currentLine =_workingList.getline(line, forward);
    refreshView(_currentLine);
}

int scriptmodel::loadFile(std::string filePath, csvlist<std::string>& targetObject, bool display, short *data_std)
{
    targetObject.clear();
    std::fstream work_f;
    work_f.open(filePath, std::ios::in);
    std::list<std::string> row;
    std::string word, line, temp;
    int lines = 0;
    if (*data_std == 1) //nonstandard dataset edit
    {
        while(getline(work_f,line))
        {
            row.clear();
            std::stringstream s(line);
            while(getline(s,word,'|'))
            {
                row.push_back(word);
            }
            targetObject.push_back(row);
            lines++;
        }
    }
    if (*data_std == 0) //LJSpeech-like dataset.
    {
        while(getline(work_f,line))
        {
            row.clear();
            std::stringstream s(line);
            while(getline(s,word,'|'))
            {
                row.push_back(word);
            }
            std::string audioName = row.front();
            //LJSpeech dataset does not save the audio file extension (.wav, .mp3) in their csv file
            //We require them to be in the dataset!
            //uncommented line would remove the file extension
            //audioName = audioName.substr(0,audioName.find_last_of("."));
            row.pop_front();
            row.push_back(audioName);
            targetObject.push_back(row);
            lines++;
        }
    }
    if (display)
    {
        //_currentLine =_workingList.getline();
        loadLine();
    }
    return lines;
}

void scriptmodel::saveFile(csvlist<std::string> &targetObject, filesystem_os_specific::path filename, std::string seperator, bool overwrite, short *data_std)
{
    std::ofstream savefile;
    int i = 0;
    if (overwrite)
    {
        savefile.open(filename,std::ofstream::out | std::ofstream::trunc);
    }
    else
    {
        savefile.open(filename,std::ofstream::out | std::ofstream::app);
        i = targetObject.getWritePos();
    }
    if (*data_std == 1)
    {
        for (;i <= targetObject.getMaxRows()-1;i++) {
            std::list<std::string> line = targetObject.getline(i);
            auto line_col_it = line.begin();
            while(line_col_it != line.end())
            {
                std::string temp = *line_col_it;
                savefile<< temp;
                line_col_it++;
                if (line_col_it != line.end())
                {
                    savefile << seperator;
                }
                else {
                    savefile << std::endl;
                }
            }
            targetObject.setWritePos(i+1);
        }
    }
    if (*data_std == 0)
    {
        for (;i <= targetObject.getMaxRows()-1;i++) {
            std::list<std::string> line = targetObject.getline(i);
            std::string audio_temp = line.back();
            line.pop_back();
            line.push_front(audio_temp);
            auto line_col_it = line.begin();
            while(line_col_it != line.end())
            {
                std::string temp = *line_col_it;
                savefile<< temp;
                line_col_it++;
                if (line_col_it != line.end())
                {
                    savefile << seperator;
                }
                else {
                    savefile << std::endl;
                }
            }
            targetObject.setWritePos(i+1);
        }
    }
    savefile.close();
}

void scriptmodel::saveProgress(filesystem_os_specific::path filename, short* data_std)
{
    saveFile(_resultList,filename,"|",false,data_std);
}

void scriptmodel::saveOrigScript(filesystem_os_specific::path filename, short* data_std)
{
    saveFile(_workingList,filename,"|",true,data_std);
}

void scriptmodel::playAudio(filesystem_os_specific::path a_path, std::string _audiofile, qint64 position, bool redraw)
{
    if (filesystem_os_specific::exists(a_path))
    {
        QString tmp_path = QString::fromStdString(a_path.string());
        mediaPlayer->setMedia(QUrl::fromLocalFile(tmp_path));
        //cout << "notifyInterval " <<mediaPlayer->notifyInterval() << std::endl;
        mediaPlayer->setNotifyInterval(20);
        //cout << "notifyInterval " <<mediaPlayer->notifyInterval() << std::endl;
        mediaPlayer->setPosition(position);
        mediaPlayer->play();
        _main_win->setAudiofileLabel(_audiofile);
        if (redraw)
        {
            _main_win->drawAudio(tmp_path);
        }
    }
    else {
        if (_audiofile=="#missingaudio#")
            _main_win->setAudiofileLabel("Audiofile marked as missing");
        else
            _main_win->setAudiofileLabel("Audiofile not found! "+a_path.string());
    }
}

void scriptmodel::refreshView(std::list<std::string> firstLine = std::list<std::string>())
{
    if (firstLine.empty())
        //firstLine = _workingList.getRow(_workingList.getCurrentReadPos());
        firstLine = _workingList[_workingList.getReadPos()-1];
    std::string jpnText = firstLine.front();
    _main_win->setJpnText(jpnText);
    firstLine.pop_front();
    std::string romaji = firstLine.front();
    _main_win->setRomajiView(romaji);
    _main_win->setRomajiEdit(romaji);
    firstLine.pop_front();
    audiofile = firstLine.front();
    _main_win->setAudiofileLabel(audiofile);
    playCurrentAudio(0,true);
    _main_win->updateProgBar(_workingList.getMaxRows(),_workingList.getReadPos()-1);
}
