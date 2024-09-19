#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <vector>

using namespace std;

const int MAX_COLUMNS = 50;
const int MAX_ROWS = 5000;

void print(string filename, int colNum);
void search(string columns[MAX_ROWS][MAX_COLUMNS], int colWidth, string colName, string val, int numCols, int numRows, string filename);
void printRow(int rowIndex, int numCols, int outputWidth, string columns[5000][50]);
void findMax(string columns[MAX_ROWS][MAX_COLUMNS], string colName, int numCols, int numRows);
void findMin(string columns[MAX_ROWS][MAX_COLUMNS], string colName, int numCols, int numRows);
void findAvg(string columns[MAX_ROWS][MAX_COLUMNS], string colName, int numCols, int numRows);
void cols(string filename);


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: ./a database.csv #col_width" << endl;
        return 0;
    }
    string input;
    string filename = argv[1];
    ifstream file(argv[1]);
    string columns[MAX_ROWS][MAX_COLUMNS];
    int colNum = atoi(argv[2]);
    int numRows = 0;
    int numCols = 0;

    if (!file.is_open()) {
        cout << "Failed to open \"" << argv[1] << "\"" << endl;
        return 0;
    }
    getline(file, input);
    stringstream iss(input);
    

    while (getline(iss, columns[0][numCols], ',')) {
        numCols++;
        // cout << "Test 2" << endl;
    }
    getline(iss, columns[0][numCols], ',');
    numCols++;
    string row;
    numRows = 1;
    while (getline(file, row)) {
        istringstream info(row);
        for (int i = 0; i < numCols; i++) {
            getline(info, columns[numRows][i], ',');
        }
        numRows++;
    }

    bool bContinue = 0;

    while (bContinue == 0) {
        cout << "Enter a command or \"help\" for a command list:" << endl;
        getline(cin, input);
        stringstream iss(input);
        string cmd;
        string colName;
        string val;
        iss >> quoted(cmd);

        if (cmd == "quit") {
            bContinue = 1;
        }
        else if (cmd == "help") {
            cout << "command list:" << endl;
            cout << "        print" << endl;
            cout << "        cols" << endl;
            cout << "        search *|col_name val" << endl;
            cout << "        min col_name" << endl;
            cout << "        max col_name" << endl;
            cout << "        avg col_name" << endl;
        }
        else if (cmd == "print") {
            print(filename, colNum);
        }
        else if (cmd == "cols") {
            cols(filename);
        }
        else if (cmd == "search") {
            iss >> quoted(colName) >> quoted(val);
            // cout << "Test 1 "  << numRows << " , " << numCols << endl;
            search(columns, colNum, colName, val, numCols, numRows, filename);
        }
        else if (cmd == "min") {
            iss >> quoted(colName) >> quoted(val);
            findMin(columns, colName, numCols, numRows);
        }
        else if (cmd == "max") {
            // cout << "Test 1" << endl;
            iss >> quoted(colName) >> quoted(val);
            findMax(columns, colName, numCols, numRows);
            // cout << "Test 2" << endl;
        }
        else if (cmd == "avg") {
            iss >> quoted(colName) >> quoted(val);
            findAvg(columns, colName, numCols, numRows);
        }
        else {
            cout << "Invalid command" << endl;
        }
        cout << endl;
    }
}

void cols(string filename) {
    ifstream file(filename);
    string input;
    getline(file, input);
    stringstream ss(input);
    string field;
    vector<string> fields;
    while (getline(ss, field, ',')) {
        fields.push_back(field);
    }
    

    for (int i = 0; i < fields.size(); i++) {
        cout << fields[i] << endl;
    }
    
    file.close();
}

void onlyFirstRow (string filename, int colWidth, int rowIndex) {
    ifstream file;
    file.open(filename);
    string line, token;
    int count = 0;
    while (getline(file, line)) {
        if (count == rowIndex) {
            istringstream iss(line);
            while (getline(iss, token, ',')) {
                cout << setw(colWidth) << token;;
            }
        }
        count++;
    }
    cout << endl;
    
    file.close();
}

void print(string filename, int colNum) {
    ifstream csv;
    csv.open(filename);
    string line, token;
    while (getline(csv, line)) {
        istringstream iss(line);

        while (getline(iss, token, ',')) {
            cout << setw(colNum) << token;
        }
        if (!line.empty() && line.back() == ',') {
            cout << setw(colNum) << ' ';
        }
        cout << endl;
    }
    // Write code later to add whitespace to end if last column is empty
    csv.close();
}

