#ifndef CODEGEN_H
#define CODEGEN_H

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
        Visitor *visitor;
        static string indent(string, string, int);
    public:
        CodeGenerator(Visitor *v);
        string generateCode(Program&);
        virtual ~CodeGenerator();
    };

class CCodeGenerator : public CodeGenerator {
public:
    CCodeGenerator() : CodeGenerator(new ExpoSEVisitor()) {}
};

// } // namespace codegen
#endif
