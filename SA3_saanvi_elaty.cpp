#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <set>
using namespace std;

int main(int argc, char* argv[]) {
    // Case 1
    if (argc < 2) {
        cerr << "NO SPECIFIED INPUT FILE NAME." << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);

    // Case 2
    if (!inputFile.is_open()) {
        cerr << "CANNOT OPEN THE FILE " << argv[1] << endl;
        return 1;
    }

    // Case 3
    if (inputFile.peek() == EOF) {
        cerr << "The File is Empty." << endl;
        return 0;
    }

    bool countKeywords = false;
    bool countSpecialWords = false;
    bool countIdentifiers = false;
    set<string> keywordSet = {"begin", "end", "if", "else", "while", "for", "break", "continue", "case", "switch", "class", "public", "private", "abstract", "final"};
    for (int i = 2; i < argc; i++) {
        string argument = argv[i];
        if (argument == "-kw") {
            countKeywords = true;
        } else if (argument == "-sp") {
            countSpecialWords = true;
        } else if (argument == "-id") {
            countIdentifiers = true;
        }
    }

    int totalWordCount = 0;
    int keywordCount = 0;
    int identifierCount = 0;
    int specialDollarCount = 0;
    int specialAtCount = 0;
    int specialPercentCount = 0;

    int currentLineNumber = 0;
    string currentLine;

    while (getline(inputFile, currentLine)) {
        currentLineNumber++;
        istringstream lineStream(currentLine);
        string currentWord;

        while (lineStream >> currentWord) {
            totalWordCount++;


            if (countSpecialWords && (currentWord[0] == '$' || currentWord[0] == '@' || currentWord[0] == '%')) {
                bool isValidSpecialWord = true;
                for (size_t i = 1; i < currentWord.size(); i++) {
                    if (!isalnum(currentWord[i]) && currentWord[i] != '_') {
                        isValidSpecialWord = false;
                        break;
                    }
                }
                if (isValidSpecialWord) {
                    if (currentWord[0] == '$') specialDollarCount++;
                    else if (currentWord[0] == '@') specialAtCount++;
                    else if (currentWord[0] == '%') specialPercentCount++;
                } else {
                    cerr << "Invalid Special Word at line " << currentLineNumber << ": " << currentWord << endl;
                }
                continue;
            }

            string lowercaseWord = currentWord;
            transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);

            
            if (countKeywords && keywordSet.count(lowercaseWord)) {
                keywordCount++;
                continue; 
            }

            
            if (countIdentifiers && isalpha(currentWord[0])) {
                bool isValidIdentifier = true;
                for (size_t i = 1; i < currentWord.size(); i++) {
                    if (!isalnum(currentWord[i])) {
                        isValidIdentifier = false;
                        break;
                    }
                }
                if (isValidIdentifier && !keywordSet.count(lowercaseWord)) { 
                    identifierCount++;
                } else {
                    cerr << "Invalid Identifier Word at line " << currentLineNumber << ": " << currentWord << endl;
                }
            }
        }
    }

    
    cout << "Total number of words: " << totalWordCount << endl;
    if (countKeywords) {
        cout << "Number of Keywords: " << keywordCount << endl;
    }
    if (countIdentifiers) {
        cout << "Number of Identifiers: " << identifierCount << endl;
    }
    if (countSpecialWords) {
        cout << "Number of Special Words Starting with $: " << specialDollarCount << endl;
        cout << "Number of Special Words Starting with @: " << specialAtCount << endl;
        cout << "Number of Special Words Starting with %: " << specialPercentCount << endl;
    }

    inputFile.close();
    return 0;
}