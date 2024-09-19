#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;

unsigned long djb2(const string& str) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

class HashTable {
private:
    int totWords = 0;
    int uniqueWords = 0;
    int totCollisions = 0;
    int size = 0;
    vector<string>* table;

public:
    HashTable(int size) : size(size) {
        table = new vector<string>[size];
    }

    ~HashTable() {
        delete[] table;
    }

    int getTotalWords() const {
        return totWords;
    }

    int getUniqueWords() const {
        return uniqueWords;
    }

    int getTotalCollisions() const {
        return totCollisions;
    }

    void hashInsert(const string& input, int lineNum, const string& probe) {
        unsigned long hash = djb2(input);
        int bucket = hash % size;
        int i = 1; // Linear probing increment

        while (!table[bucket].empty() && table[bucket][0] != input) {
            if (probe == "lp") {
                bucket = (bucket + 1) % size;
            } 
            else if (probe == "qp") {
                bucket = (hash + (i * i)) % size;
            }
            totCollisions++;
            i++;
        }

        if (!table[bucket].empty() && table[bucket][0] == input) { // If word already exists
            table[bucket].push_back(to_string(lineNum));
            totWords++;
        }
        else { // Word does not exist
            table[bucket].push_back(input);
            table[bucket].push_back(to_string(lineNum));
            uniqueWords++;
            totWords++;
        }
    }

    void hashInsertDH(const string& input, int lineNum, const string& probe, int a) {
        unsigned long hash = djb2(input);
        int bucket = hash % size;
        int i = 1; // Linear probing increment

        while (!table[bucket].empty() && table[bucket][0] != input) {
            if (probe == "dh") {
                bucket = (hash + i * (a - (hash % a))) % size; // Double hashing, double-check later
            }
            totCollisions++;
            i++;
        }

        if (!table[bucket].empty() && table[bucket][0] == input) { // If word already exists
            table[bucket].push_back(to_string(lineNum));
            totWords++;
        }
        else { // Word does not exist
            table[bucket].push_back(input);
            table[bucket].push_back(to_string(lineNum));
            uniqueWords++;
            totWords++;
        }
    }

    void hashSearch(const string& input, const string& probe) const {
        unsigned long hash = djb2(input);
        int bucket = hash % size;
        int i = 1; // Linear probing increment
        int tempCollisions = 0;

        while (!table[bucket].empty()) {
            if (table[bucket][0] == input) {
                cout << endl;
                cout << input << " appears on lines [";
                for (size_t j = 1; j < table[bucket].size(); ++j) {
                    cout << table[bucket][j];
                    if (j < table[bucket].size() - 1) {
                        cout << ",";
                    }
                }
                cout << "]" << endl;
                cout << "The search had " << tempCollisions << " collisions" << endl;
                return;
            }
            if (probe == "lp") {
                bucket = (bucket + 1) % size;
            } else if (probe == "qp") {
                bucket = (hash + (i * i)) % size;
            }
            tempCollisions++;
            i++;
        }
        cout << endl;
        cout << input << " appears on lines []" << endl << "The search had " << tempCollisions << " collisions" << endl;
    }

    void hashSearchDH(const string& input, const string& probe, int a) const {
        unsigned long hash = djb2(input);
        int bucket = hash % size;
        int i = 1; // Linear probing increment
        int tempCollisions = 0;

        while (!table[bucket].empty()) {
            if (table[bucket][0] == input) {
                cout << endl;
                cout << input << " appears on lines [";
                for (size_t j = 1; j < table[bucket].size(); ++j) {
                    cout << table[bucket][j];
                    if (j < table[bucket].size() - 1) {
                        cout << ",";
                    }
                }
                cout << "]" << endl;
                cout << "The search had " << tempCollisions << " collisions" << endl;
                return;
            }
            if (probe == "dh") {
                bucket = (hash + i * (a - (hash % a))) % size; // Double hashing
            }
            tempCollisions++;
            i++;
        }
        cout << endl;
        cout << input << " appears on lines []" << endl << "The search had " << tempCollisions << " collisions" << endl;
    }
};

int main(int argc, char **argv) {
    // if (argc != 5) {
    //     cerr << "Usage: " << argv[0] << " <input_file> <search_file> <hash_table_size> <probe_type>" << endl;
    //     return 1;
    // }

    HashTable table(stoi(argv[3]));
    ifstream in_file(argv[1]);
    string input;
    int totWords = 0;
    int lineNum = 0;
    string probe = argv[4];

    if (!in_file) {
        cerr << "Error: Unable to open input file " << argv[1] << endl;
        return 1;
    }

    if (probe == "dh") {
        int a = stoi(argv[5]);
        while (getline(in_file, input)) {
            lineNum++;
            string word;
            for (char c : input) {
                if (isalpha(c)) {
                    word += tolower(c);
                } else {
                    if (!word.empty()) {
                        table.hashInsertDH(word, lineNum, probe, a);
                        word.clear();
                        totWords++;
                    }
                }
            }
            if (!word.empty()) {
                table.hashInsertDH(word, lineNum, probe, a);
                word.clear();
                totWords++;
            }
        }
    }

    else {
        while (getline(in_file, input)) {
            lineNum++;
            string word;
            for (char c : input) {
                if (isalpha(c)) {
                    word += tolower(c);
                } else {
                    if (!word.empty()) {
                        table.hashInsert(word, lineNum, probe);
                        word.clear();
                        totWords++;
                    }
                }
            }
            if (!word.empty()) {
                table.hashInsert(word, lineNum, probe);
                word.clear();
                totWords++;
            }
        }
    }

    
    in_file.close();

    cout << "The number of words found in the file was " << totWords << endl;
    cout << "The number of unique words found in the file was " << table.getUniqueWords() << endl;
    cout << "The number of collisions was " << table.getTotalCollisions() << endl;

    ifstream search_file(argv[2]);
    if (!search_file) {
        cerr << "Error: Unable to open search file " << argv[2] << endl;
        return 1;
    }
    if (probe == "dh") {
        int a = stoi(argv[5]);
        while (getline(search_file, input)) {
            table.hashSearchDH(input, probe, a);
        }
    }
    else {
        while (getline(search_file, input)) {
            table.hashSearch(input, probe);
        }
    }
    
    search_file.close();

    return 0;
}
