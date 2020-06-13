// pass-man.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include "Crypto.h"

using namespace std;

int main()
{
    string inpPass;
    cout << "Enter a string : ";
    getline(cin, inpPass);
    cout << "Encrypted string : " << inpEncrypt(inpPass) << endl;
}