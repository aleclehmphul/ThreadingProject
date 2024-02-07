#include "ArrayListString.h"


ArrayListString::ArrayListString() {
    list = nullptr;
    size = 0;
    maxSize = 0;
}

ArrayListString::ArrayListString(int newSize) {
    list = new std::string[newSize];
    size = 0;
    maxSize = newSize;
}

// Adding entry to list.
// Makes a new list of twice the size of the old list.
// Copies all entries from old list, then adds new element to end.
void ArrayListString::addEntry(std::string line) {

    if (size == maxSize) {
        maxSize = maxSize * 2;
        std::string* newList = new std::string[maxSize];

        for (int i = 0; i < size; i++) {
            newList[i] = list[i];
        }

        delete[] list;

        newList[size] = line;

        // Update instance variables to point to new array
        list = newList;
        size = size + 1;
    }
    else {
        list[size] = line;
        size = size + 1;
    }
}

std::string ArrayListString::getEntry(int index) {
    if (index < size && index > -1)
        return list[index];

    return nullptr;
}

int ArrayListString::getSize() {
    return size;
}

std::string* ArrayListString::getList() {
    return list;
}

