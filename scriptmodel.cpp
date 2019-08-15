
#include "scriptmodel.h"
#include "mainwindow.h"

scriptmodel::scriptmodel()
{
_workingList = csvlist<string>(3);
_resultList = csvlist<string>(3);
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
}

void scriptmodel::loadWorkingFile(string filePath)
{
    //_workingList = csvlist<string>(3);
    loadFile(filePath,_workingList);
    _main_win->enableTextBtns();
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
        playCurrentAudio();
    }
    _main_win->enableAudioBtns();
}



void scriptmodel::playCurrentAudio()
{
    filesystem::path a_path = filesystem::path(_audiodir);
    a_path.append(audiofile);
    playAudio(a_path, audiofile);
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
    playAudio(a_path, next_audio);
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

void scriptmodel::loadProgressFile(string fileName)
{
    int lines_loaded = loadFile(fileName,_resultList);
    loadLine(lines_loaded, true);
}

void scriptmodel::loadLine(int line = -1, bool forward = false)
{
    _currentLine =_workingList.getline(line, forward);
    refreshView(_currentLine);
}

int scriptmodel::loadFile(string filePath, csvlist<string>& targetObject, bool display)
{
    targetObject.clear();
    fstream work_f;
    work_f.open(filePath, ios::in);
    list<string> row;
    string word, line, temp;
    int lines = 0;
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
    if (display)
    {
        //_currentLine =_workingList.getline();
        loadLine();
    }
    return lines;
}

void scriptmodel::saveFile(csvlist<string> &targetObject, filesystem::path filename, string seperator, bool overwrite = false)
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
    savefile.close();
}

void scriptmodel::saveProgress(filesystem::path filename)
{
    saveFile(_resultList,filename,"|");
}

void scriptmodel::saveOrigScript(filesystem::path filename)
{
    saveFile(_workingList,filename,"|",true);
}

void scriptmodel::playAudio(filesystem::path a_path, string _audiofile)
{
    if (filesystem::exists(a_path))
    {
        mediaPlayer->setMedia(QUrl::fromLocalFile(QString::fromStdString(a_path.string())));
        mediaPlayer->play();
        _main_win->setAudiofileLabel(_audiofile);
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
    playCurrentAudio();
}
