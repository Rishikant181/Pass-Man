#pragma once
#include <string>
#include <fstream>

// Vars to handle file IO
extern std::ifstream inpFile;
extern std::ofstream outFile;

// Declaring class passMan
class passMan {
	// Member data declaration
	// Vars to store encryption key
private:
	int enKeyMul;
	int enKeyAdd;

	// Vars to store directory locations
	std::string dataLocation;										// To store location of stored data
	std::string enFileName;											// To store location of encrption key file
	std::string auFileName;											// To store location of authentication key file
	std::string mailIdLoc;											// To store location of mail id location

public:
	// Member method declaration
	// Constructor
	passMan(std::string workDir);

	// Method to get encryption key
	void getKey();

	// Method to supply various private member data(int)
	int getIntMemberData(std::string datName);

	// Method to supply various private member data(string)
	std::string getStringMemberData(std::string datName);
};