#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

// Function to remove spaces from a string
string removeSpaces(const string str) {
    string result;
    for (char ch : str) {
        if (ch != ' ') {
            result += ch;
        }
    }
    return result;
}

// Function to generate a key based on the length of the input text
string generateKey(const string& text) {
    string key;
    for (char ch : text) {
        char randomDigit = '0' + rand() % 10;
        key += randomDigit;
    }
    return key;
}

// Function to perform columnar transposition encryption
string encryptText(const string plaintext, const string key) {

    string cleanedText = removeSpaces(plaintext);

    // Calculate the number of rows and columns
    int columns = key.length();

    int rows = (cleanedText.length() + columns - 1) / columns;

    // Dynamically allocate memory for the 2D matrix
    char** matrix = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new char[columns];
    }

    // Fill the matrix with characters from the cleaned plain text
    for (int i = 0, k = 0; i < rows; ++i) {
        for (int j = 0; j < columns && k < cleanedText.length(); ++j) {
            matrix[i][j] = cleanedText[k++];
        }
    }

     // Dynamically allocate memory for the permutation order array
    int* order = new int[columns];
    for (int i = 0; i < columns; ++i) {
        order[i] = i;
    }

    // Use a bubble sort algorithm to create an encrypted text
    for (int i = 0; i < columns - 1; ++i) {
        for (int j = 0; j < columns - i - 1; ++j) {
            if (key[order[j]] > key[order[j + 1]]) {

                // Swap elements if they are in the wrong order
                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }

    string ciphertext;
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            ciphertext += matrix[j][order[i]];
        }
    }

    return ciphertext;
}

// Function to perform columnar transposition decryption
string decryptText(const string ciphertext, const string key) {

    int columns = key.length();


    int rows = (ciphertext.length() + columns - 1) / columns;


    // Dynamically allocate memory for the 2D matrix
    char** matrix = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new char[columns];
    }

    // Dynamically allocate memory for the permutation order array
    int* order = new int[columns];
    for (int i = 0; i < columns; ++i) {
        order[i] = i;
    }

   for (int i = 0; i < columns - 1; ++i) {
        for (int j = 0; j < columns - i - 1; ++j) {
            if (key[order[j]] > key[order[j + 1]]) {
                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }


    int k = 0;
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            matrix[j][order[i]] = ciphertext[k++];
        }
    }

    string cleanedPlaintext;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cleanedPlaintext += matrix[i][j];
        }
    }
    return cleanedPlaintext;
}

string readFile(const string filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filePath << endl;
        exit(1);
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}


void writeFile(const string filePath, const string content) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filePath << endl;
        exit(1);
    }

    file << content;
    file.close();
}

int main() {
    while (true) {
   int choice;
        cout << "Choose an option:\n1. Encrypt\n2. Decrypt\n3. Quit\n";
        cin >> choice;

        if (choice == 3) {
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        }

        string inputPath, outputPath, key;

        // Get input file path, output file path, key
        cout << "Enter the input file path: ";
        getline(std::cin >> ws, inputPath);

        cout << "Enter the output file path: ";
        getline(cin >> ws, outputPath);

        cout << "Enter the key (or press Enter to generate one): ";
        getline(cin, key);

        if (key.empty()) {
            string fileContent = readFile(inputPath);
            key = generateKey(fileContent);
            cout << "Generated key: " << key << endl;
        }


        string fileContent = readFile(inputPath);

        if (choice == 1) {
            string encryptedContent = encryptText(fileContent, key);
            writeFile(outputPath, encryptedContent);
            cout << "Encryption complete. Encrypted content written to " << outputPath << endl;
        } else if (choice == 2) {
            string decryptedContent = decryptText(fileContent, key);
            writeFile(outputPath, decryptedContent);
            cout << "Decryption complete. Decrypted content written to " << outputPath << endl;
        } else {
            cout << "Invalid choice. Please choose 1 for encryption, 2 for decryption, or 3 to quit." << endl;
        }
    }

    return 0;
}
