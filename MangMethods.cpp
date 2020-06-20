// This file contains various methods for management of stored passwords
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "Crypto.h"
#include "Helper.h"
#include "PassMan.h"

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

// Method to change previous auth key
bool changeAuthKey() {
	std::string newAuthKey;										// To store new auth key
	std::string conAuthKey;										// To store auth key confirmation

	std::cout << "Enter new authorization key : ";

	// Taking new auth key
	newAuthKey = getReqInput();

	// Confirming auth key
	std::cout << "Confirm authorization key   : ";
	std::getline(std::cin, conAuthKey);

	// Chekcing confirmation
	if (conAuthKey.compare(newAuthKey) != 0) {
		std::cout << "Confirmation failed !" << std::endl;
		return false;
	}

	// Storing new auth key
	outFile.open(auFileName);
	outFile << inpEncrypt(newAuthKey) << "\n";
	outFile.clear();
	outFile.close();

	return true;
}

// Method to add a new password to the database
bool addPass(std::string refName) {
	std::string passCom;										// To store commments about passwrds
	std::string actPass;										// To store the actual password
	std::string conPass;										// To store the confirmation password

	// Checking if duplicate refName
	if (std::filesystem::exists(dataLocation + refName + ".pass") == true) {
		std::cout << "Reference name already exists, please enter another name" << std::endl;
		return false;
	}

	std::cout << "Comments (Optional) : ";
	std::getline(cin, passCom);
	std::cout << "Enter password   : ";
	actPass = getReqInput();
	std::cout << "Confirm password : ";
	conPass = getReqInput();

	// Checking if conPass == actPass
	if (conPass.compare(actPass) == 0) {
		// Adding data to database
		outFile.open(dataLocation + refName + ".pass");
		outFile << inpEncrypt(actPass) << " " << passCom << "\n";
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
	if (std::filesystem::exists(dataLocation + refName + ".pass") == false) {
		// Exitting from program
		return false;
	}
	inpFile.open(dataLocation + refName + ".pass");
	std::getline(inpFile, enPass);
	inpFile.clear();
	inpFile.close();
	enPass = enPass.substr(0, enPass.find(' '));
	std::cout << inpDecrypt(enPass);
	return true;
}

// Method to get the list of all passwords stored by pass-man and displaying only refName and comments
void getList() {
	std::string refName;									// To store file name
	std::string passCom;									// To store comment
	// Iterating through directory
	for (auto& file : std::filesystem::directory_iterator(dataLocation)) {
		// Getting refName
		refName = file.path().string().substr(dataLocation.length());
		refName = refName.substr(0, refName.length() - 5);
		
		// Getting comment from file
		inpFile.open(dataLocation + refName + ".pass");
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
bool editPass(string refName) {
	int oprChoice;											// To store operation choice
	std::string choice;
	std::string line;										// To store the input from file
	std::string newRefName;									// To store new reference name
	std::string newPassCom;									// To store new comment
	std::string newPass;									// To store new password
	std::string newPassCon;									// To store new password confirmation
	// If refName exists
	if (std::filesystem::exists(dataLocation + refName + ".pass") == true) {
		// Storing original line temporarily
		inpFile.open(dataLocation + refName + ".pass");
		std::getline(inpFile, line);
		inpFile.clear();
		inpFile.close();
		
		// Asking for operation
		std::cout << "1. Change reference name" << std::endl;
		std::cout << "2. Change comment" << std::endl;
		std::cout << "3. Change password" << std::endl;
		
		try {
			// Taking choice
			std::getline(cin, choice);
			oprChoice = std::stoi(choice);
		}
		catch (invalid_argument ia) {
			std::cout << "Please enter number only !" << std::endl;
			return false;
		}

		switch (oprChoice) {
		// For changing reference name
		case 1: {
			std::cout << "Enter new reference name : ";
			newRefName = getReqInput();

			// Deleting old data
			std::filesystem::remove(dataLocation + refName + ".pass");

			// Writing new data
			outFile.open(dataLocation + newRefName + ".pass");
			outFile << line;
			outFile.clear();
			outFile.close();
			return true;
		}
		// For changing comment
		case 2: {
			std::cout << "Enter new comment : ";
			std::getline(cin, newPassCom);

			// Deleting old data
			std::filesystem::remove(dataLocation + refName + ".pass");

			// Writing new data
			outFile.open(dataLocation + refName + ".pass");
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
			std::filesystem::remove(dataLocation + refName + ".pass");

			// Writing new data
			outFile.open(dataLocation + refName + ".pass");
			outFile << inpEncrypt(newPass) + line.substr(line.find(' '));
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
	if (std::filesystem::exists(dataLocation + refName + ".pass") == false) {
		std::cout << "No such reference name found !" << std::endl;
		return false;
	}
	// If exists
	std::cout << "Are you sure you want to delete " << refName << "?(y/n) : ";
	std::getline(cin, delChoice);

	// If confirmed yes
	if (toLower(delChoice).compare("y") == 0) {
		// Deleting
		std::filesystem::remove(dataLocation + refName + ".pass");
		return true;
	}
	// If no
	else {
		std::cout << "Operation cancelled" << std::endl;
		return false;
	}
}

// Method to change/create new authorization key
bool authKey() {
	std::string newAuthKey;										// To store new authorization key
	// Checking if previous authorization key present
	if (std::filesystem::exists(auFileName) == true) {
		std::cout << "Authorization key has already been set up. Do you wish to change it ?(y/n) : ";
		std::string aChoice;
		std::getline(std::cin, aChoice);
		// If yes
		if (aChoice.compare("y") == 0) {
			// Deleting previous auth key
			std::filesystem::remove(auFileName);
			
			// Changing authorization key
			changeAuthKey();
			
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
			if (changeAuthKey() == true) {
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