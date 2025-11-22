#include "../include/Generator.h"

using namespace std;

string generate_cpp(const vector<Token>& tokens) {
    string out;
    out += "#include <iostream>\n";
    out += "#include <vector>\n";
    out += "#include <string>\n";
    out += "using namespace std;\n\n";

    for (size_t i = 0; i < tokens.size(); i++) {
        Token t = tokens[i];
        switch (t.type) {
            case TOK_BOSS:    out += "int main() {\n"; break;
            case TOK_KHOTOM:  out += "return 0;\n}"; break;
            case TOK_DHOR:    out += "auto "; break;
            case TOK_DEKH:    out += "cout << "; break;
            case TOK_JODI:    out += "if "; break;
            case TOK_NAHOLE:  out += "else "; break;
            case TOK_CHOL:    out += "for "; break;
            case TOK_JOTOKHON: out += "while "; break;
            case TOK_BASTA:   out += "vector<int> "; break;

            case TOK_PORO:    out += "/* Input not supported yet */ "; break;
            case TOK_STRING:  out += "\"" + t.value + "\""; break;
            
            case TOK_PLUS: out += " + "; break;
            case TOK_MINUS: out += " - "; break;
            case TOK_MUL: out += " * "; break;
            case TOK_DIV: out += " / "; break;
            case TOK_MOD: out += " % "; break;
            case TOK_EQ: out += " = "; break;
            case TOK_EE: out += " == "; break;
            case TOK_LT: out += " < "; break;
            case TOK_GT: out += " > "; break;
            case TOK_LE: out += " <= "; break;
            case TOK_GE: out += " >= "; break;
            case TOK_NE: out += " != "; break;
            
            case TOK_LPAREN: out += " ("; break;
            case TOK_RPAREN: out += ") "; break;
            case TOK_LBRACE: out += " {\n"; break;
            case TOK_RBRACE: out += " }\n"; break;
            case TOK_LBRACKET: out += "{"; break;
            case TOK_RBRACKET: out += "}"; break;
            case TOK_SEMI:   out += ";\n"; break;
            case TOK_COMMA:  out += ", "; break;

            case TOK_NUMBER: 
            case TOK_IDENTIFIER: 
                out += t.value; 
                break;

            case TOK_EOF: break;
            default: break;
        }
    }
    return out;
}