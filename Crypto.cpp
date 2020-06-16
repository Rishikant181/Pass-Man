// This file contains various methods related to encrypting and decrypting of data

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "PassMan.h"

using namespace std;

// Method to generate an encryption key
void enKey(ofstream& inFile) {
	// Variable declaration and initialisation
	srand(time(NULL));								// Seed value for rand()
	int ranMul = rand() % 100 + 1;					// To store multplicative part of key
	int ranAdd = rand() % 100 + 1;					// To store additive part of key
	
	if (inFile.is_open()) {
		inFile << ranMul << "\n";
		inFile << ranAdd << "\n";
	}
}

// Method to encrypt the input data
string inpEncrypt(string inpStr) {
	// Variable declaration and initialisation
	int lenPass = inpStr.length();					//To store the length of pass
	string enPass = "";								//To store the encrypted pass
	
	// Converting input string to encrypted format
	for (int i = 0; i < lenPass; i++) {
		int iChar = inpStr[i];						//To store each char ascii value
		// Encrypting the ascii value of char
		int eIChar = iChar * enKeyMul + enKeyAdd;
		// Storing encrypted pass
		enPass += (to_string(eIChar) + "-");
	}
	return enPass;
}

// Method to decrypt the input data
string inpDecrypt(string inpStr) {
	string enChar = "";								// To store each encrypted char
	string actPass = "";							// To store the actual password
	int inpLen = inpStr.length();					// To store length of input string
	for (int i = 0 ; i < inpLen; i++) {
		if (inpStr[i] != '-') {
			enChar += inpStr[i];
		}
		else {
			actPass += (char)((stoi(enChar) - enKeyAdd) / enKeyMul);
			enChar = "";
		}
	}
	return actPass;
}