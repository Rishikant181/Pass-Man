// pass-man.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <fstream>
#include "Crypto.h"
#include "Helper.h"
#include "MangMethods.h"

using namespace std;

// Global vars
int enKeyMul = 0;                                   // To store necryption key multiplicative part
int enKeyAdd = 0;                                   // To store necryption key additive part

// Method to initialise files for 1st time use
void initaliseFirst() {
    // Generating encryption key 1st
    cout << "Setting pass-man for 1st time use ............" << endl;
    enKey();

    // Creating a new database file
    ofstream dataBase("Database.csv");
    dataBase.clear();
    dataBase.close();

    // Completing 1st time init
    cout << "Succesfully generated encryption key and database file !" << endl;
}

// Method to load encryption key
void getKey() {
    ifstream enKeyFile("EncryptionKey.key");
    // If no encryption key is detected i.e, for 1st time initialisation of encryption key
    if (enKeyFile.is_open() == false) {
        // Initialising for 1st time use
        initaliseFirst();

        // Reopening enccryption key file
        enKeyFile.open("EncryptionKey.key");
    }

    // Setting encryption key from file
    string line;                                    // To store each line from EncryptKey.key file
    
    // Getting mul val
    getline(enKeyFile, line);
    enKeyMul = stoi(line);
    
    // Getting add val
    getline(enKeyFile, line);
    enKeyAdd = stoi(line);

    // Closing enKeyFile
    enKeyFile.close();
}

int main(int nArgs, char *allArgs[]) {
    // Variable declaraion and initialisation
    string inpPass;                                 // To store input string from user
    string oprArg = allArgs[1];                     // To store operation argument
    
    // Setting encryption key
    getKey();

    // To determine operation
    if (toLower(oprArg).compare("add") == 0) {
        // Calling addPass method of MangMethods file to add a new password
        addPass();
    }
    return 0;
}