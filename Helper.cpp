// This file contains various helper methods

#include "GlobalMembers.h"

// Method to convert a string to lowercase
std::string toLower(std::string inpStr) {
	int strLen = inpStr.length();										// To store length of string
	for (int i = 0; i < strLen; i++) {
		inpStr[i] = tolower(inpStr[i]);
	}
	return inpStr;
}

// Method to get a substring from a string between two occurences of a giver char
std::string subStr(std::string line, char reqChar, int firstOccur, int secondOccur) {
	int numOccur = 0;													// To store occurence number of reqChar
	int lineLen = line.length();										// To store length of line
	std::string outStr = "";											// To store output substring
	for (int i = 0; i < lineLen; i++) {
		// Checking for the char
		if (line[i] == reqChar) {
			numOccur++;
		}

		// Getting required substring
		else if (numOccur >= firstOccur && numOccur < secondOccur) {
			outStr += line[i];
		}

		// Returning value
		else if (numOccur >= secondOccur) {
			return outStr;
		}
	}
	return outStr;
}

// Method to raise a number to a given power and return in long long data type
long long toPower(long long base, long long power) {
	long long outNum = 1;
	for (int i = 0; i < power; i++) {
		outNum = outNum * base;
	}
	return outNum;
}

// Method to generate a random alphanumeric string of length l
std::string randomString(int lenString) {
	std::srand(time(NULL));
	std::string randString = "";										// To store the random string
	int numSet[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };					// To store all numbers
	std::string charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";					// To store char set
	for (int i = 0; i < lenString; i++) {
		int a = rand() % 2;
		int b = rand() % 10;
		int c = rand() % 26;
		if (a == 0) {
			randString = randString + std::to_string(numSet[b]);
		}
		else {
			randString = randString + charSet[c];
		}
	}

	return randString;
}

// Method to convert a decimal number to any given base
std::string toBase(long long inpNum, int base) {
	std::string numSet = "0123456789";									// To store number set
	std::string alpSet = "abcdefghij";									// To store alphabet set
	std::string outNum = "";											// To store output number
	int numNum = base / 2;												// To store number of numbers to use
	int numAlp = base - numNum;											// To store number of alphabets to use

	// Converting to base
	while (inpNum != 0) {
		// Getting each digit
		int rem = inpNum % base;

		// If digit is used
		if (rem < numNum) {
			outNum = numSet[rem] + outNum;
		}
		// If alphabet used
		else {
			outNum = alpSet[rem - numNum] + outNum;
		}

		inpNum = inpNum / base;
	}

	return outNum;
}

// Method to convert from a specified base to decimal
long long toDeci(std::string inpNum, int base) {
	std::string numSet = "0123456789";									// To store number set
	std::string alpSet = "abcdefghij";									// To store alphabet set
	std::string realVal = "";											// To store real face value of each symbol
	long long outNum = 0;												// To store output number
	int numNum = base / 2;												// To store number of numbers to use
	int numAlp = base - numNum;											// To store number of alphabets to use

	// Getting real face values of each symbol
	for (int i = 0; i < base; i++) {
		if (i < numNum) {
			realVal = realVal + numSet[i];
		}
		else {
			realVal = realVal + alpSet[i - numNum];
		}
	}

	// Converting to decimal
	for (int i = inpNum.length() - 1; i >= 0; i--) {
		outNum = outNum + (realVal.find(inpNum[i]) * toPower(base, inpNum.length() - 1 - i));
	}

	return outNum;
}