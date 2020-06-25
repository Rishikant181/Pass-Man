// pass-man.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "GlobalMembers.h"

// Defining global vars
// Vars for file IO
std::ifstream inpFile;
std::ofstream outFile;

// Vars to store objects
passMan* pm;
logMan* lm;
cryptMan* cm;
secMan* sm;

// Implementing class passMan
// Defining constructor
passMan::passMan(std::string dir) {
    // Initialising member data
    // Initialising working folder
    workDir = dir;
    dataLocation = workDir + "Data\\";
}

// Method to set up pass-man for 1st time use
void firstTime() {
    // Setting up pass-man for 1st time
    if (std::filesystem::exists(cm->enKeyFile) == false) {
        std::cout << "Setting up pass-man for 1st time use ............." << std::endl;

        // Generating new encryption key
        cm->enKey();

        std::cout << "Successfully generated Encryption Key !" << std::endl;

        // Logging
        lm->logData("Set up pass-man for 1st time use");
    }

    // Loading encryption key
    cm->getKey();
}

int main(int nArgs, char *allArgs[]) {
    // Initialising working directory    
    // Storing location temporarily
    std::string loc = allArgs[0];
    loc = loc.substr(0, loc.find_last_of('\\') + 1);
    
    // Initialising objects temporarily
    passMan tempPm(loc);
    logMan tempLm(loc);
    cryptMan tempCm(loc);
    secMan tempSm(loc);

    // Passing the objects to global pointers
    pm = &tempPm;
    lm = &tempLm;
    cm = &tempCm;
    sm = &tempSm;

    // Check and set up pass-man for 1st time use
    firstTime();
    
    std::string oprArg = toLower(allArgs[1]);            // To store operation argument
    std::string refName;                                 // To store reference name
        
    // Checking authorization
    bool authStatus = sm->checkAuth();                       // To store authorization status
    // If authorization fails
    if (authStatus == false) {
        // Alerting user if mail-id set up
        if (std::filesystem::exists(sm->mailIdLoc) == true) {
            std::string mailId;                          // To store mail-id from file
            std::string timeString;                      // To store time as string
            time_t now = time(NULL);                     // To store time_t
            char nowTime[26];                            // To store current time

            // Getting mail-id from file
            inpFile.open(sm->mailIdLoc);
            std::getline(inpFile, mailId);
            inpFile.clear();
            inpFile.close();

            // Decrypting mail-id
            mailId = cm->inpDecrypt(mailId);

            // Getting current time
            ctime_s(nowTime, 26, &now);
            // Storing time as string
            timeString = nowTime;

            // Sending mail and storing status
            bool isMailSent = sendMail(mailId, "Unsuccessful access attempt of pass-man", "Someone tried to access your pass-man passwords at time : " + timeString);

            // Logging
            lm->logData("Unsuccessful attempt to use pass-man made");

            // Checking status and logging
            if (isMailSent == true) {
                lm->logData("Sent mail to alert user");
            }
            else {
                lm->logData("Failed to send mail to alert user");
            }
        }

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

            // Logging
            lm->logData("Stored a new pasword with reference name : " + refName);
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
        else {
            // Logging
            lm->logData("Retrieved password with reference name : " + refName);
        }
        
        // Exitting from program
        return 0;
    }
    // For getting stored passwords refName list with comments
    else if (oprArg.compare("list") == 0) {
        getList();

        // Logging
        lm->logData("Retrieved list of store passwords(Reference names only)");

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

            // Logging
            lm->logData("Edited stored password with reference name : " + refName);
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

            // Logging
            lm->logData("Deleted password with reference name : " + refName);
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
            std::cout << "Backup successfull !" << std::endl;

            // Logging
            lm->logData("Created backup of store passwords at location : " + backLoc);
        }
        else {
            std::cout << "Backup failed !" << std::endl;
            
            // Logging
            lm->logData("Backup creation failed");
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
            std::cout << "Restore successfull !" << std::endl;

            // Logging
            lm->logData("Restored passwords from backup location : " + backLoc);
        }
        else {
            std::cout << "Restore failed !" << std:: endl;

            // Logging
            lm->logData("Restore passwords failed");
        }
        return 0;
    }
    // For changing/adding mail id
    else if (oprArg.compare("mail") == 0) {
        // Getting status
        bool isMailIdChanged = setEmail();

        // Checking status
        if (isMailIdChanged == true) {
            std::cout << "Mail-id changed succesfully !" << std::endl;

            // Logging
            lm->logData("Changed mail-id");
        }
        else {
            std::cout << "Mail-id change failed" << std::endl;

            // Logging
            lm->logData("Mail-id change failed");
        }
        
        return 0;
    }
    // For changing/setting up authorization key
    else if (oprArg.compare("auth") == 0) {
        bool isAuthDone = authKey();
        // Checking status
        if (isAuthDone == true) {
            std::cout << "Authorization key changed successfully" << std::endl;

            // Logging
            lm->logData("Changed authorization key");
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
        // Help for mail
        std::cout << "8. mail    : " << std::endl;
        std::cout << "       Description : This command is used to set up/change mail-id of user to send alerts" << std::endl;
        std::cout << "       Usage       : pass-man mail\n" << std::endl;
        std::cout << "                     To remove authentication mail, change it to blank\n" << std::endl;
        // Help for add
        std::cout << "9. auth    : " << std::endl;
        std::cout << "       Description : This command is used to set up/change authorization key" << std::endl;
        std::cout << "       Usage       : pass-man auth\n" << std::endl;
        std::cout << "                     To remove authentication key, change key to blank\n" << std::endl;

        return 0;
    }
    return 0;
}