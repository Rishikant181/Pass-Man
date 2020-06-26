#pragma once

// Declaring class cryptMan
class cryptMan {
private:
	// Member data
	int base;															// To store base
	std::string enKey;													// To store enryption key
	std::string delimChars;												// To store delimiting chars
	long long enKeyMul;													// To store encryption key multiplicative part
	long long enKeyAdd;													// To store encryption key additive part

public:
	// Member method
	cryptMan(std::string authPass);										// Constructor
	std::string inpEncrypt(std::string inpStr);							// Method to encrypt input data
	std::string inpDecrypt(std::string inpStr);							// Method to decrypt input data
};