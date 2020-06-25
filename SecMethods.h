#pragma once

// Declaring class secMan
class secMan {
public:
	// Member data
	std::string auFileName;												// To store authentication file name
	std::string mailIdLoc;												// To store mail-id file location

	// Member methods
	secMan(std::string dir);											// Constructor
	bool checkAuth();													// Method to check authentication
	bool changeAuthKey();												// Method to change authentication key
};