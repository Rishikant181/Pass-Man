#pragma once

// Vars to handle file IO
extern std::ifstream inpFile;
extern std::ofstream outFile;

// Declaring class passMan
class passMan {	
public:
	// Member data declaration
	std::string workDir;											// To store working directory
	std::string dataLocation;										// To store location of stored data
	
	// Member method declaration
	passMan(std::string dir);										// Constructor
};