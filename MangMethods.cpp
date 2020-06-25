// This file contains various methods for management of stored passwords

#include "GlobalMembers.h"

// Method to get non-optional input
std::string getReqInput() {
	std::string inpVal;											// To store input string
	while (true) {
		// Take input
		std::getline(std::cin, inpVal);
		// If valid input
		if (inpVal.compare("") != 0) {
			return inpVal;
		}
		// If invalid input
		else {
			std::cout << "Please enter a valid input !" << std::endl;
		}
	}
}

// Method to add a new password to the database
bool addPass(std::string refName) {
	std::string passCom;										// To store commments about passwrds
	std::string actPass;										// To store the actual password
	std::string conPass;										// To store the confirmation password

	// Checking if duplicate refName
	if (std::filesystem::exists(pm->getStringData("dataloc") + refName + ".pass") == true) {
		std::cout << "Reference name already exists, please enter another name" << std::endl;
		return false;
	}

	std::cout << "Comments (Optional) : ";
	std::getline(std::cin, passCom);
	std::cout << "Enter password   : ";
	actPass = getReqInput();
	std::cout << "Confirm password : ";
	conPass = getReqInput();

	// Checking if conPass == actPass
	if (conPass.compare(actPass) == 0) {
		// Adding data to database
		outFile.open(pm->getStringData("dataloc") + refName + ".pass");
		outFile << cm->inpEncrypt(actPass) << " " << passCom << "\n";
		outFile.clear();
		outFile.close();
		return true;
	}
	else {
		std::cout << "Confirmation failed ! Please try again !";
		return false;
	}
}

// Method to get encrypted pass from database
bool getPass(std::string refName) {
	std::string enPass;											// To store encrypted password
	// If reference name not found
	if (std::filesystem::exists(pm->getStringData("dataloc") + refName + ".pass") == false) {
		// Exitting from program
		return false;
	}

	// Getting data from database
	inpFile.open(pm->getStringData("dataloc") + refName + ".pass");
	std::getline(inpFile, enPass);
	inpFile.clear();
	inpFile.close();

	// Getting encrypted password
	enPass = enPass.substr(0, enPass.find(' '));
	
	// Decrypting password and displaying
	std::cout << cm->inpDecrypt(enPass);
	return true;
}

// Method to get the list of all passwords stored by pass-man and displaying only refName and comments
void getList() {
	std::string refName;									// To store file name
	std::string passCom;									// To store comment
	// Iterating through directory
	for (auto& file : std::filesystem::directory_iterator(pm->getStringData("dataloc"))) {
		// Getting refName
		refName = file.path().string().substr(pm->getStringData("dataloc").length());
		refName = refName.substr(0, refName.length() - 5);
		
		// Getting comment from file
		inpFile.open(pm->getStringData("dataloc") + refName + ".pass");
		std::getline(inpFile, passCom);
		inpFile.clear();
		inpFile.close();
		passCom = passCom.substr(passCom.find(' ') + 1);

		// Displaying
		std::cout << "\nReference Name : " << refName << std::endl;
		std::cout << "Comments       : " << passCom << std::endl;
	}
}

// Method to modify a stored password using refName
bool editPass(std::string refName) {
	int oprChoice;											// To store operation choice
	std::string choice;
	std::string line;										// To store the input from file
	std::string newRefName;									// To store new reference name
	std::string newPassCom;									// To store new comment
	std::string newPass;									// To store new password
	std::string newPassCon;									// To store new password confirmation
	// If refName exists
	if (std::filesystem::exists(pm->getStringData("dataloc") + refName + ".pass") == true) {
		// Storing original line temporarily
		inpFile.open(pm->getStringData("dataloc") + refName + ".pass");
		std::getline(inpFile, line);
		inpFile.clear();
		inpFile.close();
		
		// Asking for operation
		std::cout << "1. Change reference name" << std::endl;
		std::cout << "2. Change comment" << std::endl;
		std::cout << "3. Change password" << std::endl;
		
		try {
			// Taking choice
			std::getline(std::cin, choice);
			oprChoice = std::stoi(choice);
		}
		catch (std::invalid_argument ia) {
			std::cout << "Please enter number only !" << std::endl;
			return false;
		}

		switch (oprChoice) {
		// For changing reference name
		case 1: {
			std::cout << "Enter new reference name : ";
			newRefName = getReqInput();

			// Deleting old data
			std::filesystem::remove(pm->getStringData("dataloc") + refName + ".pass");

			// Writing new data
			outFile.open(pm->getStringData("dataloc") + newRefName + ".pass");
			outFile << line;
			outFile.clear();
			outFile.close();
			return true;
		}
		// For changing comment
		case 2: {
			std::cout << "Enter new comment : ";
			std::getline(std::cin, newPassCom);

			// Deleting old data
			std::filesystem::remove(pm->getStringData("dataloc") + refName + ".pass");

			// Writing new data
			outFile.open(pm->getStringData("dataloc") + refName + ".pass");
			outFile << line.substr(0, line.find(' ')) + " " + newPassCom;
			outFile.clear();
			outFile.close();
			return true;
		}
		// For changing password
		case 3: {
			std::cout << "Enter new password   : ";
			newPass = getReqInput();
			std::cout << "Confirm new password : ";
			newPassCon = getReqInput();

			// Checking for confirmation
			if (newPass.compare(newPassCon) != 0) {
				std::cout << "Confirmation failed !" << std::endl;
				return false;
			}
			
			// Deleting old data
			std::filesystem::remove(pm->getStringData("dataloc") + refName + ".pass");

			// Writing new data
			outFile.open(pm->getStringData("dataloc") + refName + ".pass");
			outFile << cm->inpEncrypt(newPass) + line.substr(line.find(' '));
			outFile.clear();
			outFile.close();
			return true;
		}
		// For no match case
		default: {
			return false;
		}
		}
	}
	// If refName does not exist
	else {
		std::cout << "Reference name not found, password does not exist" << std::endl;
		return false;
	}
}

