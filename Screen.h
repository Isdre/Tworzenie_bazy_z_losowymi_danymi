//
// Created by User on 03.08.2023.
//

#ifndef GENEROWANIE_BAZ_DANYCH_SCREEN_H
#define GENEROWANIE_BAZ_DANYCH_SCREEN_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>

class ScreenException : public std::exception {
    private:
        char * message;

    public:
        ScreenException(char * msg) : message(msg) {};
        char * what () {
            return message;
        }
};

class Screen {

    std::vector<std::string> valueType;
    std::vector<std::string> constraintType;

    std::vector<std::string> databases;
    std::vector<std::string> tables;

    std::string loginFileName;
    std::string tableFileName;

    std::string GetInput(int request);

public:
    Screen() : valueType(), constraintType(), databases(), tables(), loginFileName(), tableFileName() {};
    Screen(std::string loginFileName, std::string valueTypeFileName = "PostgreSQL_data_types.txt", std::string constrainsTypeFileName = "PostgreSQL_Constrains_Types.txt");
    ~Screen();
    void PrintAllDatabases();
    void PrintAllTables();

    void MakeJsonFile();
    void CreateTable();
    Screen operator=(Screen screen);
};


#endif //GENEROWANIE_BAZ_DANYCH_SCREEN_H
