// pass-man.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "Crypto.h"
#include "Helper.h"
#include "MangMethods.h"

// Global vars
int enKeyMul = 0;                                       // To store necryption key multiplicative part
int enKeyAdd = 0;                                       // To store necryption key additive part

// Vars to store files names
std::string enFileName = "Key\\EncryptionKey.key";      // To store encryption key file name
std::string dataLocation = "Data\\";                    // To store location of data

// Files to handle reading-writing of database
std::ifstream inpFile;                                  // File to handle reading from file
std::ofstream outFile;                                  // File to handle writing to file  

// Method to load encryption key
void getKey() {
    // Setting encryption key from file
    std::string line;                                   // To store each line from EncryptKey.key file
    
    // Getting mul val
    std::getline(inpFile, line);
    enKeyMul = std::stoi(line);
    
    // Getting add val
    std::getline(inpFile, line);
    enKeyAdd = std::stoi(line);
}

void firstInit() {
    if (filesystem::exists(dataLocation.substr(0, dataLocation.find('\\'))) == false) {
        std::cout << "Setting up pass-man for 1st time use ............." << endl;

        // Creating directory to store data
        filesystem::create_directory("Key");
        filesystem::create_directory("Data");

        enKey();

        std::cout << "Successfully generated Database file and Encryption Key !" << endl;
    }
    // Getting encryption key
    inpFile.open(enFileName);
    getKey();
    // Closing file
    inpFile.clear();
    inpFile.close();
}

int main(int nArgs, char *allArgs[]) {
    // Variable declaraion and initialisation
    firstInit();
    std::string oprArg = toLower(allArgs[1]);            // To store operation argument
    std::string refName;                                 // To store reference name
        
    // To determine operation
    
    // For adding password
    if (oprArg.compare("add") == 0) {
        // Getting reference name
        refName = toLower(allArgs[2]);
        
        outFile.open(dataLocation + refName + ".pass");
        // Calling addPass method of MangMethods file to add a new password
        addPass();
        
        // Exiting from program
        return 0;
    }
    // For getting password
    else if (oprArg.compare("get") == 0) {
        // Getting reference name
        refName = toLower(allArgs[2]);
        
        inpFile.open(dataLocation + refName + ".pass");
        // Opening Passfile
        std::string refName = toLower(allArgs[2]);           // To store refName
        std::string enPass = getPass(refName);               // To store encrypted password from database

        // If reference name not found
        if (enPass.compare("NA") == 0) {
            std::cout << "Reference name not found in database" << endl;
            return 0;
        }
        
        // Decrypting password and outputting
        std::cout << inpDecrypt(enPass) << endl;
        
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
        
        inpFile.open(dataLocation + refName + ".pass");
        // Getting reference name
        refName = toLower(allArgs[2]);
        isEdited = editPass(refName);

        // Checking if changes made
        if (isEdited == true) {
            std::cout << "Password update succesful" << endl;
        }
        else {
            std::cout << "No changes were made" << endl;
        }
        return 0;
    }
    return 0;
}