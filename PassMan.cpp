// pass-man.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <ctime>
#include "PassMan.h"
#include "Crypto.h"
#include "Helper.h"
#include "MangMethods.h"
#include "SecMethods.h"
#include "Mailman.h"

// Global vars
// Files to handle reading-writing of database
std::ifstream inpFile;                                  // File to handle reading from file
std::ofstream outFile;                                  // File to handle writing to file

// Other vars
std::string authPass;                                   // To store authorization password

// Implementing class passMan
// Defining constructor
passMan::passMan(std::string workDir) {
    // Initialising member data
    // Initialising enKeys
    enKeyMul = 0;
    enKeyAdd = 0;
    
    // Initialising working folder
    enFileName = workDir + "Key\\EncryptionKey.key";
    auFileName = workDir + "Security\\AuthPass.key";
    dataLocation = workDir + "Data\\";
    mailIdLoc = workDir + "Security\\MailId.id";
}

// Defining getKey method
void passMan::getKey() {
    // Setting encryption key from file
    std::string line;                                   // To store each line from EncryptKey.key file

    // Getting mul val
    std::getline(inpFile, line);
    enKeyMul = std::stoi(line);

    // Getting add val
    std::getline(inpFile, line);
    enKeyAdd = std::stoi(line);
}

// Defining getIntMemberData method
int passMan::getIntMemberData(std::string datName) {
    //For dataLocation
    if (datName.compare("enmul") == 0) {
        return enKeyMul;
    }
    // For enFileName
    else if (datName.compare("enadd") == 0) {
        return enKeyAdd;
    }
}

// Defining getStringMemberData method
std::string passMan::getStringMemberData(std::string datName) {
    //For dataLocation
    if (datName.compare("data") == 0) {
        return dataLocation;
    }
    // For enFileName
    else if (datName.compare("enfile") == 0) {
        return enFileName;
    }
    // For auFileName
    else if (datName.compare("aufile") == 0) {
        return auFileName;
    }
    // For mailIdLoc
    else if (datName.compare("mailid") == 0) {
        return mailIdLoc;
    }
}

// Method to set up pass-man for 1st time use
void firstTime(passMan &ob) {
    // Setting up pass-man for 1st time
    if (std::filesystem::exists(ob.getStringMemberData("enfile")) == false) {
        std::cout << "Setting up pass-man for 1st time use ............." << std::endl;

        // Generating new encryption key
        enKey(ob);

        std::cout << "Successfully generated Encryption Key !" << std::endl;

        // Checking status of authorization key
        bool hasAuthKey = authKey(ob);
        if (hasAuthKey == true) {
            std::cout << "Successfully set up authorization key !" << std::endl;
        }
    }

    // Getting encryption key
    inpFile.open(ob.getStringMemberData("enfile"));
    ob.getKey();
    // Closing file
    inpFile.clear();
    inpFile.close();
}

int main(int nArgs, char *allArgs[]) {
    // Initialising working directory    
    // Storing location temporarily
    std::string loc = allArgs[0];
    
    // Initialising passMan object
    passMan ob(loc.substr(0, loc.find_last_of('\\') + 1));

    // Check and set up pass-man for 1st time use
    firstTime(ob);
    
    std::string oprArg = toLower(allArgs[1]);            // To store operation argument
    std::string refName;                                 // To store reference name
        
    // Checking authorization
    bool authStatus = checkAuth(ob);                       // To store authorization status
    // If authorization fails
    if (authStatus == false) {
        std::cout << "Failed to authorize. Suspending further operations" << std::endl;

        // Alerting user if mail-id set up
        if (std::filesystem::exists(ob.getStringMemberData("mailid")) == true) {
            std::string mailId;                          // To store mail-id from file
            std::string timeString;                      // To store time as string
            time_t now = time(NULL);                     // To store time_t
            char nowTime[26];                            // To store current time

            // Getting mail-id from file
            inpFile.open(ob.getStringMemberData("mailid"));
            std::getline(inpFile, mailId);
            inpFile.clear();
            inpFile.close();

            // Decrypting mail-id
            mailId = inpDecrypt(ob, mailId);

            // Getting current time
            ctime_s(nowTime, 26, &now);
            // Storing time as string
            timeString = nowTime;

            // Sending mail
            sendMail(mailId, "Unsuccessful access attempt of pass-man", "Someone tried to access your pass-man passwords at time : " + timeString);
        }
        return 0;
    }

    // To determine operation
    
    // For adding password
    if (oprArg.compare("add") == 0) {
        bool isAdded;                                    // To store whether password added or not
        
        // Getting reference name
        refName = toLower(allArgs[2]);
        
        // Calling addPass method of MangMethods file to add a new password
        isAdded = addPass(ob, refName);
        
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
        isPresent = getPass(ob, refName);

        // Checking status
        if (isPresent == false) {
            std::cout << "No such reference name found !" << std::endl;
        }
        
        // Exitting from program
        return 0;
    }
    // For getting stored passwords refName list with comments
    else if (oprArg.compare("list") == 0) {
        getList(ob);
        return 0;
    }
    // For editing a stored password
    else if (oprArg.compare("edit") == 0) {
        bool isEdited;                                  // To store whether password edited or not
        
        // Getting reference name
        refName = toLower(allArgs[2]);
        
        // Editing and storing status
        isEdited = editPass(ob, refName);

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
        isDeleted = delPass(ob, refName);

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
        isBacked = backPass(ob, backLoc);

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
        isRestored = restorePass(ob, backLoc);
        
        // Checking status
        if (isRestored == true) {
            std::cout << "Restore successfull !" << endl;
        }
        else {
            std::cout << "Restore failed !" << endl;
        }
        return 0;
    }
    // For changing/adding mail id
    else if (oprArg.compare("mail") == 0) {
        // Getting status
        bool isMailIdChanged = setEmail(ob);

        // Checking status
        if (isMailIdChanged == true) {
            std::cout << "Mail id changed succesfully !" << std::endl;
        }
        else {
            std::cout << "Maild id change failed" << std::endl;
        }
        
        return 0;
    }
    // For changing/setting up authorization key
    else if (oprArg.compare("auth") == 0) {
        bool isAuthDone = authKey(ob);
        // Checking status
        if (isAuthDone == true) {
            std::cout << "Authorization key changed successfully" << std::endl;
        }
        else {
            std::cout << "Operation cancelled" << std::endl;
        }
        return 0;
    }
    // For changing email id
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