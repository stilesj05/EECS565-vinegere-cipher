#include <iostream> //allows us to use input and output
#include <string> //allows us to use strings
#include <cctype> //allows us to use toupper

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

int main() { //main function where the program starts
    int choice; //stores whether the user wants to encrypt or decrypt
    string message; //stores the user's message
    string key; //stores the user's key

    cout << "Vigenere Cipher" << endl; //prints the program title
    cout << "1. Encrypt" << endl; //prints the encryption option
    cout << "2. Decrypt" << endl; //prints the decryption option
    cout << "Choose an option: "; //asks the user to choose
    cin >> choice; //gets the user's choice

    cin.ignore(); //clears the newline left behind by cin

    cout << "Enter message: "; //asks for the message
    getline(cin, message); //gets the entire message including spaces

    cout << "Enter key: "; //asks for the key
    getline(cin, key); //gets the key

    if (choice == 1) { //checks if the user chose encryption
        cout << "Encrypted: " << encrypt(message, key) << endl; //encrypts and prints the message
    }
    else if (choice == 2) { //checks if the user chose decryption
        cout << "Decrypted: " << decrypt(message, key) << endl; //decrypts and prints the message
    }

    return 0; //ends the program successfully
}