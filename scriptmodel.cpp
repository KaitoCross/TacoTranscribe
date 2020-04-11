
#include "scriptmodel.h"
#include "mainwindow.h"
#include <iostream>

scriptmodel::scriptmodel()
{
    _workingList = csvlist<string>(3);
    _resultList = csvlist<string>(3);
    _audiodir="";
    audiofile="";
    _currentLine = list<string>();
    mediaPlayer = new QMediaPlayer();
    //cout << "notifyInterval " <<mediaPlayer->notifyInterval() << endl;
    mediaPlayer->setNotifyInterval(100);
    //cout << "notifyInterval " <<mediaPlayer->notifyInterval() << endl;
}

scriptmodel::~scriptmodel()
{
    delete mediaPlayer;
}

scriptmodel::scriptmodel(vController& vcont, MainWindow& mwin)
{
    _workingList = csvlist<string>(3);
    _resultList = csvlist<string>(3);
    _my_control = &vcont;
    _main_win = &mwin;
    _audiodir="";
    audiofile="";
    _currentLine = list<string>();
    mediaPlayer = new QMediaPlayer();
    _main_win->updateProgBar(100,0);
    QObject::connect(mediaPlayer, &QMediaPlayer::positionChanged,&mwin,&MainWindow::on_position_change);
    QObject::connect(mediaPlayer, &QMediaPlayer::durationChanged,&mwin,&MainWindow::on_duration_change);
}

void scriptmodel::loadWorkingFile(string filePath, short *data_std)
{
    //_workingList = csvlist<string>(3);
    loadFile(filePath,_workingList,true,data_std);
    enableButtons();
}

void scriptmodel::loadNextLine(list<string> args)
{
    _resultList.push_back(args);
    _currentLine =_workingList.getline();
    refreshView(_currentLine);
}

void scriptmodel::setAudioDir(string dir)
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

void scriptmodel::playCurrentAudio(qint64 position, bool redraw, bool jump)
{
    filesystem::path a_path = filesystem::path(_audiodir);
    a_path.append(audiofile);
    //std::cout<<audiofile << " is the audio"<<endl;
    if (jump)
        mediaPlayer->pause();
    playAudio(a_path, audiofile, position,redraw);
}

void scriptmodel::playNextAudio()
{
    filesystem::path a_path = filesystem::path(_audiodir);
    string next_audio;
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
    refreshView(list<string>());
}

void scriptmodel::shiftAudioAlignmentDown()
{
    _workingList.moveColContentDown(3,1);
    refreshView(list<string>());
}

void scriptmodel::replaceAudiofile(string n_audiofile)
{
    audiofile = n_audiofile;
    _workingList.editRowColContent(3,_workingList.getReadPos()-1,audiofile);
    refreshView(list<string>());
}

void scriptmodel::markCurrentAudioMissing()
{
    audiofile = "#missingaudio#";
    _workingList.editRowColContent(3,_workingList.getReadPos()-1,audiofile);
    refreshView(list<string>());
}

void scriptmodel::removeCurrLine()
{
    _workingList.deleteRow(_workingList.getReadPos()-1);
    refreshView(list<string>());
}

void scriptmodel::loadProgressFile(string fileName, short *data_standard)
{
    int lines_loaded = loadFile(fileName,_resultList,true,data_standard);
    loadLine(lines_loaded, true);
}

void scriptmodel::loadLine(int line = -1, bool forward = false)
{
    _currentLine =_workingList.getline(line, forward);
    refreshView(_currentLine);
}

int scriptmodel::loadFile(string filePath, csvlist<string>& targetObject, bool display, short *data_std)
{
    targetObject.clear();
    fstream work_f;
    work_f.open(filePath, ios::in);
    list<string> row;
    string word, line, temp;
    int lines = 0;
    if (*data_std == 1) //nonstandard dataset edit
    {
        while(getline(work_f,line))
        {
            row.clear();
            stringstream s(line);
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
            stringstream s(line);
            while(getline(s,word,'|'))
            {
                row.push_back(word);
            }
            string audioName = row.front();
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

void scriptmodel::saveFile(csvlist<string> &targetObject, filesystem::path filename, string seperator, bool overwrite, short *data_std)
{
    ofstream savefile;
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
            list<string> line = targetObject.getline(i);
            auto line_col_it = line.begin();
            while(line_col_it != line.end())
            {
                string temp = *line_col_it;
                savefile<< temp;
                line_col_it++;
                if (line_col_it != line.end())
                {
                    savefile << seperator;
                }
                else {
                    savefile << endl;
                }
            }
            targetObject.setWritePos(i+1);
        }
    }
    if (*data_std == 0)
    {
        for (;i <= targetObject.getMaxRows()-1;i++) {
            list<string> line = targetObject.getline(i);
            string audio_temp = line.back();
            line.pop_back();
            line.push_front(audio_temp);
            auto line_col_it = line.begin();
            while(line_col_it != line.end())
            {
                string temp = *line_col_it;
                savefile<< temp;
                line_col_it++;
                if (line_col_it != line.end())
                {
                    savefile << seperator;
                }
                else {
                    savefile << endl;
                }
            }
            targetObject.setWritePos(i+1);
        }
    }
    savefile.close();
}

void scriptmodel::saveProgress(filesystem::path filename, short* data_std)
{
    saveFile(_resultList,filename,"|",false,data_std);
}

void scriptmodel::saveOrigScript(filesystem::path filename, short* data_std)
{
    saveFile(_workingList,filename,"|",true,data_std);
}

void scriptmodel::playAudio(filesystem::path a_path, string _audiofile, qint64 position, bool redraw)
{
    if (filesystem::exists(a_path))
    {
        QString tmp_path = QString::fromStdString(a_path.string());
        mediaPlayer->setMedia(QUrl::fromLocalFile(tmp_path));
        //cout << "notifyInterval " <<mediaPlayer->notifyInterval() << endl;
        mediaPlayer->setNotifyInterval(20);
        //cout << "notifyInterval " <<mediaPlayer->notifyInterval() << endl;
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

void scriptmodel::refreshView(list<string> firstLine = list<string>())
{
    if (firstLine.empty())
        //firstLine = _workingList.getRow(_workingList.getCurrentReadPos());
        firstLine = _workingList[_workingList.getReadPos()];
    string jpnText = firstLine.front();
    _main_win->setJpnText(jpnText);
    firstLine.pop_front();
    string romaji = firstLine.front();
    _main_win->setRomajiView(romaji);
    _main_win->setRomajiEdit(romaji);
    firstLine.pop_front();
    audiofile = firstLine.front();
    _main_win->setAudiofileLabel(audiofile);
    playCurrentAudio(0,true);
    _main_win->updateProgBar(_workingList.getMaxRows(),_workingList.getReadPos()-1);
}
