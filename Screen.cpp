//
// Created by User on 03.08.2023.
//

#include "Screen.h"

std::string execute(const std::string& command) {
    system((command + " > temp.txt").c_str());

    std::ifstream ifs("temp.txt");
    std::string ret{ std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
    ifs.close(); // must close the inout stream so the file can be cleaned up
    if (std::remove("temp.txt") != 0) {
        perror("Error deleting temporary file");
    }
    return ret;
}

Screen::Screen(std::string loginFileName, std::string valueTypeFileName, std::string constrainsTypeFileName) : valueType(), constraintType(), databases(), tables(){
    std::cout<< "Screen - start making" <<std::endl;
    std::string output = execute("echo python connect.py " + loginFileName + " databases");
    //przypisz do this.databases
    std::string word = "";
    for (auto x : output)
    {
        if (x == ' ')
        {
            this->databases.push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }

    output = execute("echo python connect.py " + loginFileName + " tables");
    //przypisz do this.tables
    word = "";
    for (auto x : output)
    {
        if (x == ' ')
        {
            this->tables.push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    //pobierz z valueTypeFileName dane do this.valueType
    std::fstream plik;
    plik.open( valueTypeFileName, std::ios::in);
    if( plik.good() ){
        while(!plik.eof()){
            getline(plik, word);
            this->valueType.push_back(word);
        }
        plik.close();
    }
    //pobierz z constrainsTypeFileName dane do this.constraintType
    plik.open( constrainsTypeFileName, std::ios::in);
    if( plik.good() ){
        while(!plik.eof()){
            getline(plik, word);
            this->constraintType.push_back(word);
        }
        plik.close();
    }
    std::cout<< "Screen - done" <<std::endl;
    std::system("cls");
}

Screen::~Screen() {
    std::cout<< "~Screen" <<std::endl;
}

void Screen::PrintAllDatabases() {
    std::cout<< "Databases:" <<std::endl;
    for(std::string x : this->databases) {
        std::cout<<"- "<<x<<std::endl;
    }
}

void Screen::PrintAllTables() {
    std::cout<< "Tables:" <<std::endl;
    for(std::string x : this->tables) {
        std::cout<<"- "<<x<<std::endl;
    }
}

void Screen::GetInput() {
    std::cout<< "GetInput..." <<std::endl;
    //wypisz this.valueType
    //wypisz this.constraintType
    std::cout<< "Value types:" <<std::endl;
    for(auto x : this->valueType) {
        std::cout<<x<<std::endl;
    }
    std::cout<< "Constraint types: "<<std::endl;
    for(auto x : this->constraintType) {
        std::cout<<x<<std::endl;
    }

    //pobierz komende
    //wywował MakeJsonFiles(std::string commend);
}
/*
void Screen::MakeJsonFiles(std::string commend) {
    std::cout<< "MakeJsonFiles:" <<std::endl;
    //stwórz plik.json odpowiadający do example.json
    //przypisz nazwe pliku do tableFileName
}

void Screen::CreateTable() {
    std::cout<< "CreateTable:" <<std::endl;
    //wywołaj python connect.py loginFileName tableFileName
}
*/

Screen Screen::operator=(Screen screen) {
    valueType = screen.valueType;
    constraintType = screen.constraintType;
    databases = screen.databases;
    tables = screen.tables;
    tableFileName = screen.tableFileName;
    return *this;
}