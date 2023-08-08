#include "Screen.h"
#include "Screen.cpp"

using namespace std;

int main(){
    Screen actualScreen("login.json");
    actualScreen.PrintAllDatabases();
    actualScreen.PrintAllTables();
    actualScreen.GetInput();
    //actualScreen.MakeJsonFiles();
    //actualScreen.CreateTable();
    return 0;
}