// Method to delete a stored password
bool delPass(std::string refName) {
	std::string delChoice;											// To store confirmation choice
	// Checking if reference name does not exist
	if (std::filesystem::exists(pm->getStringData("dataloc") + refName + ".pass") == false) {
		std::cout << "No such reference name found !" << std::endl;
		return false;
	}
	// If exists
	std::cout << "Are you sure you want to delete " << refName << "?(y/n) : ";
	std::getline(std::cin, delChoice);

	// If confirmed yes
	if (toLower(delChoice).compare("y") == 0) {
		// Deleting
		std::filesystem::remove(pm->getStringData("dataloc") + refName + ".pass");
		return true;
	}
	// If no
	else {
		std::cout << "Operation cancelled" << std::endl;
		return false;
	}
}

// Method to backup passwords to specified location
bool backPass(std::string backLoc) {
	// If specified location does not exist
	if (std::filesystem::exists(backLoc) == false) {
		std::cout << "Specified location does not exist ! Please specify a different location" << std::endl;
		return false;
	}

	// Backing up data
	// Creating folder
	backLoc = backLoc + "\\Pass-Man_Backup";
	std::filesystem::create_directory(backLoc);
	std::filesystem::copy(pm->getStringData("dataloc"), backLoc);

	return true;
}

// Method tp restore passwords from specific location
bool restorePass(std::string backLoc) {
	// If specified location does not exist
	if (std::filesystem::exists(backLoc) == false) {
		std::cout << "Specified backup location does not exist ! Please specify a different location" << std::endl;
		return false;
	}
	
	// Restoring data file by file
	for (auto& fileName : std::filesystem::directory_iterator(backLoc)) {
		// Trying to copy file
		try {
			std::filesystem::copy(std::filesystem::absolute(fileName.path()), pm->getStringData("dataloc"));
		}
		// If file already exists
		catch (std::exception e) {
			// Skip over to next file if this file already exists
			continue;
		}
	}

	return true;
}

// Method to change/create new authorization key
bool authKey() {
	std::string newAuthKey;										// To store new authorization key
	// Checking if previous authorization key present
	if (std::filesystem::exists(pm->getStringData("aufile")) == true) {
		std::cout << "Authorization key has already been set up. Do you wish to change it ?(y/n) : ";
		std::string aChoice;
		std::getline(std::cin, aChoice);
		// If yes
		if (aChoice.compare("y") == 0) {
			// Deleting previous auth key
			std::filesystem::remove(pm->getStringData("aufile"));
			
			// Changing authorization key
			sm->changeAuthKey();			
			return true;
		}
		else {
			return false;
		}
	}
	else {
		std::cout << "No authorization key has been set up. Do you wish to create one now?(y/n) : ";
		std::string aChoice;
		std::getline(std::cin, aChoice);
		// If yes
		if (aChoice.compare("y") == 0) {
			if (sm->changeAuthKey() == true) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
}

// Method to get mail-id to send notifications
bool setEmail() {
	bool isMailSent;											// To check if mail is sent
	std::string mailId;											// To store mail-id entered by user
	std::string oneTimePass;									// To store one time password
	std::string inputOtp;										// To store otp entered by user
	std::string uChoice;										// To store user choice

	// Checking if mail already set up
	// If does not exist
	if (std::filesystem::exists(pm->getStringData("mailloc")) == false) {
		std::cout << "Mail-id has not been set up. Do you want to set it now ?(y/n) : ";
	}
	// If exists
	else {
		std::cout << "Mail-id has already been set. Do you want to change it ?(y/n) : ";
	}
	getline(std::cin, uChoice);

	// If no
	if (toLower(uChoice).compare("y") != 0) {
		return false;
	}
																
	// Asking for mail id to send alert notifications
	std::cout << "Enter mail-id to send alert notifications to : ";
	std::getline(std::cin, mailId);

	// To remove mail-id
	if (mailId.compare("") == 0) {
		// If set up earlier
		if (std::filesystem::exists(pm->getStringData("mailloc")) == true) {
			// Removing mail-id
			std::filesystem::remove(pm->getStringData("mailloc"));
			std::cout << "Removed mail-id successfully !" << std::endl;
		}
		else {
			std::cout << "Mail-id not set up, hence not removed" << std::endl;
		}
		return false;
	}

	// Generating otp of length 6
	oneTimePass = randomString(6);

	// Sending otp
	isMailSent = sendMail(mailId, "OTP for email id verification of pass-man", "Your OTP to verify your mail-id is : " + oneTimePass);

	// Checking status
	if (isMailSent == false) {
		std::cout << "Please enter a valid e-mail id !" << std::endl;
		return false;
	}
	else {
		// Asking for otp
		std::cout << "OTP has been sent to the given mail-id. Enter it to verify it's you : ";
	}

	// Checking OTP
	std::getline(std::cin, inputOtp);
	if (inputOtp.compare(oneTimePass) != 0) {
		std::cout << "Wrong OTP! Email verification failed !" << std::endl;
		return false;
	}

	std::cout << "Email-id verification complete !" << std::endl;
	
	// Storing email to file
	outFile.open(pm->getStringData("mailloc"));
	outFile << cm->inpEncrypt(mailId) << std::endl;
	outFile.clear();
	outFile.close();	
	return true;
}
