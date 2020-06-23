#pragma once
#include <fstream>

bool addPass(passMan &ob, std::string refName);
bool getPass(passMan &ob, std::string refName);
void getList(passMan &ob);
bool editPass(passMan &ob, std::string refName);
bool delPass(passMan &ob, std::string refName);
bool backPass(passMan &ob, std::string backLoc);
bool restorePass(passMan &ob, std::string backLoc);
bool setEmail(passMan &ob);
bool authKey(passMan &ob);