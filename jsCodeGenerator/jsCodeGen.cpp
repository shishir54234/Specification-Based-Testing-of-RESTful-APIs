#include <iostream>
#include <bits/stdc++.h>

#include "jsCodeGen.h"
#include "../ast.hpp"

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
    // cout<<"visited program\n";
    string raw = visitor->retrieve();
    regex del("\n");

    sregex_token_iterator it(raw.begin(), raw.end(), del, -1);
    sregex_token_iterator end;
    vector<string> lines;
    while (it != end) {
        // cout<<"line: "<<*it<<endl;
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
    // cout<<"indented code: "<<indentedCode;
    return indentedCode;
}


void ExpoSEVisitor::visitExpr(Expr& e) {
    switch(e.expressionType) {
        // case EMPTY: visitEmpty((Empty&)e); break;
        case VAR:   visitVar((Var&)e); break;
        case NUM:   visitNum((Num&)e); break;
        case FUNCTIONCALL_EXPR: visitFuncCall((FuncCall&)e); break;
        case MAP: visitMap((Map&)e); break;
        case SET: visitSet((Set&)e); break;
        // case TUPLE: visitTuple((Tuple&)e); break;
        default: throw "Visitor::visitExpression - Unknown expression type.";
    }
}

// void ExpoSEVisitor::visitTypeExpr(TypeExpr& t) {
//     switch(t.typeExpression) {
//         case TypeExpression::FUNC_TYPE: visitFuncType((FuncType&)t); break;
//         case TypeExpression::MAP_TYPE: visitMapType((MapType&)t); break;
//         case TypeExpression::SET_TYPE: visitSetType((SetType&)t); break;
//         case TypeExpression::TUPLE_TYPE: visitTupleType((TupleType&)t); break;
//         case TypeExpression::TYPE_CONST: visitTypeConst((TypeConst&)t); break;
//         case TypeExpression::TYPE_VARIABLE: visitVar((Var&)t); break;
//         // default: throw "Visitor::visitTypeExpr - Unknown type expression type.";
//     }
// }

// void ExpoSEVisitor::

// void ExpoSEVisitor::visitDecl(Decl& d) {
//     d.accept(this);
//     string name = d.name;
//     string type = pop(strings);
//     strings.push(type + " " + name);
// }

// void ExpoSEVisitor::visitFuncDecl(FuncDecl& f) {
//     f.accept(this);
//     string name = f.name;
//     // string type = pop(strings);
//     string params = "";
//     int counter =0;
//     while(f.params.size()==counter)
//     {
//         string param = pop(strings);
//         params += param + ",";
//         counter++;
//     }
//     counter =0;
//     while(f.returnType.second.size()==counter)
//     {
//         string param = pop(strings);
//     }
//     // string args = pop(strings);
//     // strings.push(type + " " + name);
// }

void ExpoSEVisitor::visitFuncCallStmt(FuncCallStmt& f) {
    f.accept(this);
    string fcall = pop(strings);
    strings.push(fcall + ";");
}


void ExpoSEVisitor::visitFuncCall(FuncCall& f) {
    f.accept(this);
    string name = f.name;

    if(f.name == "assume"){
        name = "S$.assume";
    }

    if(f.name == "assert"){
        name = "S$.assert";
    }

    string args = "";
    for(auto& arg : f.args) {
        string argstr = pop(strings);
        args += argstr + ",";
    }
    args.pop_back();

    if(f.name == "print"){
        name = "S$.symbol";
        args += ",";
        args += "\"\"";
    }

    strings.push(name + "(" + args + ")");
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


void ExpoSEVisitor::visitStmt(Stmt& s) {
    switch (s.statementType)
    {
    case StatementType::ASSIGN:
        visitAssign((Assign&)s);
        break;
    case StatementType::FUNCTIONCALL_STMT:
        visitFuncCallStmt((FuncCallStmt&)s);
        break;
    default:
        break;  
    }
}
// ExpoSEVisitor::~ExpoSEVisitor() {}

void ExpoSEVisitor::visitAssign(Assign& a) {
    a.accept(this);
    string v1 = pop(strings);
    string exp1 = pop(strings);
    strings.push("var " + v1 + " = " + exp1+ ";");
}

Visitor::~Visitor() {}

ExpoSEVisitor::~ExpoSEVisitor() {}

void ExpoSEVisitor::visitVar(Var& v) {
    v.accept(this);
    string a = v.name;
    strings.push(v.name);
}

void ExpoSEVisitor::visitNum(Num& n) {
    n.accept(this);
    strings.push(to_string(n.value));
}

void ExpoSEVisitor :: visitProgram(Program& program) {
    program.accept(this);
    string resultantProgram;
    while(!strings.empty()) {
        resultantProgram += pop(strings);
    }
    strings.push(resultantProgram);
}

// void ExpoSEVisitor::visitFuncCall(FuncCall& f) {
//     f.accept(this);
//     strings.push(f.name);
// }

void ExpoSEVisitor::visitMap(Map& m) {
    m.accept(this);
    // strings.push(m);
}

void ExpoSEVisitor::visitSet(Set& s) {
    s.accept(this);
    // strings.push(s.name);
}


