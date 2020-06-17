// pass-man.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "Crypto.h"
#include "Helper.h"
#include "MangMethods.h"

using namespace std;

// Global vars
int enKeyMul = 0;                                   // To store necryption key multiplicative part
int enKeyAdd = 0;                                   // To store necryption key additive part

// Vars to store files names
string enFileName = "Key\\EncryptionKey.key";       // To store encryption key file name
string dataLocation = "Data\\";                     // To store location of data

// Files to handle reading-writing of encryption key
ifstream inpEnKey;                                  // File to handle reading from encryption key
ofstream outEnKey;                                  // File to handle writing to encryption key

// Files to handle reading-writing of database
ifstream inpData;                                   // File to handle reading from database
ofstream outData;                                   // File to handle writing to database  

// Method to load encryption key
void getKey() {
    // Setting encryption key from file
    string line;                                    // To store each line from EncryptKey.key file
    
    // Getting mul val
    getline(inpEnKey, line);
    enKeyMul = stoi(line);
    
    // Getting add val
    getline(inpEnKey, line);
    enKeyAdd = stoi(line);
}

void firstInit() {
    if (filesystem::exists("Data") == false) {
        cout << "Setting up pass-man for 1st time use ............." << endl;

        // Creating directory to store data
        filesystem::create_directory("Key");
        filesystem::create_directory("Data");

        enKey();

        cout << "Successfully generated Database file and Encryption Key !" << endl;
    }
    // Getting encryption key
    inpEnKey.open(enFileName);
    getKey();
    inpEnKey.clear();
    inpEnKey.close();
}

int main(int nArgs, char *allArgs[]) {
    // Variable declaraion and initialisation
    firstInit();
    string oprArg = toLower(allArgs[1]);            // To store operation argument
    string refName;                                 // To store reference name
        
    // To determine operation
    
    // For adding password
    if (oprArg.compare("add") == 0) {
        // Getting reference name
        refName = toLower(allArgs[2]);
        
        // Opening output database in append mode
        outData.open(dataLocation + refName + ".pass", ios_base::app);
        
        // Calling addPass method of MangMethods file to add a new password
        addPass();
        outData.clear();
        outData.close();
        
        // Exiting from program
        return 0;
    }
    // For getting password
    else if (oprArg.compare("get") == 0) {
        // Getting reference name
        refName = toLower(allArgs[2]);
        
        // Opening Passfile
        string refName = toLower(allArgs[2]);           // To store refName
        string enPass = getPass(refName);               // To store encrypted password from database
        
                                                        // If reference name not found
        if (enPass.compare("NA") == 0) {
            cout << "Reference name not found in database" << endl;
            return 0;
        }
        
        // Decrypting password and outputting
        cout << inpDecrypt(enPass) << endl;
        
        // Exitting from program
        return 0;
    }
    // For getting stored passwords refName list with comments
    else if (oprArg.compare("list") == 0) {
        getList();
        return 0;
    }
    // For editing a stored password
    else if (oprArg.compare("edit") == 0) {
        bool isEdited;                                  // To store whether password edited or not
        
        // Getting reference name
        refName = toLower(allArgs[2]);
        isEdited = editPass(refName);

        // Checking if changes made
        if (isEdited == true) {
            cout << "Password update succesful" << endl;
        }
        else {
            cout << "No changes were made" << endl;
        }
        return 0;
    }
    return 0;
}