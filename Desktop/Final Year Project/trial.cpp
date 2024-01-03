#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to remove spaces from a string
string removeSpaces(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (ch != ' ') {
            result += ch;
        }
    }
    return result;
}

// Function to perform columnar transposition encryption
string encryptCipher(const string& plaintext, const string& key) {
    // Remove spaces from the plaintext
    string cleanedText = removeSpaces(plaintext);

    // Determine the number of columns based on the key length
    int columns = key.length();

    // Determine the number of rows required
    int rows = (cleanedText.length() + columns - 1) / columns;

    // Create a 2D matrix to store the characters
    vector<vector<char>> matrix(rows, vector<char>(columns, ' '));

    // Fill the matrix with characters from the cleaned plain text
    for (int i = 0, k = 0; i < rows; ++i) {
        for (int j = 0; j < columns && k < cleanedText.length(); ++j) {
            matrix[i][j] = cleanedText[k++];
        }
    }

    // Display the characters according to the order of the key
    vector<int> order(columns);
    for (int i = 0; i < columns; ++i) {
        order[i] = i;
    }

    // Sort the order based on the key using Lambda function
    sort(order.begin(), order.end(), [&key](int a, int b) {
        return key[a] < key[b];
    });

    // Build the cipher text by reading columns in the order specified by the key
    string ciphertext;
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            ciphertext += matrix[j][order[i]];
        }
    }

    return ciphertext;
}

// Function to perform columnar transposition decryption
string decryptCipher(const string& ciphertext, const string& key) {
    // Determine the number of columns based on the key length
    int columns = key.length();

    // Determine the number of rows required
    int rows = (ciphertext.length() + columns - 1) / columns;

    // Create a 2D matrix to store the characters
    vector<vector<char>> matrix(rows, vector<char>(columns, ' '));

    // Create a permutation order based on the key
    vector<int> order(columns);
    for (int i = 0; i < columns; ++i) {
        order[i] = i;
    }

    // Sort the order based on the key
    sort(order.begin(), order.end(), [&key](int a, int b) {
        return key[a] < key[b];
    });

    // Fill the matrix with characters from the ciphertext based on the order
    int k = 0;
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            matrix[j][order[i]] = ciphertext[k++];
        }
    }

    // Build the cleaned plaintext by reading rows
    string cleanedPlaintext;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cleanedPlaintext += matrix[i][j];
        }
    }

    return cleanedPlaintext;
}

int main() {
    string inputText, key;
    int choice;

    // Get input text, key, and user choice
    cout << "Enter the text: ";
    getline(cin >> ws, inputText);

    cout << "Enter the key: ";
    cin >> key;

    cout << "Choose an option:\n1. Encrypt\n2. Decrypt\n";
    cin >> choice;

    if (choice == 1) {
        // Encrypt the input text
        string ciphertext = encryptCipher(inputText, key);
        cout << "Encrypted Text: " << ciphertext << endl;
    } else if (choice == 2) {
        // Decrypt the input text
        string decryptedText = decryptCipher(inputText, key);
        cout << "Decrypted Text: " << decryptedText << endl;
    } else {
        cout << "Invalid choice. Please choose 1 for encryption or 2 for decryption." << endl;
    }

    return 0;
}
