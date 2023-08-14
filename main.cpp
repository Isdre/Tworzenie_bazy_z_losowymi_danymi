#include "Screen.h"
#include "Screen.cpp"


int main(){
    Screen actualScreen("login.json");
    actualScreen.MakeJsonFile();
    actualScreen.CreateTable();
    return 0;
}
