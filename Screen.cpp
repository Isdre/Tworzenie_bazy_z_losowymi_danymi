//
// Created by User on 03.08.2023.
//

#include "Screen.h"

std::string execute(const std::string& command) {
    //std::cout<<command<<std::endl;
    system((command + " > temp.txt").c_str());

    std::string out = "";
    std::string w;
    std::fstream plik;
    plik.open( "temp.txt", std::ios::in);
    if( plik.good() ){
        while(!plik.eof()){
            getline(plik, w);
            out += w;
            out += " ";
        }
        plik.close();
    }

    if (std::remove("temp.txt") != 0) {
        perror("Error deleting temporary file");
    }

    //std::cout<<out<<std::endl;
    return out;
}

Screen::Screen(std::string loginFileName, std::string valueTypeFileName, std::string constrainsTypeFileName) : valueType(), constraintType(), databases(), tables(), loginFileName(loginFileName){
    std::system(std::string("python -m pip install -r requirements.txt").c_str());
    std::cout<< "Screen - start making" <<std::endl;
    std::string output = execute("python connect.py " + loginFileName + " databases");
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
    this->databases.pop_back();
    output = execute("python connect.py " + loginFileName + " tables");
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
    this->tables.pop_back();
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

void Screen::MakeJsonFile() {
    std::fstream plik;
    tableFileName = "temp";
    plik.open(tableFileName + ".txt", std::ios::out);
    if( !plik.good() ) return;
    plik << "{\n";
    plik << GetInput(0);
    plik << GetInput(1);
    std::cout<< "How many column you want?: ";
    int x;
    std::cin>>x;
    plik << "\"table_count\": " + std::to_string(x) + ",\n";
    for (int i=0; i<x; i++) {
        plik << "\"column_" + std::to_string(i) + "\": {\n";
        plik << GetInput(2);
        if (i != x-1) plik << "},\n";
        else plik << "}\n";
    }
    plik << "}\n";
    plik.close();
    rename((tableFileName + ".txt").c_str(),(tableFileName + ".json").c_str());

}

std::string Screen::GetInput(int request) {
    if(request == 0) {
        this->PrintAllDatabases();
        std::string database = "";
        while (true) {
            std::cout << "Write name of database you want to use: ";
            std::cin >> database;
            std::vector<std::string>::iterator it;
            it = std::find(this->databases.begin(), this->databases.end(), database);
            if (it == this->databases.end()) {
                std::cout << "Databases doesn't exist.\n";
            } else return std::string("\"database\":\"" + database + "\",\n");
        }
    }
    else if(request == 1) {
        this->PrintAllTables();
        std::string table = "";
        while (true) {
            std::cout << "Write name of table you want to create: ";
            std::cin >> table;
            std::vector<std::string>::iterator it;
            it = std::find(this->tables.begin(), this->tables.end(), table);
            if (it == this->tables.end()) return std::string("\"table_name\":\"" + table + "\",\n");
        }
    }
    else if(request == 2) {
        std::string out;

        std::string column_name;
        std::string column_type;
        std::vector<std::string> constraints;

        std::cout << "Write name: ";
        std::cin >> column_name;
        out += "\"column_name\": \"";
        out += column_name;
        out += "\",\n";

        //wypisz this.valueType
        std::cout<< "Value types:" <<std::endl;
        for(auto x : this->valueType) {
            std::cout<<x<<std::endl;
        }
        while (true) {
            std::cout << "Write type: ";
            std::getline(std::cin,column_type);
            std::vector<std::string>::iterator it;
            it = std::find(this->valueType.begin(), this->valueType.end(), column_type);
            if (it != this->valueType.end()) break;
        }

        out += "\"column_type\": \"";
        out += column_type;
        out += "\",\n";

        //wypisz this.constraintType
        std::cout<< "Constraint types: "<<std::endl;
        for(auto x : this->constraintType) {
            std::cout<<x<<std::endl;
        }

        while (true) {
            std::string x;
            std::cout << "Write contraint one at a time(STOP to end write contraints): ";
            std::getline(std::cin,x);
            if (x == "STOP") break;
            std::vector<std::string>::iterator it;
            it = std::find(this->constraintType.begin(), this->constraintType.end(), x);
            if (it != this->constraintType.end()) {
                it = std::find(constraints.begin(), constraints.end(), x);
                if (constraints.size() == 0 or it == constraints.end()) {
                    constraints.push_back(x);
                }
            }
        }

        out += "\"constrains\": [\n";
        for (int j=0; j<constraints.size(); j++) {
            out += "\"" + constraints[j];
            if (j == constraints.size()-1) out += "\"\n";
            else out += "\",\n";
        }
        out += "]\n";

        return out;
    }
    throw ScreenException("Request with that code doesn't exist.\n");
}


void Screen::CreateTable() {
    std::cout<< "CreateTable:" <<std::endl;
    //wywołaj python connect.py loginFileName tableFileName
    std::system(std::string("python connect.py " + this->loginFileName + " " + this->tableFileName + ".json").c_str());

    if (std::remove((tableFileName + ".json").c_str()) != 0) {
        perror("Error deleting temporary file");
    }
}


Screen Screen::operator=(Screen screen) {
    valueType = screen.valueType;
    constraintType = screen.constraintType;
    databases = screen.databases;
    tables = screen.tables;
    tableFileName = screen.tableFileName;
    return *this;
}