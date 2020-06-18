// pass-man.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "Crypto.h"
#include "Helper.h"
#include "MangMethods.h"
#include "SecMethods.h"

// Global vars
int enKeyMul = 0;                                       // To store necryption key multiplicative part
int enKeyAdd = 0;                                       // To store necryption key additive part

// Vars to store files names
std::string enFileName = "Key\\EncryptionKey.key";      // To store encryption key file name
std::string auFileName = "Key\\AuthPass.key";           // To store authorization password
std::string dataLocation = "Data\\";                    // To store location of data

// Files to handle reading-writing of database
std::ifstream inpFile;                                  // File to handle reading from file
std::ofstream outFile;                                  // File to handle writing to file

// Other vars
std::string authPass;                                   // To store authorization password

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
    if (std::filesystem::exists(enFileName) == false) {
        std::cout << "Setting up pass-man for 1st time use ............." << endl;

        // Creating directory to store data
        std::filesystem::create_directory("Key");
        std::filesystem::create_directory("Data");

        // Generating new encryption key
        enKey();

        std::cout << "Successfully generated Database file and Encryption Key !" << endl;

        // Checking status of authorization key
        bool hasAuthKey = authKey();
        if (hasAuthKey == true) {
            std::cout << "Successfully set up authorization key !" << endl;
        }
        else {
            std::cout << "No authorization key set up ! Your passwords are vulnerable !" << endl;
        }
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
        
    // Checking authorization
    bool authStatus = checkAuth();                       // To store authorization status
    // If authorization fails
    if (authStatus == false) {
        std::cout << "Failed to authorize. Suspending further operations" << endl;
        return 0;
    }

    // To determine operation
    
    // For adding password
    if (oprArg.compare("add") == 0) {
        bool isAdded;                                    // To store whether password added or not
        
        // Getting reference name
        refName = toLower(allArgs[2]);
        
        // Calling addPass method of MangMethods file to add a new password
        isAdded = addPass(refName);
        
        // Checking status
        if (isAdded == true) {
            std::cout << "Successfully stored password !" << endl;
            return 0;
        }
        else {
            std::cout << "No password was added" << endl;
            return 0;
        }
    }
    // For getting password
    else if (oprArg.compare("get") == 0) {
        bool isPresent;                                      // To store whether reference name present or not

        // Getting reference name
        refName = toLower(allArgs[2]);
        
        // Getting password
        std::string refName = toLower(allArgs[2]);           // To store refName
        
        // Calling method to get pass
        isPresent = getPass(refName);

        // Checking status
        if (isPresent == false) {
            std::cout << "No such reference name found !" << endl;
            return 0;
        }
        
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
        
        // Editing and storing status
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
    // For deleting a stored password
    else if (oprArg.compare("del") == 0) {
        bool isDeleted;                                     // To store whether password deleted or not
        
        // Getting reference name
        refName = toLower(allArgs[2]);

        // Deleting
        isDeleted = delPass(refName);

        // Checking status
        if (isDeleted == true) {
            std::cout << "Password deletion successful !" << endl;
            return 0;
        }
        else {
            std::cout << "No changes were made" << endl;
            return 0;
        }
    }
    // For changing/setting up authorization key
    else if (oprArg.compare("auth") == 0) {
        bool isAuthDone = authKey();
        // Checking status
        if (isAuthDone == true) {
            std::cout << "Authorization key changed successfully" << endl;
            return 0;
        }
        else {
            std::cout << "Operation cancelled" << endl;
        }
    }
    return 0;
}