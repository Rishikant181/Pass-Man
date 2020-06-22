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

// Vars to store files names and locations
std::string workDir;                                    // To store location of working directory
std::string enFileName = "Key\\EncryptionKey.key";      // To store encryption key file name
std::string auFileName = "Key\\AuthPass.key";           // To store authorization password
std::string dataLocation = "Data\\";                    // To store location of data
std::string mailIdLoc = "Sec\\";                        // To store mail id of user

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
        std::cout << "Setting up pass-man for 1st time use ............." << std::endl;

        // Creating directory to store data
        std::filesystem::create_directory(workDir + "Key");
        std::filesystem::create_directory(workDir + "Data");
        std::filesystem::create_directory(workDir + "Sec");

        // Generating new encryption key
        enKey();

        std::cout << "Successfully generated Database file and Encryption Key !" << std::endl;

        // Checking status of authorization key
        bool hasAuthKey = authKey();
        if (hasAuthKey == true) {
            std::cout << "Successfully set up authorization key !" << std::endl;
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
    // Initialising working directory
    
    // Storing location temporarily
    std::string loc = allArgs[0];
    // Storing actual location
    workDir = loc.substr(0, loc.find_last_of('\\') + 1);
    // Changing file names
    enFileName = workDir + enFileName;
    auFileName = workDir + auFileName;
    dataLocation = workDir + dataLocation;
    mailIdLoc = workDir + mailIdLoc;
    
    // Variable declaraion and initialisation
    firstInit();
    std::string oprArg = toLower(allArgs[1]);            // To store operation argument
    std::string refName;                                 // To store reference name
        
    // Checking authorization
    bool authStatus = checkAuth();                       // To store authorization status
    // If authorization fails
    if (authStatus == false) {
        std::cout << "Failed to authorize. Suspending further operations" << std::endl;
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
            std::cout << "Successfully stored password !" << std::endl;
        }
        else {
            std::cout << "No password was added" << std::endl;
        }
        return 0;
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
            std::cout << "No such reference name found !" << std::endl;
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
            std::cout << "Password update succesful" << std::endl;
        }
        else {
            std::cout << "No changes were made" << std::endl;
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
            std::cout << "Password deletion successful !" << std::endl;
        }
        else {
            std::cout << "No changes were made" << std::endl;
        }
        return 0;
    }
    // For backing up saved passwords
    else if (oprArg.compare("backup") == 0) {
        bool isBacked;                                              // To store whether backup successfull
        std::string backLoc;                                        // To store backup location
        
        // Storing backup location
        backLoc = allArgs[2];

        // Backing up passwords
        isBacked = backPass(backLoc);

        // Checking status
        if (isBacked == true) {
            std::cout << "Backup successfull !" << endl;
        }
        else {
            std::cout << "Backup failed !" << endl;
        }
        return 0;
    }
    // For restoring backed up passwords
    else if (oprArg.compare("restore") == 0) {
        bool isRestored;                                            // To store whether restored or not
        std::string backLoc;                                        // To store backup location

        // Storing backup location
        backLoc = allArgs[2];

        // Restoring from backup
        isRestored = restorePass(backLoc);
        
        // Checking status
        if (isRestored == true) {
            std::cout << "Restore successfull !" << endl;
        }
        else {
            std::cout << "Restore failed !" << endl;
        }
        return 0;
    }
    // For changing/setting up authorization key
    else if (oprArg.compare("auth") == 0) {
        bool isAuthDone = authKey();
        // Checking status
        if (isAuthDone == true) {
            std::cout << "Authorization key changed successfully" << std::endl;
        }
        else {
            std::cout << "Operation cancelled" << std::endl;
        }
        return 0;
    }
    // For displaying help
    else if (oprArg.compare("help") == 0) {
        // Displaying help
        std::cout << " - This commandline utility is used to store store passwords along with comments" << std::endl;
        std::cout << " - Each password is addressed by using a reference name which is set by user" << std::endl;
        std::cout << " - If the user wants, he/she may set up an authorization key using the auth command so that all passwords can be accessed provived you have the authorization password" << std::endl;
        std::cout << "\nThe following commands are available to manage passwords : \n" << std::endl;
        // Help for add
        std::cout << "1. add     : " << std::endl;
        std::cout << "       Description : This command is used to store a new password" << std::endl;
        std::cout << "       Usage       : pass-man add <reference name to create>\n" << std::endl;
        // Help for get
        std::cout << "2. get     : " << std::endl;
        std::cout << "       Description : This command is used to retrieve a stored password" << std::endl;
        std::cout << "       Usage       : pass-man get <reference name to retrieve>\n" << std::endl;
        // Help for list
        std::cout << "3. list    : " << std::endl;
        std::cout << "       Description : This command is used display list of all reference names stored along with their comments" << std::endl;
        std::cout << "       Usage       : pass-man list\n" << std::endl;
        // Help for edit
        std::cout << "4. edit    : " << std::endl;
        std::cout << "       Description : This command is used to edit a stored password" << std::endl;
        std::cout << "       Usage       : pass-man edit <reference name to edit>\n" << std::endl;
        // Help for del
        std::cout << "5. del     : " << std::endl;
        std::cout << "       Description : This command is used to delete a stored password" << std::endl;
        std::cout << "       Usage       : pass-man del <reference name to delete>\n" << std::endl;
        // Help for backup
        std::cout << "6. backup  : " << std::endl;
        std::cout << "       Description : This command is used create a backup of stored passwords" << std::endl;
        std::cout << "       Usage       : pass-man backup <folder to store backup to>\n" << std::endl;
        // Help for restore
        std::cout << "7. restore : " << std::endl;
        std::cout << "       Description : This command is used passwords from a specified backup" << std::endl;
        std::cout << "       Usage       : pass-man backup <path to backup>\n" << std::endl;
        // Help for add
        std::cout << "8. auth    : " << std::endl;
        std::cout << "       Description : This command is used to set up/change authorization key" << std::endl;
        std::cout << "       Usage       : pass-man auth\n" << std::endl;
        std::cout << "                     To remove authentication key, change key to blank\n" << std::endl;

        return 0;
    }
    return 0;
}