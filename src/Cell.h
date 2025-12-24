//cell.h
#pragma once

#include <set>

class Cell {
public:
    Cell();
    
    void setValue(int value);
    int getValue() const;
    bool isEmpty() const;
    bool isGiven() const;
    void setGiven(bool given);
    
    void addNote(int value);
    void removeNote(int value);
    void clearNotes();
    bool hasNote(int value) const;
    const std::set<int>& getNotes() const;
    
    void setError(bool error);
    bool hasError() const;
    
private:
    int value_;           // 0 means empty
    bool isGiven_;        // true if this was part of the initial puzzle
    std::set<int> notes_; // pencil marks
    bool hasError_;
};

