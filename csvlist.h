#ifndef CSVLIST_H
#define CSVLIST_H
#include <list>
#include <string>

using namespace std;

template <typename T>
class csvlist
{
public:
    csvlist<T>()
    {
        _maxSize = 1;
        for (int var = 0; var < _maxSize; var++) {
            _colList.push_back(list<T>());
            _colListPoppedFromTop.push_back(list<T>());
            _colListPoppedFromBottom.push_back(list<T>());
        }
        currentReadPos = getRowStartIterators(_colList);
    }
    csvlist<T>(int cols)
    {
        _maxSize = cols;
        for (int var = 0; var < _maxSize; var++) {
            _colList.push_back(list<T>());
            _colListPoppedFromTop.push_back(list<T>());
            _colListPoppedFromBottom.push_back(list<T>());
        }
        currentReadPos = getRowStartIterators(_colList);
    }
    void push_back(list<T> newContentLine)
    {
         _maxRows++;
         push_back(newContentLine,_colList);

    }
    void moveColContentDown(int col, int n)
    {
        for (int j = 0;j<n;j++) {
            auto iter = _colList.begin();
            for (int i = 0 ; (iter != _colList.end() && i < col-1); i++, iter++) {
            }
            T moveme = iter->back();
            iter->pop_back();
            auto _backbufferiter = _colListPoppedFromBottom.begin();
            for (int i = 0 ; (_backbufferiter != _colListPoppedFromBottom.end() && i < col-1); i++, iter++) {
            }
            _backbufferiter->push_front(moveme);
            auto _topbufferiter = _colListPoppedFromTop.begin();
            for (int i = 0 ; (_topbufferiter != _colListPoppedFromTop.end() && i < col-1); i++, iter++) {
            }
            T nachruecker;
            if (_topbufferiter->empty())
            {
                nachruecker = T();
            }
            else {
                nachruecker = _topbufferiter->back();
                _topbufferiter->pop_back();
            }
            auto InsertBeforeIter = iter->begin();
            for (int l = 0;l < _readPos-1 && InsertBeforeIter != iter->end(); InsertBeforeIter++, l++) {
            }
            iter->insert(InsertBeforeIter,nachruecker);
        }
    }
    void moveColContentUp(int col, int n)
    {
        for (int j = 0;j<n;j++) {
            auto iter = _colList.begin();
            for (int i = 0 ; (iter != _colList.end() && i < col-1); i++, iter++) {
            }
            //T moveme = iter->front();
            //iter->pop_front();
            auto RemoveAtIter = iter->begin();
            for (int l = 0;l < _readPos-1 && RemoveAtIter != iter->end(); RemoveAtIter++, l++) {
            }
            T moveme = *RemoveAtIter;
            iter->erase(RemoveAtIter);
            auto _topbufferiter = _colListPoppedFromTop.begin();
            for (int i = 0 ; (_topbufferiter != _colListPoppedFromTop.end() && i < col-1); i++, iter++) {
            }
            _topbufferiter->push_back(moveme);
            auto _backbufferiter = _colListPoppedFromBottom.begin();
            for (int i = 0 ; (_backbufferiter != _colListPoppedFromTop.end() && i < col-1); i++, iter++) {
            }
            T nachruecker;
            if (_backbufferiter->empty())
            {
                nachruecker = T();
            }
            else {
                nachruecker = _backbufferiter->front();
                _backbufferiter->pop_front();
            }
            iter->push_back(nachruecker);
        }
    }

    void editRowColContent(int col,int row,T newvalue)
    {
        auto coliter = _colList.begin();
        auto currentReadPosChanging = currentReadPos.begin();

        for (int i = 0 ; (coliter != _colList.end() && i < col-1 && currentReadPosChanging != currentReadPos.end()) ; i++, coliter++, currentReadPosChanging++) {
        }
        auto row_iter = coliter->begin();
        for (auto j = 0; (row_iter != coliter->end() && j < row); j++, row_iter++) {
        }
        auto insertIntoIter = coliter->erase(row_iter);
        auto new_ele_it = coliter->insert(insertIntoIter,newvalue);
        auto erasetemp = currentReadPos.erase(currentReadPosChanging);
        currentReadPos.insert(erasetemp,new_ele_it);
        //*row_iter = newvalue;
    }

