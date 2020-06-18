// This file contains various methods for management of stored passwords
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "Crypto.h"
#include "Helper.h"
#include "PassMan.h"

// Method to check for duplicate refName, return false if not present and true otherwise
bool checkRefName(ifstream& inpFile, string inpRefName) {
	std::string line;										// To store each line of inpFile
	std::string refName;									// To store extracted refName from file
	inpRefName = toLower(inpRefName);

	// Getting each line from file
	while (true) {
		std::getline(inpFile, line);
		// Checking for blank line, i.e, end of file
		if (line.compare("") == 0) {
			return false;
		}
		// For non-blank line
		refName = toLower(line.substr(0, line.find(',')));
		
		// Checking for match
		if (refName.compare(inpRefName) == 0) {
			return true;
		}
	}
	// For not present
	return false;
}

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
			std::cout << "Please enter a valid input !" << endl;
		}
	}
}

// Method to add a new password to the database
bool addPass(std::string refName) {
	std::string passCom;										// To store commments about passwrds
	std::string actPass;										// To store the actual password
	std::string conPass;										// To store the confirmation password

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
	if (filesystem::exists(dataLocation + refName + ".pass") == false) {
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
	for (auto& file : filesystem::directory_iterator(dataLocation)) {
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
		std::cout << "Reference Name : " << refName << endl;
		std::cout << "Comments       : " << passCom << "\n" << endl;
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
	if (filesystem::exists(dataLocation + refName + ".pass") == true) {
		// Storing original line temporarily
		inpFile.open(dataLocation + refName + ".pass");
		std::getline(inpFile, line);
		inpFile.clear();
		inpFile.close();
		
		// Asking for operation
		std::cout << "1. Change reference name" << endl;
		std::cout << "2. Change comment" << endl;
		std::cout << "3. Change password" << endl;
		
		try {
			// Taking choice
			std::getline(cin, choice);
			oprChoice = std::stoi(choice);
		}
		catch (invalid_argument ia) {
			std::cout << "Please enter number only !" << endl;
			return false;
		}

		switch (oprChoice) {
		// For changing reference name
		case 1: {
			std::cout << "Enter new reference name : ";
			newRefName = getReqInput();

			// Deleting old data
			filesystem::remove(dataLocation + refName + ".pass");

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
			filesystem::remove(dataLocation + refName + ".pass");

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
				std::cout << "Confirmation failed !" << endl;
				return false;
			}
			
			// Deleting old data
			filesystem::remove(dataLocation + refName + ".pass");

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
		std::cout << "Reference name not found, password does not exist" << endl;
		return false;
	}
}

// Method to delete a stored password
bool delPass(std::string refName) {
	std::string delChoice;											// To store confirmation choice
	// Checking if reference name does not exist
	if (filesystem::exists(dataLocation + refName + ".pass") == false) {
		std::cout << "No such reference name found !" << endl;
		return false;
	}
	// If exists
	std::cout << "Are you sure you want to delete " << refName << "?(y/n) : ";
	std::getline(cin, delChoice);

	// If confirmed yes
	if (toLower(delChoice).compare("y") == 0) {
		// Deleting
		filesystem::remove(dataLocation + refName + ".pass");
		return true;
	}
	// If no
	else {
		std::cout << "Operation cancelled" << endl;
		return false;
	}
}