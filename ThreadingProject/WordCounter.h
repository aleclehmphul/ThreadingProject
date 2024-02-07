#include <string>

class WordCounter {
	std::string* wordList;
	int* countList;
	int size;
	int maxSize;

public:
	WordCounter();
	WordCounter(int);

	void addWord(std::string);
	int contains(std::string);
	void incrementCount(int);
	int getSize();
	std::string getWord(int);
	int getCount(int);
	void sortLists();
};

