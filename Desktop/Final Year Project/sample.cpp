#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

struct KeyValuePair {
    int Key;
    char Value;
};

bool compare(const KeyValuePair &first, const KeyValuePair &second) {
    return first.Value < second.Value;
}

vector<vector<char>> create2DArray(int rowCount, int colCount) {
    return vector<vector<char>>(rowCount, vector<char>(colCount));
}

string padRight(string str, int max, char padChar) {
    int strLen = str.length();

    if (strLen < max) {
        int padLen = max - strLen;
        str += string(padLen, padChar);
    }

    return str;
}

vector<int> getShiftIndexes(const string &key) {
    int keyLength = key.length();
    vector<KeyValuePair> sortedKey;

    for (int i = 0; i < keyLength; ++i)
        sortedKey.push_back({i, key[i]});

    sort(sortedKey.begin(), sortedKey.end(), compare);

    vector<int> indexes(keyLength);

    for (const auto &pair : sortedKey)
        indexes[pair.Key] = pair.Key;

    return indexes;
}

string encipher(const string &input, const string &key, char padChar) {
    string output = "";
    int totalChars = input.length();
    int keyLength = key.length();
    string paddedInput = (totalChars % keyLength == 0) ? input : padRight(input, totalChars - (totalChars % keyLength) + keyLength, padChar);
    totalChars = paddedInput.length();
    int totalColumns = keyLength;
    int totalRows = (totalChars + totalColumns - 1) / totalColumns;
    auto rowChars = create2DArray(totalRows, totalColumns);
    auto colChars = create2DArray(totalColumns, totalRows);
    auto sortedColChars = create2DArray(totalColumns, totalRows);
    int currentRow, currentColumn, i, j;
    auto shiftIndexes = getShiftIndexes(key);

    for (i = 0; i < totalChars; ++i) {
        currentRow = i / totalColumns;
        currentColumn = i % totalColumns;
        rowChars[currentRow][currentColumn] = paddedInput[i];
    }

    for (i = 0; i < totalRows; ++i)
        for (j = 0; j < totalColumns; ++j)
            colChars[j][i] = rowChars[i][j];

    for (i = 0; i < totalColumns; ++i)
        for (j = 0; j < totalRows; ++j)
            sortedColChars[shiftIndexes[i]][j] = colChars[i][j];

    for (i = 0; i < totalChars; ++i) {
        currentRow = i / totalRows;
        currentColumn = i % totalRows;
        output += sortedColChars[currentRow][currentColumn];
    }

    return output;
}

string decipher(const string &input, const string &key) {
    string output = "";
    int keyLength = key.length();
    int totalChars = input.length();
    int totalColumns = (totalChars + keyLength - 1) / keyLength;
    int totalRows = keyLength;
    auto rowChars = create2DArray(totalRows, totalColumns);
    auto colChars = create2DArray(totalColumns, totalRows);
    auto unsortedColChars = create2DArray(totalColumns, totalRows);
    int currentRow, currentColumn, i, j;
    auto shiftIndexes = getShiftIndexes(key);

    for (i = 0; i < totalChars; ++i) {
        currentRow = i / totalColumns;
        currentColumn = i % totalColumns;
        rowChars[currentRow][currentColumn] = input[i];
    }

    for (i = 0; i < totalRows; ++i)
        for (j = 0; j < totalColumns; ++j)
            colChars[j][i] = rowChars[i][j];

    for (i = 0; i < totalColumns; ++i)
        for (j = 0; j < totalRows; ++j)
            unsortedColChars[i][j] = colChars[i][shiftIndexes[j]];

    for (i = 0; i < totalChars; ++i) {
        currentRow = i / totalRows;
        currentColumn = i % totalRows;
        output += unsortedColChars[currentRow][currentColumn];
    }

    return output;
}

int main() {
    string input, key;
    char padChar = 'X';

    cout << "Enter the plaintext: ";
    getline(cin, input);

    cout << "Enter the key: ";
    getline(cin, key);

    string encrypted = encipher(input, key, padChar);
    string decrypted = decipher(encrypted, key);

    cout << "Original: " << input << endl;
    cout << "Encrypted: " << encrypted << endl;
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}
