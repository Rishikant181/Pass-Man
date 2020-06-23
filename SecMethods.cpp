// This files contains various methods related to security
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "PassMan.h"
#include "Crypto.h"
#include "Helper.h"

// Using cli .net library
using namespace System::Net;

// Method to change previous auth key
bool changeAuthKey(passMan &ob) {
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
			std::filesystem::remove(ob.getStringMemberData("aufile"));
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
	outFile.open(ob.getStringMemberData("aufile"));
	outFile << inpEncrypt(ob, newAuthKey) << "\n";
	outFile.clear();
	outFile.close();

	return true;
}

// Method to check authorization
bool checkAuth(passMan &ob) {
	std::string inpAuthPass;										// To store input auth pass
	std::string actAuthPass;										// To store actual auth pass
	// Checking if authorization key set up
	if (std::filesystem::exists(ob.getStringMemberData("aufile")) == true) {
		// Taking input
		std::cout << "Enter authorization password : ";
		std::getline(std::cin, inpAuthPass);

		// Getting actual auth pass
		inpFile.open(ob.getStringMemberData("aufile"));
		std::getline(inpFile, actAuthPass);
		inpFile.clear();
		inpFile.close();

		// Checking for correct auth
		// For successful authorization
		if (inpAuthPass.compare(inpDecrypt(ob, actAuthPass)) == 0) {
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