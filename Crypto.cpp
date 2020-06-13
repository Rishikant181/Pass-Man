// This file contains various methods related to encrypting and decrypting of data

#include <string>
#include <iostream>

using namespace std;

// Method to encrypt the input data
string inpEncrypt(string inpStr) {
	int lenPass = inpStr.length();					//To store the lenght of pass
	string enPass = "";								//To store the encrypted pass
	for (int i = 0; i < lenPass; i++) {
		int iChar = inpStr[i];						//To store each char int value
		// Encrypting the ascii value of char
		int eIChar = iChar * 7 + 13;
		// Storing encrypted pass
		enPass += (to_string(eIChar) + "-");
	}
	return enPass;
}