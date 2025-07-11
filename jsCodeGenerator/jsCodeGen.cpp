#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>
#include <map>
#include <vector>
#include <set>
#include "visitor.h
#include "jsCodeGen.h"
#include "../ast.hpp"

// using std::regex;
using namespace std;


CodeGenerator::CodeGenerator(ExpoSEVisitor *v) : visitor(v) {}

string CodeGenerator::indent(string line, string istring, int level) {
	string indentation = "";
	for(int i = 0; i < level; i++) {
		indentation += istring;
	}
	line = indentation + line;
	return line;
}
string CodeGenerator::generateCode(Program& program) const {
    visitor->visitProgram(program);

    string raw = visitor->retrieve();
    // std::cout << strings.size() << std::endl;
    cout<<"This is raw string"<<endl;
    cout<<raw<<endl;
    const regex del("\n");
    sregex_token_iterator it(raw.begin(), raw.end(), del, -1);
    sregex_token_iterator end;
    vector<string> lines;
    while (it != end) {
        // cout<<"line: "<<*it<<endl;
        lines.push_back(*it);
        ++it;
    }

    string indentedCode = "";
    for(int i=lines.size()-1;i>=0;i--) {
        indentedCode += lines[i] + '\n';
    }
    // cout<<"indented code: "<<indentedCode;
    return indentedCode;
}
CodeGenerator::~CodeGenerator() {
    delete visitor;
}

string ExpoSEVisitor::retrieve()
{
    if(strings.empty())
    {
        cout<<"strings is empty"<<endl; return "";
    }
    std::cout << strings.size() << std::endl;
    return pop(strings);
}
string ExpoSEVisitor::pop(stack<string> &s)
{
    if(s.empty()) return "";
    string top = s.top();
    s.pop();
    return top;
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

void ExpoSEVisitor::visitDecl(Decl& d) {
    // d.accept(this);
    string name = d.name;
    string type = pop(strings);
    strings.push(type + " " + name);
}

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
    // cout<<"reached visit FuncCallStmt"<<endl;
    (f.call)->accept(this);
    // cout<<"finished visit FuncCallStmt"<<endl;;
    string fcall = pop(strings);
    strings.push(fcall + ";");
}


void ExpoSEVisitor::visitFuncCall(const FuncCall& f) {
    // cout<<"enterred visit FuncCall"<<endl;
    // cout<<"Function Name: "<<f.name<<endl;
    // f.accept(this);

    vector<string> arguments;
    string name = f.name;
    cout<<f.name<<endl;
    // cout<<"Function Name: "<<f.name<<endl;
    for(auto& arg : f.args) 
    {
        arg->accept(this);
    }
    string args = "";
    for(auto& arg : f.args) {
        string argstr = pop(strings);
        arguments.push_back(argstr);
        args += argstr + ",";
        // cout<<argstr<<" ";
    }
    // cout<<endl;
    if(args.size())args.pop_back();

    if(f.name == "assume"){
        name = "S$.assume";
    }

    if(f.name == "assert"){
        name = "S$.assert";
    }

    if(f.name == "="){
        string equalCheck = arguments[0] + " == " + arguments[1];
        strings.push(equalCheck);
        return;
    }

    if(f.name == "not in"){
        string notIn  = "!(" + arguments[0] + ".has(" + arguments[1] + "))";
        strings.push(notIn);
        return;
    }

    if  (f.name == "[]"){
        string access = arguments[1] + "[" + arguments[0] + "]";
        strings.push(access);
        return;
    }

    if(f.name == "dom"){
        strings.push(arguments[0]);
        return;
    }

    if(f.name == "input"){
        string assign = "var " + args + " = ";
        name = "S$.symbol";
        args += ",";
        args += "\"\"";

        strings.push(assign + name + "(" + args + ")");
        return;
    }
    strings.push(name + "(" + args + ")");
}



void ExpoSEVisitor::visitStmt(Stmt& s) {
    // cout<<"reached visit Stmt";
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
void ExpoSEVisitor::visitAssign(Assign& a) {
    // cout<<"reached visit Assign";
    // a.accept(this);
    string v1 = pop(strings);
    string exp1 = pop(strings);
    strings.push("var " + v1 + " = " + exp1+ ";");
}


ExpoSEVisitor::~ExpoSEVisitor() {}

void ExpoSEVisitor::visitVar(Var& v) {
    string a = v.name;
    strings.push(v.name);
}

void ExpoSEVisitor::visitNum(Num& n) {
    strings.push(to_string(n.value));
}
void ExpoSEVisitor::visitString(String &s)
{
    strings.push(s.value);
}

void ExpoSEVisitor :: visitProgram(Program& program) {
    int cnt=0;
    for(auto& stmt : program.statements)  
    {
        cout<<cnt++<<endl;

        stmt->accept(this);
    }
    string resultantProgram;
    while(!strings.empty()) {
        resultantProgram += pop(strings);
        resultantProgram += "\n";
    }
    strings.push(resultantProgram);
}

// void ExpoSEVisitor::visitFuncCall(FuncCall& f) {
//     f.accept(this);
//     strings.push(f.name);
// }

void ExpoSEVisitor::visitMap(Map& m) {
    // cout<<"map\n";
    // m.accept(this);
    string map_decl = "new Map();";
    strings.push(map_decl);
}

void ExpoSEVisitor::visitSet(Set& s) {
    // s.accept(this);
    // strings.push(s.name);
}

void ExpoSEVisitor::visitInit(Init &i)
{
    cout<<"init\n";
    // i.accept(this);
    string varName = i.varName;
    string expression = pop(strings);
    strings.push("var " + varName + " = " + expression + ";");
}
