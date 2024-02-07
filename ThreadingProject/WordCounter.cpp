#include "WordCounter.h"

WordCounter::WordCounter() {
	wordList = nullptr;
	countList = nullptr;
	size = 0;
}

WordCounter::WordCounter(int startingSize) {
	size = 0;
	maxSize = startingSize;

	wordList = new std::string[maxSize];
	countList = new int[maxSize];
}


// Adds a new word to wordList and adds a linked countList entry (set to 1)
// If arrays are at maxSize, they double in size before adding to list.
void WordCounter::addWord(std::string word) {

	if (size == maxSize) {
		maxSize = maxSize * 2;

		std::string* newWordList = new std::string[maxSize];
		int* newCountList = new int[maxSize];

		for (int i = 0; i < size; i++) {
			newWordList[i] = wordList[i];
			newCountList[i] = countList[i];
		}

		delete[] wordList;
		delete[] countList;

		newWordList[size] = word;
		newCountList[size] = 1;

		// Update instance variables to point to new array
		wordList = newWordList;
		countList = newCountList;
		size = size + 1;
	}
	else {
		wordList[size] = word;
		countList[size] = 1;
		size = size + 1;
	}

}

// Returns index of word if found.  Else returns -1.
int WordCounter::contains(std::string word) {
	
	for (int i = 0; i < size; i++) {
		if (wordList[i].compare(word) == 0)
			return i;
	}

	return -1;
}

// Increments the count at a specified index
void WordCounter::incrementCount(int index) {
	countList[index] = countList[index] + 1;
}

int WordCounter::getSize() {
	return size;
}

std::string WordCounter::getWord(int index) {
	return wordList[index];
}

int WordCounter::getCount(int index) {
	return countList[index];
}


void swap(int* counts, std::string* words, int i, int j) {
	int tempCount = counts[i];
	counts[i] = counts[j];
	counts[j] = tempCount;

	std::string tempWord = words[i];
	words[i] = words[j];
	words[j] = tempWord;
}

void bubbleSort(int counts[], std::string words[], int size) {
	bool swapped;
	do {
		swapped = false;
		for (int i = 0; i < size - 1; i++) {
			if (counts[i] < counts[i + 1]) {
				swap(counts, words, i, i + 1);
				swapped = true;
			}
		}
		size--;
	} while (swapped);
}

void WordCounter::sortLists() {
	bubbleSort(countList, wordList, size);
}

