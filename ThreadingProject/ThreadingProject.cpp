// ThreadingProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <chrono>

// Two allowed header files needed to get Memory Consumption
// These libraries are only used for that purpose
#include <windows.h>
#include <psapi.h>

// My own created helper classes
#include "ArrayListString.h"
#include "WordCounter.h"

#define MAX_THREADS 1
#define IN_FILENAME "lyrics.txt"
#define OUT_FILENAME "output_lyrics.txt"


ArrayListString fileLines = ArrayListString(32);
WordCounter wordCounter = WordCounter(32);
std::mutex mtx;

/* Manually converts a string to lowercase */
std::string toLowercase(std::string str) {
    std::string strLowercase;

    for (char c : str) {
        if (c >= 'A' && c <= 'Z') {
            strLowercase += (c - 'A' + 'a');
        }
        else {
            strLowercase += c;
        }
    }

    return strLowercase;
}

/* Reads file line by line */
void readFile(std::string filename) {
    std::ifstream inFile;
    inFile.open(filename);

    std::string line, lowercaseLine;

    while (getline(inFile, line)) {
        lowercaseLine = toLowercase(line);

        fileLines.addEntry(lowercaseLine);
    }

    inFile.close();
}


/* Checks if the character is an escape sequence character */
bool isEscapeSequence(char c) {
    return c == '\n' || c == '\t' || c == '\r'
        || c == '\\' || c == '\"'
        || c == '\?' || c == '\0';
}


/* Illegal characters are spaces, digits, punctuation, and escape characters */
bool isIllegalCharacter(char c, char previous, char next) {
    // For cases of apostrophe in words like they're OR I'll
    // For cases of hyphen in words like jury-men OR co-worker
    if ((c == '\'' || c == '-') && (previous >= 'a' && previous <= 'z')
            && ((next >= 'a' && next <= 'z'))) {
        return false;
    }

    return isspace(c) || isdigit(c) || ispunct(c) || isEscapeSequence(c);
}

/* Critical function for threads, counting words */
void countWord(std::string word) {
    mtx.lock();

    int index = wordCounter.contains(word);

    if (index == -1) {
        wordCounter.addWord(word);
    }
    else {
        wordCounter.incrementCount(index);
    }

    mtx.unlock();
}

void countWords(int startIndex, int endIndex) {
    std::string line, word;

    for (int i = startIndex; i < endIndex; i++) {
        line = fileLines.getEntry(i);
        char previous = ' ', next;
        int position = -1;

        for (char c : line) {
            if (position >= line.size())
                next = ' ';
            else {
                next = line[position];
            }

            if (isIllegalCharacter(c, previous, next)) {
                if (!word.empty()) {
                    countWord(word);

                    word = "";
                }
            }
            else {
                word += c;
            }
            previous = c;
            position++;
        }
        if (!word.empty()) {
            countWord(word);

            word = "";
        }
    }
}


/* Generates output.txt file displaying word and count */
static void generateOutput(const char* filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        fprintf(stderr, "Error opening output file!\n");
    }

    for (int i = 0; i < wordCounter.getSize(); i++) {
        std::string spaceForFormat;

        if (wordCounter.getWord(i).length() >= 16)
            spaceForFormat = "\t";
        else if (wordCounter.getWord(i).length() >= 8) {
            spaceForFormat = "\t\t";
        }
        else {
            spaceForFormat = "\t\t\t";
        }

        outFile << wordCounter.getWord(i) << spaceForFormat << wordCounter.getCount(i) << std::endl;
    }

    outFile.close();
}


void getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pMemoryCounter;
    HANDLE myProcess = GetCurrentProcess();

    if (GetProcessMemoryInfo(myProcess, &pMemoryCounter, sizeof(pMemoryCounter))) {
        std::cout << "Memomy Consumption --> Peak working set size: " << pMemoryCounter.PeakWorkingSetSize / 1024 << " KB" << std::endl;
    }
    else {
        std::cerr << "Failed to get process memory information." << std::endl;
    }
}


int main()
{
    auto start = std::chrono::steady_clock::now();

    std::cout << "Number of threads = " << MAX_THREADS << std::endl << std::endl;

    std::cout << "Begin reading input file " << IN_FILENAME << "." << std::endl;
    readFile(IN_FILENAME);
    std::cout << "Finished reading file." << std::endl;

    int limit = fileLines.getSize() / MAX_THREADS;

    std::cout << "Begin getting word count results." << std::endl;
    countWords(0, fileLines.getSize());
    std::cout << "Finish getting word count results." << std::endl;


    std::cout << "Begin sorting results." << std::endl;
    wordCounter.sortLists();
    std::cout << "Finished sorting results." << std::endl;

    std::cout << "Begin generating " << OUT_FILENAME << " file." << std::endl;
    generateOutput(OUT_FILENAME);
    std::cout << "Finished generating " << OUT_FILENAME <<" file." << std::endl;
    std::cout << "\nProgram finished. Results can be found within the " << OUT_FILENAME << " file." << std::endl;

    int sum = 0;
    for (int i = 0; i < wordCounter.getSize(); i++) {
        sum += wordCounter.getCount(i);
    }

    std::cout << "\nTotal words in input file: " << sum << std::endl;

    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "\nProgram runtime: " << milliseconds << " milliseconds" << std::endl;

    getMemoryUsage();
}
