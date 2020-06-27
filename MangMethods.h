#pragma once

bool addPass(std::string refName);
bool getPass(std::string refName);
void getList();
bool editPass(std::string refName);
bool delPass(std::string refName);
bool backPass(std::string backLoc);
bool restorePass(std::string backLoc);
bool setEmail();
bool authKey();
void convertData(cryptMan* oldcm, cryptMan* newcm);