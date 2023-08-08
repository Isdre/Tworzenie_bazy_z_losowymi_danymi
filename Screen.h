//
// Created by User on 03.08.2023.
//

#ifndef GENEROWANIE_BAZ_DANYCH_SCREEN_H
#define GENEROWANIE_BAZ_DANYCH_SCREEN_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
//ScreenException

class Screen {
    std::vector<std::string> valueType;
    std::vector<std::string> constraintType;

    std::vector<std::string> databases;
    std::vector<std::string> tables;

    std::string tableFileName;
public:
    Screen() : valueType(), constraintType(), databases(), tables(), tableFileName() {};
    Screen(std::string loginFileName, std::string valueTypeFileName = "PostgreSQL_data_types.txt", std::string constrainsTypeFileName = "PostgreSQL_Constrains_Types.txt");
    ~Screen();
    void PrintAllDatabases();
    void PrintAllTables();
    void GetInput();
    //void MakeJsonFiles(std::string commend);
    //void CreateTable();
    Screen operator=(Screen screen);
};


#endif //GENEROWANIE_BAZ_DANYCH_SCREEN_H
