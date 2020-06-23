#pragma once
#include <string>
#include "PassMan.h"

using namespace std;

string inpEncrypt(passMan &ob, string name);
string inpDecrypt(passMan &ob, string inpStr);
void enKey(passMan &ob);