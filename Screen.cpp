//
// Created by User on 03.08.2023.
//

#include "Screen.h"

std::string exec(std::string command) {
    char buffer[256];
    std::string result = "";

    // Open pipe to file
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe)) {

        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return result;
}

Screen::Screen(std::string valueTypeFileName, std::string constrainsTypeFileName, std::string loginFileName) {
    std::string output = exec("python connect.py " + loginFileName + " databases");
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


    output = exec("python connect.py" + loginFileName + " tables");
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
/*
void Screen::GetInput() {
    std::cout<< "GetInput:" <<std::endl;
    //wypisz this.valueType
    //wypisz this.constraintType
    //pobierz komende
    //wywował MakeJsonFiles(std::string commend);
}

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