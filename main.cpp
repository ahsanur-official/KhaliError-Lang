#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Lexer.h"
#include "../include/Generator.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: khali_error <filename.ke>" << endl;
        return 1;
    }

    string srcFile = argv[1];
    string cppFile = "output.cpp";
    string exeFile = "program.exe";

    ifstream in(srcFile);
    if (!in) {
        cerr << "Error: File '" << srcFile << "' khuje pacchi na boss!" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << in.rdbuf();
    string src = buffer.str();
    in.close();

    vector<Token> tokens = tokenize(src);
    string code = generate_cpp(tokens);

    ofstream out(cppFile);
    out << code;
    out.close();

    cout << "[KhaliError] Compiling: " << srcFile << "..." << endl;

    string command = "g++ " + cppFile + " -o " + exeFile;
    int result = system(command.c_str());

    if (result == 0) {
        cout << "[KhaliError] Success! Ekhon run koro: " << exeFile << endl;
        // system(exeFile.c_str()); // Auto run
    } else {
        cerr << "[KhaliError] Compilation Failed! Code e jhamela ache." << endl;
    }

    return 0;
}