//cell.cpp
#include "Cell.h"

Cell::Cell() : value_(0), isGiven_(false), hasError_(false) {
}

void Cell::setValue(int value) {
    value_ = value;
    if (value != 0) {
        clearNotes();
    }
}

int Cell::getValue() const {
    return value_;
}

bool Cell::isEmpty() const {
    return value_ == 0;
}

bool Cell::isGiven() const {
    return isGiven_;
}

void Cell::setGiven(bool given) {
    isGiven_ = given;
}

void Cell::addNote(int value) {
    if (value >= 1 && value <= 9 && isEmpty()) {
        notes_.insert(value);
    }
}

void Cell::removeNote(int value) {
    notes_.erase(value);
}

void Cell::clearNotes() {
    notes_.clear();
}

bool Cell::hasNote(int value) const {
    return notes_.find(value) != notes_.end();
}

const std::set<int>& Cell::getNotes() const {
    return notes_;
}

void Cell::setError(bool error) {
    hasError_ = error;
}

bool Cell::hasError() const {
    return hasError_;
}

