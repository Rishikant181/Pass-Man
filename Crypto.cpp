// This file contains various methods related to encrypting and decrypting of data

#include "GlobalMembers.h"

// Defining member methods
// Defining constructor
cryptMan::cryptMan(std::string workDir) {
	enKeyFile = workDir;
}

// Defining method enKey
void cryptMan::enKey() {
	// Variable declaration and initialisation
	std::srand(time(NULL));									// Seed value for rand()
	int ranMul = std::rand() % 100 + 1;						// To store multplicative part of key
	int ranAdd = std::rand() % 100 + 1;						// To store additive part of key
	
	// Opening encryption key file
	outFile.open(enKeyFile);

	if (outFile.is_open()) {
		outFile << ranMul << "\n";
		outFile << ranAdd << "\n";
	}

	// Closing enKeyFile
	outFile.clear();
	outFile.close();
}

// Defining getKey method
void cryptMan::getKey() {
	// Getting encryption key
	inpFile.open(enKeyFile);
		
	// Setting encryption key from file
	std::string line;                                   // To store each line from EncryptKey.key file

	// Getting mul val
	std::getline(inpFile, line);
	enKeyMul = std::stoi(line);

	// Getting add val
	std::getline(inpFile, line);
	enKeyAdd = std::stoi(line);

	// Closing file
	inpFile.clear();
	inpFile.close();
}

// Defining method inpEncrypt
std::string cryptMan::inpEncrypt(std::string inpStr) {
	// Variable declaration and initialisation
	int lenPass = inpStr.length();							//To store the length of pass
	std::string enPass = "";								//To store the encrypted pass
	
	// Converting input string to encrypted format
	for (int i = 0; i < lenPass; i++) {
		int iChar = inpStr[i];						//To store each char ascii value
		// Encrypting the ascii value of char
		int eIChar = iChar * enKeyMul + enKeyAdd;
		// Storing encrypted pass
		enPass = enPass + (std::to_string(eIChar) + "-");
	}
	return enPass;
}

// Defining method inpDecypt
std::string cryptMan::inpDecrypt(std::string inpStr) {
	std::string enChar = "";								// To store each encrypted char
	std::string actPass = "";								// To store the actual password
	int inpLen = inpStr.length()		;					// To store length of input string
	for (int i = 0 ; i < inpLen; i++) {
		if (inpStr[i] != '-') {
			enChar = enChar + inpStr[i];
		}
		else {
			actPass = actPass + (char)((std::stoi(enChar) - enKeyAdd) / enKeyMul);
			enChar = "";
		}
	}
	return actPass;
}