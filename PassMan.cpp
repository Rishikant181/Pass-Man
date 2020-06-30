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
    
    // Initialising file and folder locations
    dataLocation = workDir + "Data\\";
    refLocation = workDir + "Refs\\";
    logDir = workDir + "Logs\\";
    auFileName = workDir + "Security\\AuthPass.pass";
}

// Defining desctructor
int selfDestruct() {
    // Displaying blank line for readability
    std::cout <<  std::endl;

    // Deleting pointed objects to re-allocate memory
    delete pm;
    delete lm;
    delete sm;
    delete cm;

    // Pointing dangling pointers to null
    pm = NULL;
    lm = NULL;
    sm = NULL;
    cm = NULL;

    return 0;
}

// Defining method getStringData
std::string passMan::getStringData(std::string dName) {
    // If dataLocation
    if (dName.compare("dataloc") == 0) {
        return dataLocation;
    }
    // If refLocation
    else if (dName.compare("refloc") == 0) {
        return refLocation;
    }
    // If logDir
    else if (dName.compare("logdir") == 0) {
        return logDir;
    }
    // If auFileName
    else if (dName.compare("aufile") == 0) {
        return auFileName;
    }
}

// Method to check if pass-man is started for 1st time
bool firstTime() {
    std::cout << "Setting up pass-man for 1st time use ............." << std::endl;

    std::cout << "To use pass-man, you need to set up an authorization password to secure all you passwords" << std::endl;

    // Calling changeAuthKey method to set up authentication key
    bool isAuthChanged = sm->changeAuthKey();

    // If not set up then exitting
    if (isAuthChanged == false) {
        return false;
    }

    // Logging
    lm->logData("Set up pass-man for 1st time use");

    return true;
}

int main(int nArgs, char *allArgs[]) {
    // Adding blank line for readability
    std::cout << std::endl;
    
    // Initialising working directory    
    // Storing location temporarily
    std::string workDir = allArgs[0];
    workDir = workDir.substr(0, workDir.find_last_of('\\') + 1);
    
    // Initialising global pointers
    pm = new passMan(workDir);
    lm = new logMan(pm->getStringData("logdir"));
    sm = new secMan();

    // Checking if this is the first time pass-man is started
    // Checking if pass-man started for 1st time
    if (sm->getStringData("authpass").compare("NA") == 0) {
        bool isFirstDone = firstTime();
        
        // If first time setup failed then exitting
        if (isFirstDone == false) {
            return selfDestruct();
        }
        else {
            std::cout << "Restart pass-man to continue" << std::endl;
            return selfDestruct();
        }
    }

    // Checking authorization
    bool authStatus = sm->checkAuth();                                      // To store authorization status    

    // If authorization fails
    if (authStatus == false) {
        bool isMailSent = sm->failAuthMail();

        // Logging
        lm->logData("Unsuccessful attempt to use pass-man made");

        // Checking status and logging
        if (isMailSent == true) {
            lm->logData("Sent mail to alert user");
        }
        else {
            lm->logData("Failed to send mail to alert user");
        }

        std::cout << "Failed to authorize. Suspending further operations" << std::endl;
        return selfDestruct();
    }

    // Initialising cryptman with authPass
    cm = new cryptMan(sm->getStringData("authpass"));

    // Getting arguments
    std::string oprArg = toLower(allArgs[1]);               // To store operation argument
    std::string refName;                                    // To store reference name

    // Adding blank line for readability
    std::cout << std::endl;

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
            lm->logData("Stored a new pasword with hashed reference name : " + std::to_string(std::hash<std::string>{}(refName)));
        }
        else {
            std::cout << "No password was added" << std::endl;
        }
        return selfDestruct();
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
            lm->logData("Retrieved password with reference name : " + std::to_string(std::hash<std::string>{}(refName)));
        }        
        // Exitting from program
        return selfDestruct();
    }
    // For getting stored passwords refName list with comments
    else if (oprArg.compare("list") == 0) {
        getList();

        // Logging
        lm->logData("Retrieved list of store passwords(Reference names only)");
        return selfDestruct();
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
            lm->logData("Edited stored password with hashed reference name : " + std::to_string(std::hash<std::string>{}(refName)));
        }
        else {
            std::cout << "No changes were made" << std::endl;
        }
        return selfDestruct();
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
            lm->logData("Deleted password with hashed reference name : " + std::to_string(std::hash<std::string>{}(refName)));
        }
        else {
            std::cout << "No changes were made" << std::endl;
        }
        return selfDestruct();
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
        return selfDestruct();
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
        return selfDestruct();
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
        
        return selfDestruct();
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
        return selfDestruct();
    }
    // For cleaning all logs
    else if (oprArg.compare("clean") == 0) {
        // Cleaning all stored logs
        for (auto& file : std::filesystem::directory_iterator(pm->getStringData("logdir"))) {
            // Deleting logs
            std::filesystem::remove(file.path().string());
        }

        std::cout << "Cleaned all logs !" << std::endl;

        return selfDestruct();
    }
    // For resetting pass-man to factory state
    else if (oprArg.compare("reset") == 0) {
        std::string conChoice;                                              // To store user confirmation choice
        // Asking for confirmation
        std::cout << "*WARNING* This will remove all stored passwords. Do you want to continue ?(y/n) : ";
        std::getline(std::cin, conChoice);
        
        // If yes
        if (conChoice.compare("y") == 0) {
            // Deleting everything inside Data folder
            for (auto& file : std::filesystem::directory_iterator(pm->getStringData("dataloc"))) {
                std::filesystem::remove(file.path().string());
            }

            // Deleting everything inside Logs folder
            for (auto& file : std::filesystem::directory_iterator(pm->getStringData("logdir"))) {
                std::filesystem::remove(file.path().string());
            }

            // Deleting everything inside Refs folder
            for (auto& file : std::filesystem::directory_iterator(pm->getStringData("refloc"))) {
                std::filesystem::remove(file.path().string());
            }

            // Deleting everything inside Security folder
            for (auto& file : std::filesystem::directory_iterator(pm->getStringData("aufile").substr(0, pm->getStringData("aufile").find_last_of('\\')))) {
                std::filesystem::remove(file.path().string());
            }

            // Deleting mail-id info from registry
            Microsoft::Win32::RegistryKey^ regKey = Microsoft::Win32::Registry::CurrentUser;
            regKey->DeleteSubKeyTree(gcnew System::String("SOFTWARE\\PassMan"), false);

            // Displaying result
            std::cout << "Reset PassMan to factory default !" << std::endl;

            return selfDestruct();
        }
        
        std::cout << "No changes were done" << std::endl;

        return selfDestruct();

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

        return selfDestruct();
    }
    return selfDestruct();
}