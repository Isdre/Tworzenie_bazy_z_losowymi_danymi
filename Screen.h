//
// Created by User on 03.08.2023.
//

#ifndef GENEROWANIE_BAZ_DANYCH_SCREEN_H
#define GENEROWANIE_BAZ_DANYCH_SCREEN_H

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
//ScreenException

class Screen {
    std::vector<std::string> valueType;
    std::vector<std::string> constraintType;

    std::vector<std::string> databases;
    std::vector<std::string> tables;

    std::string tableFileName;
public:
    Screen(std::string valueTypeFileName = "PostgreSQL_data_types.txt", std::string constrainsTypeFileName = "PostgreSQL_Constrains_Types.txt",std::string loginFileName = "login.json");
    ~Screen();
    void PrintAllDatabases();
    void PrintAllTables();
    //void GetInput();
    //void MakeJsonFiles(std::string commend);
    //void CreateTable();
};


#endif //GENEROWANIE_BAZ_DANYCH_SCREEN_H
