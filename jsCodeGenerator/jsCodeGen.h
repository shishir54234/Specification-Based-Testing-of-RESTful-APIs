#ifndef CODEGEN_H
#define CODEGEN_H
#pragma once
#include <iostream>
#include <stack>
#include "visitor.h"

// #include "../ast.hpp"
class Program;

using namespace std;
// using namespace ast;

// namespace codegen {

class CodeGenerator {
    protected:
        ExpoSEVisitor *visitor;
        static string indent(string, string, int);
    public:
        CodeGenerator(ExpoSEVisitor *v);
        string generateCode(Program&) const;
        virtual ~CodeGenerator();
    };

class ExpoSECodeGenerator : public CodeGenerator {
public:
    ExpoSECodeGenerator() : CodeGenerator(new ExpoSEVisitor()) {}
};

// } // namespace codegen
#endif
