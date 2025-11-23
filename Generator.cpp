#include "../include/Generator.h"

using namespace std;

string generate_cpp(const vector<Token>& tokens) {
    string out;
    out += "#include <iostream>\n";
    out += "#include <vector>\n";
    out += "#include <string>\n";
    out += "#include <sstream>\n";
    out += "using namespace std;\n\n";
    // We'll perform a simple statement-level generation so we can infer types
    // for `dhor` declarations and properly support `poro` input.
    for (size_t i = 0; i < tokens.size(); ) {
        Token t = tokens[i];
        if (t.type == TOK_BOSS) {
            out += "int main() {\n";
            i++;
            continue;
        }
        if (t.type == TOK_KHOTOM) {
            out += "    return 0;\n}\n";
            i++;
            continue;
        }

        // Handle variable declaration: dhor <id> [= expr] ;
        if (t.type == TOK_DHOR) {
            // expect identifier next
            size_t j = i + 1;
            string ident;
            if (j < tokens.size() && tokens[j].type == TOK_IDENTIFIER) {
                ident = tokens[j].value;
                j++;
            }

            // collect RHS tokens until semicolon
            vector<Token> rhs;
            if (j < tokens.size() && tokens[j].type == TOK_EQ) {
                j++; // skip '='
                while (j < tokens.size() && tokens[j].type != TOK_SEMI && tokens[j].type != TOK_EOF) {
                    rhs.push_back(tokens[j]);
                    j++;
                }
            }

            // Determine type by simple heuristic: if rhs contains TOK_STRING -> std::string
            // if contains TOK_NUMBER -> int (could refine to double), if contains TOK_PORO -> string (input)
            string declType = "auto";
            bool hasString = false, hasNumber = false, hasPoro = false;
            for (auto &rt : rhs) {
                if (rt.type == TOK_STRING) hasString = true;
                if (rt.type == TOK_NUMBER) hasNumber = true;
                if (rt.type == TOK_PORO) hasPoro = true;
            }
            if (hasPoro) declType = "std::string";
            else if (hasString) declType = "std::string";
            else if (hasNumber) declType = "int";

            // Emit declaration
            out += "    " + declType + " " + ident;

            if (!rhs.empty()) {
                out += " = ";
                // Special handling if RHS starts with poro: poro("msg")
                if (rhs.size() >= 1 && rhs[0].type == TOK_PORO) {
                    // Try to capture prompt inside parens following poro
                    string prompt;
                    size_t k = 1;
                    if (k < rhs.size() && rhs[k].type == TOK_LPAREN) k++;
                    if (k < rhs.size() && rhs[k].type == TOK_STRING) { prompt = rhs[k].value; }

                    // generate prompt and getline
                    out += ""; // assignment handled below
                    out += ";\n";
                    out += "    cout << \"" + prompt + "\";\n";
                    out += "    std::string __tmp_" + ident + ";\n";
                    out += "    std::getline(cin, __tmp_" + ident + ");\n";
                    if (declType == "int") {
                        out += "    " + ident + " = stoi(__tmp_" + ident + ");\n";
                    } else {
                        out += "    " + ident + " = __tmp_" + ident + ";\n";
                    }
                    // advance i to after semicolon
                    if (j < tokens.size() && tokens[j].type == TOK_SEMI) j++;
                    i = j;
                    continue;
                } else {
                    // Emit RHS tokens as-is with proper string quoting
                    for (auto &rt : rhs) {
                        if (rt.type == TOK_STRING) out += "\"" + rt.value + "\"";
                        else out += rt.value;
                    }
                    out += ";\n";
                }
            } else {
                out += ";\n";
            }

            i = j;
            continue;
        }

        // Handle dekh (print)
        if (t.type == TOK_DEKH) {
            // collect until semicolon
            size_t j = i + 1;
            string expr;
            while (j < tokens.size() && tokens[j].type != TOK_SEMI && tokens[j].type != TOK_EOF) {
                Token rt = tokens[j];
                if (rt.type == TOK_STRING) expr += "\"" + rt.value + "\"";
                else expr += rt.value;
                j++;
            }
            out += "    cout << (" + expr + ") << endl;\n";
            if (j < tokens.size() && tokens[j].type == TOK_SEMI) j++;
            i = j;
            continue;
        }

        // Simple flow constructs and braces
        if (t.type == TOK_JODI) { out += "    if "; i++; continue; }
        if (t.type == TOK_NAHOLE) { out += "    else "; i++; continue; }
        if (t.type == TOK_CHOL) { out += "    for "; i++; continue; }
        if (t.type == TOK_JOTOKHON) { out += "    while "; i++; continue; }
        if (t.type == TOK_LBRACE) { out += "    {\n"; i++; continue; }
        if (t.type == TOK_RBRACE) { out += "    }\n"; i++; continue; }

        // Operators, parentheses, identifiers, numbers - fall back to simple emission
        if (t.type == TOK_STRING) out += "\"" + t.value + "\"";
        else out += t.value;
        i++;
    }

    return out;
}