void search(string columns[MAX_ROWS][MAX_COLUMNS], int colWidth, string colName, string val, int numCols, int numRows, string filename) {
    int numLoops = 0;
    bool found = false;
    bool anyVals = false;
    int colIndex = -1;

    for (int i = 0; i < numCols; i++) {
        if (columns[0][i] == colName) {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1 && colName != "*") {
        cout << "Invalid column \"" << colName << "\"" << endl;
        return;
    }
    
    for (int index = 1; index < numRows; index++) {
        found = false;

        // Checks if the val exists in the cols or nah
        if (colName == "*") {
            for (int i = 0; i < numCols; i++) {
                if (columns[index][i] == val) {
                    found = true;
                    anyVals = true;
                    break;
                }
            }
            if (found) {
                while (numLoops == 0) {
                    onlyFirstRow(filename, colWidth, 0);
                    numLoops = 1;
                }
                onlyFirstRow(filename, colWidth, index);
            }
        }
        else {
            for (int i = 0; i < numCols; i++) {
                if (columns[0][i] == colName && columns[index][i] == val) {
                    found = true;
                    anyVals = true;
                    break;
                }
            }
            if (found) {
                while (numLoops == 0) {
                    onlyFirstRow(filename, colWidth, 0);
                    numLoops = 1;
                }
                onlyFirstRow(filename, colWidth, index);
            }
        }
    }

    // Prints "No results" if no result is ever found while searching
    if (!anyVals) {
        cout << "No results" << endl;
    }
}

// void printRow(int rowIndex, int numCols, int outputWidth, string columns[MAX_ROWS][MAX_COLUMNS]) {
//     cout << "Test row" << endl;
//     for (int i = 0; i < numCols; i++) {
//         cout << setw(outputWidth) << columns[rowIndex][i];
//     }
//     cout << endl;
// }

void findMin(string columns[MAX_ROWS][MAX_COLUMNS], string colName, int numCols, int numRows) {
    int colIndex = -1;
    double minVal = 1000000;
    bool found = false;

    // Finds the index of the column
    for (int i = 0; i < numCols; i++) {
        if (columns[0][i] == colName) {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1) {
        cout << "Invalid column \"" << colName << "\"" << endl;
        return;
    }

    // Finds the min val of the rows
    for (int i = 1; i < numRows; i++) {
        if (!columns[i][colIndex].empty()) {
            double currentVal = stod(columns[i][colIndex]);
            if (currentVal < minVal) {
                minVal = currentVal;
                found = true;
            }
        }
    }

    if (found) {
        cout << "The min for col \"" << colName << "\" = " << minVal << endl;
    }
    else {
        cout << "The min for col \"" << colName << "\" = N/A" << endl;
    }
}

void findMax(string columns[MAX_ROWS][MAX_COLUMNS], string colName, int numCols, int numRows) {
    int colIndex = -1;
    double maxVal = -1000000;
    bool found = false;

    // Finds the index of the column (Copied from findMin)
    for (int i = 0; i < numCols; i++) {
        if (columns[0][i] == colName) {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1) {
        cout << "Invalid column \"" << colName << "\"" << endl;
        return;
    }

    // Finds the max val of the rows (Copied from findMin)
    for (int i = 1; i < numRows; i++) {
        if (!columns[i][colIndex].empty()) {
            double currentVal = stod(columns[i][colIndex]);
            if (currentVal > maxVal) {
                maxVal = currentVal;
                found = true;
            }
        }
    }

    if (found) {
        cout << "The max for col \"" << colName << "\" = " << maxVal << endl;
    }
    else {
        cout << "The max for col \"" << colName << "\" = N/A" << endl;
    }
}

void findAvg(string columns[MAX_ROWS][MAX_COLUMNS], string colName, int numCols, int numRows) {
    int colIndex = -1;
    double total = 0;
    double avg = 0;
    bool found = false;
    int divisor = 0;

    // Finds the index of the column (Copied from findMin)
    for (int i = 0; i < numCols; i++) {
        if (columns[0][i] == colName) {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1) {
        cout << "Invalid column \"" << colName << "\"" << endl;
        return;
    }

    // Finds the avg val of the rows (Copied from findMin)
    for (int i = 1; i < numRows; i++) {
        if (!columns[i][colIndex].empty()) {
            double currentVal = stod(columns[i][colIndex]);
            total += currentVal;
            found = true;
            divisor++;
        }
        if (i == numRows - 1) {
            avg = total / divisor;
        }
    }

    if (found) {
        cout << "The avg for col \"" << colName << "\" = " << avg << endl;
    }
    else {
        cout << "The avg for col \"" << colName << "\" = N/A" << endl;
    }
}