    list<T> getline(int line = -1, bool forward=false)
    {
        list<T> result;
        if (line == -1)
        {
            result = getRow(_readPos,&_colList,0);
            //list<T> result = getRow(&currentReadPos);
            _readPos++;
        }
        else {
            result = getRow(line,&_colList,0);
            if (forward)
                _readPos = line;
        }
        return result;
    }
    list<T> getRow(int row, list<list<T>>* source, int correct = -1)
    {
        list<T> result;
        list<typename list<T>::iterator> iter_list = getRowStartIterators(*source);
        list<typename list<T>::iterator> iter_end_list = getRowEndIterators(*source);
        for (auto s_iters = iter_list.begin(), e_iters = iter_end_list.begin() ; s_iters != iter_list.end() && e_iters != iter_end_list.end() ; s_iters++, e_iters++) {
            auto sublist_iter = *s_iters;
            auto sublist_iter_end = *e_iters;
            int le_dist = distance(sublist_iter,sublist_iter_end);
            if (le_dist > row+correct)
            {
                advance(sublist_iter,row+correct);
                result.push_back(*sublist_iter);
            }
            else {
                result.push_back(T());
            }
        }
        return result;
    }
    list<T> getRow(list<typename list<T>::iterator> *rowAddresses)
    {
        list<T> result;
        list<typename list<T>::iterator> *iter_list = rowAddresses;
        list<typename list<T>::iterator> iter_end_list = getRowEndIterators(_colList);
        list<typename list<T>::iterator> newAddresses;
        for (auto s_iters = iter_list->begin(), e_iters = iter_end_list.begin() ; s_iters != iter_list->end() && e_iters != iter_end_list.end() ; s_iters++, e_iters++) {
            auto sublist_iter = *s_iters;
            auto sublist_iter_end = *e_iters;
            if (sublist_iter != sublist_iter_end)
            {
                T entry = *sublist_iter;
                result.push_back(entry);
                sublist_iter++;
                newAddresses.push_back(sublist_iter);
            }
        }
        currentReadPos = newAddresses;
        return result;
    }
    void clear()
    {
        if (!_colList.empty())
        {
            _readPos=0;
            _writePos=0;
            _maxRows=0;
            for (auto iter = _colList.begin();iter != _colList.end();iter++) {
                iter->clear();
            }
            currentReadPos = getRowStartIterators(_colList);
        }
        if (!_colListPoppedFromTop.empty())
        {
            for (auto iter = _colListPoppedFromTop.begin();iter != _colListPoppedFromTop.end();iter++) {
                iter->clear();
            }
        }
        if (!_colListPoppedFromBottom.empty())
        {
            for (auto iter = _colListPoppedFromTop.begin();iter != _colListPoppedFromTop.end();iter++) {
                iter->clear();
            }
        }
    }
    list<string> getCsvList(string);
    list<string> getCsvList(int, string);
    int getReadPos()
    {
        return _readPos;
    }
    list<T> operator[](int idx)
    {
        return getRow(idx, &_colList);
    }
    void deleteRow(int n, bool refreshReadPos = true)
    {
        list<typename list<T>::iterator> newReadPos;
        auto ita = _colList.begin();
        for (;ita != _colList.end(); ita++) {
            auto rowIt = ita->begin();
            for (auto i = 0; i < n && rowIt != ita->end(); i++, rowIt++) {
            }
            auto newPos = ita->erase(rowIt);
            if (refreshReadPos)
                newReadPos.push_back(newPos);
        }
        _maxRows--;
        if (refreshReadPos)
            currentReadPos=newReadPos;
    }
    /*void deleteCurrentRow(bool refreshReadPos = true)
    {
        list<typename list<T>::iterator> newReadPos;
        auto ita = _colList.begin();
        for (auto colIt = currentReadPos.begin(); ita != _colList.end() && colIt != currentReadPos.end() ; ita++, colIt++) {
            auto rowIt = *colIt;
            auto newPos = ita->erase(rowIt);
            if (refreshReadPos)
                newReadPos.push_back(newPos);
        }
        _maxRows--;
        if (refreshReadPos)
            currentReadPos=newReadPos;
    }*/
    list<typename list<T>::iterator> *getCurrentReadPos()
    {
        return &currentReadPos;
    }
    int getMaxRows()
    {
        return _maxRows;
    }
    int getWritePos()
    {
        return _writePos;
    }
    void setWritePos(int pos)
    {
        _writePos = pos;
    }
private:
    list<typename list<T>::iterator> getRowStartIterators(list<list<T>>& target_list)
    {
        auto ita = target_list.begin();
        list<typename list<T>::iterator> iter_list;
        for (;ita != target_list.end();ita++) {
            iter_list.push_back(ita->begin());
        }
        return iter_list;
    }
    list<typename list<T>::iterator> getRowEndIterators(list<list<T>>& target_list)
    {
        auto ita = target_list.begin();
        list<typename list<T>::iterator> iter_list;
        for (;ita != target_list.end();ita++) {
            iter_list.push_back(ita->end());
        }
        return iter_list;
    }
    void push_back(list<T>& newContentLine, list<list<T>>& target_list)
    {
     if (newContentLine.size()>= target_list.size())
     {
         auto col = target_list.begin();
         for (auto item = newContentLine.begin(); col != target_list.end(); col++, item++) {
             col->push_back(T(*item));
         }
     }
     currentReadPos = getRowStartIterators(_colList);
    }
    list<list<T>> _colListPoppedFromTop;
    list<list<T>> _colList;
    list<list<T>> _colListPoppedFromBottom;
    list<typename list<T>::iterator> currentReadPos;
    list<typename list<T>::iterator> currentWritePos;
    int _readPos = 0;
    int _writePos = 0;
    int _maxSize=0;
    int _maxRows=0;
};

#endif // CSVLIST_H
