// This files contains various methods related to security

#include "GlobalMembers.h"

// Using cli .net library
using namespace System::Net;

// Defining member method
secMan::secMan(std::string auFile, std::string mailLoc) {
	// Initialising member data
	auFileName = auFile;
	mailIdLoc = mailLoc;
}

// Defining method checkAuth
bool secMan::checkAuth() {
	std::string inpAuthPass;										// To store input auth pass
	std::string actAuthPass;										// To store actual auth pass
	// Checking if authorization key set up
	if (std::filesystem::exists(auFileName) == true) {
		// Taking input
		std::cout << "Enter authorization password : ";
		std::getline(std::cin, inpAuthPass);

		// Getting actual auth pass
		inpFile.open(auFileName);
		std::getline(inpFile, actAuthPass);
		inpFile.clear();
		inpFile.close();

		// Checking for correct auth
		// For successful authorization
		if (inpAuthPass.compare(cm->inpDecrypt(actAuthPass)) == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	// Default case when no authorization has been set up
	std::cout << "*WARNING* No authorization key has been set up. Your passwords are vulnerable !" << std::endl;
	return true;
}

// Defining method changeAuthKey
bool secMan::changeAuthKey() {
	std::string newAuthKey;										// To store new auth key
	std::string conAuthKey;										// To store auth key confirmation
	std::string mailId;											// To store input mail id

	std::cout << "Enter new authorization key : ";

	// Taking new auth key
	std::getline(std::cin, newAuthKey);

	// If user wants to remove auth key
	if (newAuthKey.compare("") == 0) {
		std::string conChoice;
		// Asking confirmation
		std::cout << "Are you sure you want to remove authentication ?(y/n) : ";
		std::getline(std::cin, conChoice);
		// If yes
		if (toLower(conChoice).compare("y") == 0) {
			std::filesystem::remove(auFileName);
			return true;
		}
		// If no
		else {
			return false;
		}
	}

	// Confirming auth key
	std::cout << "Confirm authorization key   : ";
	std::getline(std::cin, conAuthKey);

	// Checking confirmation
	if (conAuthKey.compare(newAuthKey) != 0) {
		std::cout << "Confirmation failed !" << std::endl;
		return false;
	}

	// Storing new auth key
	outFile.open(auFileName);
	outFile << cm->inpEncrypt(newAuthKey) << "\n";
	outFile.clear();
	outFile.close();

	return true;
}

// Defining method failAuthMail
bool secMan::failAuthMail() {
	// Alerting user if mail-id set up
	if (std::filesystem::exists(mailIdLoc) == true) {
		std::string mailId;                          // To store mail-id from file
		std::string timeString;                      // To store time as string
		time_t now = time(NULL);                     // To store time_t
		char nowTime[26];                            // To store current time

		// Getting mail-id from file
		inpFile.open(mailIdLoc);
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

		return isMailSent;
	}

	return false;
}