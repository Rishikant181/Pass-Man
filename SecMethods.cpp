// This files contains various methods related to security
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "PassMan.h"
#include "Crypto.h"

// Method to check authorization
bool checkAuth() {
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

		// Checking for correct auth
		// For successful authorization
		if (inpAuthPass.compare(inpDecrypt(actAuthPass)) == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	// Default case when no authorization has been set up
	std::cout << "*WARNING* No authorization has been set up. Your passwords are vulenrable !" << endl;
	return true;
}