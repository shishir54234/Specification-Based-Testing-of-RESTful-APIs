#include <iostream>
#include <bits/stdc++.h>

#include "../ast.hpp"
#include "jsCodeGen.h"

using std::regex;
using namespace std;


CodeGenerator::CodeGenerator(Visitor *v) : visitor(v) {}

string CodeGenerator::indent(string line, string istring, int level) {
	string indentation = "";
	for(int i = 0; i < level; i++) {
		indentation += istring;
	}
	line = indentation + line;
	return line;
}

string CodeGenerator::generateCode(Program& program) {
    visitor->visitProgram(program);
    string raw = visitor->retrieve();
    regex del("\n");

    sregex_token_iterator it(raw.begin(), raw.end(), del, -1);
    sregex_token_iterator end;
    vector<string> lines;
    while (it != end) {
        lines.push_back(*it);
        ++it;
    }
	int level = 0;
    for(auto& line : lines) {
		if(line.find("{") != string::npos) {
			line = indent(line, "    ", level);
			level++;
		}
		else if(line.find("}") != string::npos) {
			level--;
			line = indent(line, "    ", level);
		}
		else {
			line = indent(line, "    ", level);
		}
    }
    string indentedCode = "";
    for(auto& line : lines) {
	    indentedCode += line + '\n';
    }
    return indentedCode;
}

CodeGenerator::~CodeGenerator() {
    delete visitor;
}

string Visitor::pop(stack<string>& s) {
    string str = s.top();
    s.pop();
    return str;
}


string Visitor::retrieve() {
    return pop(strings);
}


