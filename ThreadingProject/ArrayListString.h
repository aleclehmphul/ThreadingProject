#include <string>

class ArrayListString {
    int size;
    int maxSize;
    std::string* list;

public:
    ArrayListString();
    ArrayListString(int);

    void addEntry(std::string);
    std::string getEntry(int);
    int getSize();
    std::string* getList();
};
