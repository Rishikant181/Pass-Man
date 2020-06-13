// This file contains various methods related to encrypting and decrypting of data

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

// Method to generate an encryption key
void enKey() {
	srand(time(NULL));								// Seed value for rand()
	int ranMul = rand() % 100 + 1;					// To store multplicative part of key
	int ranAdd = rand() % 100 + 1;					// To store additive part of key
	// Saving encryption key to file
	ofstream outFile("EncryptKey.key");
	if (outFile.is_open()) {
		outFile << ranMul << "\n";
		outFile << ranAdd << "\n";
		cout << "Generated a new encryption key and saved it to file !" << endl;
		outFile.close();
	}
}

// Method to encrypt the input data
string inpEncrypt(string inpStr, int keyMul, int keyAdd) {
	int lenPass = inpStr.length();					//To store the length of pass
	string enPass = "";								//To store the encrypted pass
	for (int i = 0; i < lenPass; i++) {
		int iChar = inpStr[i];						//To store each char int value
		// Encrypting the ascii value of char
		int eIChar = iChar * keyMul + keyAdd;
		// Storing encrypted pass
		enPass += (to_string(eIChar) + "-");
	}
	return enPass;
}