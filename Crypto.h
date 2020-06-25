#pragma once

class cryptMan {
private:
	// Member data
	int enKeyMul;														// To store encryption key multiplicative part
	int enKeyAdd;														// To store encryption key additive part

public:
	// Member data
	std::string enKeyFile;												// To store encryption key file name
	
	// Member method
	cryptMan(std::string workDir);										// Constructor
	void enKey();														// Method to generate encryption key
	void getKey();														// Method to load encryption key
	std::string inpEncrypt(std::string name);							// Method to encrypt input data
	std::string inpDecrypt(std::string inpStr);							// Method to decrypt input data
};