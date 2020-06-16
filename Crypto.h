#include <string>

using namespace std;

string inpEncrypt(string name, int keyMul, int keyAdd);
string inpDecrypt(string inpStr, int keyMul, int keyAdd);
void enKey(ofstream& inFile);