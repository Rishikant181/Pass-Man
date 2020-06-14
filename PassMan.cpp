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

// Method to load encryption key
void getKey() {
    ifstream inFile("EncryptKey.key");
    // If no encryption key is detected i.e, for 1st time initialisation of encryption key
    if (inFile.is_open() == false) {
        // Setting new encryption key if not present
        inFile.clear();
        enKey();
        inFile.open("EncryptKey.key");
        
        // Creating a new database file to store passes
        ofstream outFile("Database.csv");
        outFile.clear();
        outFile.close();
    }

    // Setting encryption key from file
    string line;                                    // To store each line from EncryptKey.key file
    
    // Getting mul val
    getline(inFile, line);
    enKeyMul = stoi(line);
    
    // Getting add val
    getline(inFile, line);
    enKeyAdd = stoi(line);
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