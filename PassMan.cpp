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

// Vars to store files names
string enFileName = "EncryptionKey.key";            // To store encryption key file name
string dataFileName = "Database.csv";               // To store database file name

// Files to handle reading-writing of encryption key
ifstream inpEnKey;                                  // File to handle reading from encryption key
ofstream outEnKey;                                  // File to handle writing to encryption key

// Files to handle reading-writing of database
ifstream inpData;                                   // File to handle reading from database
ofstream outData;                                   // File to handle writing to database  

// Method to load encryption key
void getKey(ifstream& inpFile) {
    // Setting encryption key from file
    string line;                                    // To store each line from EncryptKey.key file
    
    // Getting mul val
    getline(inpFile, line);
    enKeyMul = stoi(line);
    
    // Getting add val
    getline(inpFile, line);
    enKeyAdd = stoi(line);
}

void firstInit() {
    // Opening files to test whether 1st time or not
    inpEnKey.open(enFileName);
    inpData.open(dataFileName);

    // Checking if 1st time use of pass-man
    if (inpEnKey.is_open() == false || inpData.is_open() == false) {
        cout << "Setting up pass-man for 1st time use ............." << endl;
        inpEnKey.close();
        inpData.close();

        // Generating new files and reopening
        outEnKey.open(enFileName);
        outData.open(dataFileName);

        // Saving new but empty files
        outEnKey.close();
        outData.close();

        // Generating new encryption key
        outEnKey.open(enFileName);
        enKey(outEnKey);
        outEnKey.clear();
        outEnKey.close();

        cout << "Successfully generated Database file and Encryption Key !" << endl;
    }
    // Closing previously opened files
    inpEnKey.close();
    inpData.close();

    // Getting encryption key
    inpEnKey.open(enFileName);
    getKey(inpEnKey);
    inpEnKey.clear();
    inpEnKey.close();
}

int main(int nArgs, char *allArgs[]) {
    // Variable declaraion and initialisation
    firstInit();
    string inpPass;                                 // To store input string from user
    string oprArg = toLower(allArgs[1]);            // To store operation argument
        
    // To determine operation
    // For adding password
    if (oprArg.compare("add") == 0) {
        // Opening output database in append mode
        outData.open(dataFileName, ios_base::app);
        // Calling addPass method of MangMethods file to add a new password
        addPass(outData, enKeyMul, enKeyAdd);
        outData.clear();
        outData.close();
        // Exiting from program
        return 0;
    }
    // For getting password
    else if (oprArg.compare("get") == 0) {
        // Opening database
        inpData.open(dataFileName);
        string valArg = toLower(allArgs[2]);            // To store argument passed to operation
        string enPass = getPass(inpData, toLower(valArg));
        inpData.clear();
        inpData.close();
        // If reference name not found
        if (enPass.compare("Not Found") == 0) {
            cout << "Reference name not found in database !";
            // Exitting from program
            return 0;
        }
        else {
            // Decrypting password and outputting
            cout << inpDecrypt(enPass, enKeyMul, enKeyAdd) << endl;
            // Exitting from program
            return 0;
        }        
    }
    return 0;
}