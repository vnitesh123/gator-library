#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
#include "red_black_tree.h"

// This function parses the input file of commands and executes 
// them in order until a Quit() command is called or EOF is reached
void parse(string inputFile) {
    Library library = Library();

    ifstream commandsFile(inputFile+".txt");

    // Read and Open the input txt file containing the commands to be executed
    if (!commandsFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return ;
    }

    // create an output file stream to write the output of each command executed to a txt file
    ofstream outputFile(inputFile+"_output_file.txt");


    string command;

    while(getline(commandsFile, command, '\n')){

            // find position of '(' , everything before that is the operation we want to perform
            string operation;
            size_t pos = command.find("(");
            operation = command.substr(0, pos);

            /*
                Here the operation value is compared with the valid operation names and if a match is found we execute the command
                Operations list:
                1. InsertBook() - adds a new book into the library
                2. PrintBook() - prints the book details of the bookID passed in the argument
                3. PrintBooks() - prints the book details of all books falling in a given range
                4. BorrowBook() - a book is assigned to the patron if available, else the patron is added to the reservation list
                5. ReturnNook() - called when a book is returned by patron, book's status is updated to available if no other reservation are made, else the book is reassigned to the patron who has highest priority
                6. DeleteBook() - removes a book from the library, aditionally notifies the patrons in the reservation list that the book is not available and cancels their reservations
                7. FindClosestBook() - finds the closest book from the ID passed in the argument
                8. ColorFlipCount() - returns the color flips performed so for in the Library Red Black tree.
                9. Quit() - terminates the program

            */ 

            if (operation == "InsertBook"){
                int arg1;
                string arg2, arg3, arg4;

                istringstream argStream(command.substr(pos + 1)); 
                // this is to ignore characters in between the desired arguments
                char ignore;

                // scans the arguments in the respective data types
                if (argStream >> arg1 >> ignore >> quoted(arg2) >> ignore >> quoted(arg3) >> ignore >> quoted(arg4) >> ignore && ignore == ')') {
                library.InsertBook(arg1, arg2, arg3, arg4);
                } else {
                    cerr << "Error parsing arguments." << endl;
                    return ; 
                }

            } else if (operation == "PrintBook"){
                int arg1;
                istringstream argStream(command.substr(pos + 1)); 
                // this is to ignore characters in between the desired arguments
                char ignore;

                // scans the arguments in the respective data types
                if (argStream >> arg1 >> ignore && ignore == ')') {
                string output = library.PrintBook(arg1);
                outputFile<<output;
                } else {
                    cerr << "Error parsing arguments." << endl;
                    return ; 
                }

            } else if (operation == "BorrowBook"){
                int arg1,arg2,arg3;

                istringstream argStream(command.substr(pos + 1)); 
                // this is to ignore characters in between the desired arguments
                char ignore;

                // scans the arguments in the respective data types
                if (argStream >> arg1 >> ignore >> arg2 >> ignore >> arg3 >> ignore && ignore == ')') {
                string output = library.BorrowBook(arg1, arg2, arg3);
                outputFile<<output;
                } else {
                    cerr << "Error parsing arguments." << endl;
                    return ; 
                }

            } else if (operation == "ReturnBook"){
                int arg1,arg2;

                istringstream argStream(command.substr(pos + 1)); 
                // this is to ignore characters in between the desired arguments
                char ignore;

                // scans the arguments in the respective data types
                if (argStream >> arg1 >> ignore >> arg2 >> ignore && ignore == ')') {
                string output = library.ReturnBook(arg1, arg2);
                outputFile<<output;
                } else {
                    cerr << "Error parsing arguments." << endl;
                    return ; 
                }

            } else if (operation == "PrintBooks"){
                int arg1, arg2;

                istringstream argStream(command.substr(pos + 1)); 
                // this is to ignore characters in between the desired arguments
                char ignore;

                // scans the arguments in the respective data types
                if (argStream >> arg1 >> ignore >> arg2 >> ignore && ignore == ')') {
                string output = library.PrintBooks(arg1, arg2);
                outputFile<<output;
                } else {
                    cerr << "Error parsing arguments." << endl;
                    return ; 
                }

            } else if (operation == "DeleteBook"){
                int arg1;
            
                istringstream argStream(command.substr(pos + 1));
                // this is to ignore characters in between the desired arguments
                char ignore;

                // scans the arguments in the respective data types
                if (argStream >> arg1 >> ignore && ignore == ')') {
                string output = library.DeleteBook(arg1);
                outputFile<<output;
                } else {
                    cerr << "Error parsing arguments." << endl;
                    return ; 
                }

            } else if (operation == "FindClosestBook"){
                int arg1;

                istringstream argStream(command.substr(pos + 1)); 
                // this is to ignore characters in between the desired arguments
                char ignore;

                // scans the arguments in the respective data types
                if (argStream >> arg1 >>  ignore && ignore == ')') {
                string output = library.FindClosestBook(arg1);
                outputFile<<output;
                } else {
                    cerr << "Error parsing arguments." << endl;
                    return ; 
                }

            } else if (operation == "ColorFlipCount"){
                string output = library.ColorFlipCount();
                outputFile<<output;

            } else if (operation == "Quit"){
                outputFile<< "Program Terminated!!";
                break;

            } else {
                cerr << "Invalid command." << endl;
                return ; 
            } 
       
    } 

    return ; 
}
