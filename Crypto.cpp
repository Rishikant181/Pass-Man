// This file contains various methods related to encrypting and decrypting of data

#include "GlobalMembers.h"

// Defining member methods
// Defining constructor
cryptMan::cryptMan(std::string authPass) {
	base = authPass.length();
	enKey = authPass;
	enKeyMul = 1;
	enKeyAdd = 0;

	// Initialising delimiting chars
	delimChars = "klmnopqrstuvwxyz";

	// Initialsing encryption keys
	for (int i = 0; i <= 7; i++) {
		long long num = std::stoll(std::to_string(authPass[i]));
		
		// For enKeyMul
		if (i % 2 == 0) {
			enKeyMul = enKeyMul * num;
		}
		// For enKeyAdd
		else {
			enKeyAdd = enKeyAdd + num;
		}
	}
}

// Defining method inpEncrypt
std::string cryptMan::inpEncrypt(std::string inpStr) {
	int inpStrLen = inpStr.length();									// To store inputStr length
	std::string enStr = "";												// To store encrypted string

	// Encrypting
	for (int i = 0; i < inpStrLen; i++) {
		enStr = enStr + toBase(inpStr[i] * enKeyMul - enKeyAdd, base) + delimChars[rand() % 16];
	}

	return enStr;
}

// Defining method inpDecypt
std::string cryptMan::inpDecrypt(std::string inpStr) {
	int inpStrLen = inpStr.length();									// To store inputStr length
	std::string deStr = "";												// To store decrypted string
	std::string enChar = "";											// To store each encrypted char

	// Decrypting
	for (int i = 0; i < inpStrLen; i++) {
		// If no delimiter
		if (delimChars.find(inpStr[i]) == std::string::npos) {
			enChar = enChar + inpStr[i];
		}
		// If delimiter
		else {
			// Getting original ascii val and converting to char
			char realChar = (toDeci(enChar, base) + enKeyAdd) / enKeyMul;
			deStr = deStr + realChar;
			enChar = "";
		}
	}

	return deStr;
}