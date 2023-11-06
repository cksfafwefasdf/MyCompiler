//
// Created by cksfa on 2023/10/18.
//
#ifndef MYCOMPAILER_ANALYSIS_H
#define MYCOMPAILER_ANALYSIS_H

#include <string>
#include <vector>
#include <iostream>
#include "keyword.h"
using std::string;

class Analyzer{
public:
    string stringHold;
    std::map<int,string> alphabet;
    std::vector<string> tokens;
    void readText();
    void readTextFromFile(string path);
    std::vector<std::pair<string,int>> res;
    void analyze();
    void printResult();

private:
    int checkAlphabet(string token);
    bool isDigit(char c);
    bool isChar(char c);
};
#endif //MYCOMPAILER_ANALYSIS_H
