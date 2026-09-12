#include <iostream> //allows us to use input and output
#include <string> //allows us to use strings
#include <cctype> //allows us to use toupper
#include <fstream> //allows us to read files
#include <unordered_set> //allows fast dictionary lookups
#include <chrono> //allows us to measure how long the cracker takes

using namespace std; //allows us to use standard c++ names

string cleanText(string text) { //removes spaces and converts letters to uppercase
    string cleaned = ""; //stores the cleaned text

    for (int i = 0; i < text.length(); i++) { //loops through each character
        if (text[i] != ' ') { //ignores spaces
            cleaned += toupper(text[i]); //converts the character to uppercase and adds it
        }
    }

    return cleaned; //returns the cleaned text
}

unordered_set<string> loadDictionary(string filename) { //loads dictionary words into memory
    unordered_set<string> dictionary; //stores the dictionary words
    ifstream file(filename); //opens the dictionary file
    string word; //stores each word as it is read

    if (!file.is_open()) { //checks if the dictionary opened successfully
        cout << "Error opening dictionary file." << endl;
        return dictionary;
    }

    while (file >> word) { //reads one word at a time
        dictionary.insert(word); //adds the word to the dictionary
    }

    file.close(); //closes the dictionary file
    return dictionary; //returns the dictionary
}

string encrypt(string message, string key) { //encrypts a message using the vigenere cipher
    message = cleanText(message); //removes spaces and converts the message to uppercase
    key = cleanText(key); //converts the key to uppercase

    string encrypted = ""; //stores the encrypted message

    for (int i = 0; i < message.length(); i++) { //loops through each letter in the message
        int messageValue = message[i] - 'A'; //converts the message letter to a number from 0 to 25
        int keyValue = key[i % key.length()] - 'A'; //gets the matching key letter as a number

        int encryptedValue = (messageValue + keyValue) % 26; //applies the encryption formula
        encrypted += encryptedValue + 'A'; //converts the result back into a letter
    }

    return encrypted; //returns the encrypted message
}

string decrypt(string ciphertext, string key) { //decrypts a message using the vigenere cipher
    ciphertext = cleanText(ciphertext); //removes spaces and converts the ciphertext to uppercase
    key = cleanText(key); //converts the key to uppercase

    string decrypted = ""; //stores the decrypted message

    for (int i = 0; i < ciphertext.length(); i++) { //loops through each encrypted letter
        int ciphertextValue = ciphertext[i] - 'A'; //converts the encrypted letter to a number
        int keyValue = key[i % key.length()] - 'A'; //gets the matching key letter as a number

        int decryptedValue = (ciphertextValue - keyValue + 26) % 26; //applies the decryption formula
        decrypted += decryptedValue + 'A'; //converts the result back into a letter
    }

    return decrypted; //returns the decrypted message
}

void crackPassword(string ciphertext, int keyLength, int firstWordLength,
                   const unordered_set<string>& dictionary) {
    auto startTime = chrono::high_resolution_clock::now(); //records when cracking starts

    long long totalKeys = 1; //stores the total number of possible keys

    for (int i = 0; i < keyLength; i++) { //calculates 26 raised to the key length
        totalKeys *= 26;
    }

    cout << "Trying " << totalKeys << " possible keys..." << endl;

    unordered_set<string> prefixes; //stores valid beginnings of dictionary words

    for (const string& word : dictionary) { //goes through every dictionary word
        if (word.length() == firstWordLength) { //only uses words with the correct length
            for (int i = 1; i <= firstWordLength; i++) { //creates every prefix of the word
                prefixes.insert(word.substr(0, i)); //adds the prefix to the set
            }
        }
    }

    string key(keyLength, 'A'); //starts with the first possible key

    for (long long keyNumber = 0; keyNumber < totalKeys; keyNumber++) { //tries every possible key
        string firstWord = ""; //stores the decrypted first word
        bool possibleWord = true; //keeps track of whether the word could still be valid

        for (int i = 0; i < firstWordLength; i++) { //decrypts the first word one letter at a time
            int ciphertextValue = ciphertext[i] - 'A'; //converts ciphertext letter to a number
            int keyValue = key[i % keyLength] - 'A'; //gets the matching key letter
            int decryptedValue = (ciphertextValue - keyValue + 26) % 26; //decrypts the letter

            firstWord += decryptedValue + 'A'; //adds the decrypted letter to the first word

            if (prefixes.find(firstWord) == prefixes.end()) { //checks whether this can still be a dictionary word
                possibleWord = false; //marks the word as impossible
                break; //stops decrypting this word early
            }
        }

        if (possibleWord && dictionary.find(firstWord) != dictionary.end()) { //checks complete dictionary match
            string plaintext = decrypt(ciphertext, key); //decrypts the full message

            cout << "Possible match:" << endl;
            cout << "Key: " << key << endl;
            cout << "Plaintext: " << plaintext << endl;
            cout << endl;
        }

        for (int i = keyLength - 1; i >= 0; i--) { //moves to the next key
            if (key[i] < 'Z') { //checks whether this letter can increase
                key[i]++; //moves the letter forward by one
                break; //the next key is ready
            } else {
                key[i] = 'A'; //rolls Z back over to A
            }
        }
    }

    auto endTime = chrono::high_resolution_clock::now(); //records when cracking ends

    chrono::duration<double> elapsedTime = endTime - startTime; //calculates total cracking time

    cout << "Time: " << elapsedTime.count() << " seconds" << endl; //prints cracking time
}
int main() { //main function where the program starts
    unordered_set<string> dictionary = loadDictionary("MP1_dict.txt"); //loads the dictionary

    cout << "Dictionary words loaded: " << dictionary.size() << endl; //prints number of loaded words
    cout << "Vigenere Cipher" << endl; //prints the program title
    cout << "1. Encrypt" << endl; //prints the encryption option
    cout << "2. Decrypt" << endl; //prints the decryption option
    cout << "3. Crack Password" << endl; //prints the cracking option
    cout << "Choose an option: "; //asks the user to choose

    int choice; //stores the user's choice
    cin >> choice; //gets the user's choice
    cin.ignore(); //clears the newline left behind by cin

    if (choice == 1 || choice == 2) { //handles encryption and decryption
        string message; //stores the user's message
        string key; //stores the user's key

        cout << "Enter message: "; //asks for the message
        getline(cin, message); //gets the message

        cout << "Enter key: "; //asks for the key
        getline(cin, key); //gets the key

        if (choice == 1) { //checks if encryption was selected
            cout << "Encrypted: " << encrypt(message, key) << endl; //encrypts the message
        }
        else { //otherwise decryption was selected
            cout << "Decrypted: " << decrypt(message, key) << endl; //decrypts the message
        }
    }
    else if (choice == 3) { //handles password cracking
        string ciphertext; //stores the encrypted message
        int keyLength; //stores the known key length
        int firstWordLength; //stores the known first word length

        cout << "Enter ciphertext: "; //asks for the ciphertext
        getline(cin, ciphertext); //gets the ciphertext

        cout << "Enter key length: "; //asks for the key length
        cin >> keyLength; //gets the key length

        cout << "Enter first word length: "; //asks for the first word length
        cin >> firstWordLength; //gets the first word length

        ciphertext = cleanText(ciphertext); //cleans the ciphertext

        crackPassword(ciphertext, keyLength, firstWordLength, dictionary); //starts the cracker
    }

    return 0; //ends the program successfully
}