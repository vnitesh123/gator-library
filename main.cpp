#include<iostream>
#include<string>
#include<vector>

#include "parse_file.h"

using namespace std;

// accept arguments from the execution command
int main(int arg, char* argv[]){

    string fileName;
    fileName = argv[1];

    parse(fileName);
    return 0;
}