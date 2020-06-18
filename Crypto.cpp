// This file contains various methods related to encrypting and decrypting of data

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "PassMan.h"

// Method to generate an encryption key
void enKey() {
	// Variable declaration and initialisation
	std::srand(time(NULL));									// Seed value for rand()
	int ranMul = std::rand() % 100 + 1;						// To store multplicative part of key
	int ranAdd = std::rand() % 100 + 1;						// To store additive part of key
	
	// Opening encryption key file
	outFile.open(enFileName);

	if (outFile.is_open()) {
		outFile << ranMul << "\n";
		outFile << ranAdd << "\n";
	}

	// Closing enKeyFile
	outFile.clear();
	outFile.close();
}

// Method to encrypt the input data
std::string inpEncrypt(std::string inpStr) {
	// Variable declaration and initialisation
	int lenPass = inpStr.length();							//To store the length of pass
	std::string enPass = "";								//To store the encrypted pass
	
	// Converting input string to encrypted format
	for (int i = 0; i < lenPass; i++) {
		int iChar = inpStr[i];						//To store each char ascii value
		// Encrypting the ascii value of char
		int eIChar = iChar * enKeyMul + enKeyAdd;
		// Storing encrypted pass
		enPass += (std::to_string(eIChar) + "-");
	}
	return enPass;
}

// Method to decrypt the input data
std::string inpDecrypt(std::string inpStr) {
	std::string enChar = "";								// To store each encrypted char
	std::string actPass = "";								// To store the actual password
	int inpLen = inpStr.length()		;					// To store length of input string
	for (int i = 0 ; i < inpLen; i++) {
		if (inpStr[i] != '-') {
			enChar += inpStr[i];
		}
		else {
			actPass += (char)((std::stoi(enChar) - enKeyAdd) / enKeyMul);
			enChar = "";
		}
	}
	return actPass;
}