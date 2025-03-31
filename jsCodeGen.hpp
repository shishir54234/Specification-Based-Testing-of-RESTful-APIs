#pragma once
#include <iostream>
#include "ast.hpp"
#include <iomanip> // For std::quoted

class jsCodeGen : public ASTVisitor
{
private:
    int indent = 0;

    void printIndent()
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }
    }
public:
    // Existing type expression visitors
    void visit(const TypeConst &node) override
    {
        std::cout << node.name;
    }

    void visit(const FuncType &node) override
    {
        printIndent();
        std::cout << "FuncType:\n";
        indent++;

        printIndent();
        std::cout << "Parameters:\n";
        indent++;
        for (const auto &param : node.params)
        {
            param->accept(*this);
        }
        indent--;

        printIndent();
        std::cout << "Return type:\n";
        indent++;
        node.returnType->accept(*this);
        indent--;

        indent--;
    }

    void visit(const MapType &node) override
    {
        printIndent();
        std::cout << "MapType:\n";
        indent++;

        printIndent();

        std::cout << "Domain:\n";
        if(node.domain){
            indent++;
            node.domain->accept(*this);
            indent--;
        }
        

        printIndent();
        std::cout << "Range:\n";
        if(node.range){
            indent++;
            node.range->accept(*this);
            indent--;
        }
        

        indent--;
    }

    void visit(const TupleType &node) override
    {
        printIndent();
        std::cout << "TupleType:\n";
        indent++;
        for (const auto &elem : node.elements)
        {
            if(elem)elem->accept(*this);
        }
        indent--;
    }

    void visit(const SetType &node) override
    {
        printIndent();
        std::cout << "SetType:\n";
        if(node.elementType){
            indent++;
            node.elementType->accept(*this);
            indent--;
        }
        
    }

    // Expression visitors
    void visit(const Var &node) override
    {
        std::cout <<node.name;
    }

    void visit(const FuncCall &node) override
    {
        std::cout << node.name << '(';
        
        for (size_t i = 0; i < node.args.size(); ++i) {
            node.args[i]->accept(*this);
            if (i < node.args.size() - 1) {
                std::cout << ", ";
            }
        }
        
        std::cout<<')';
    }

    void visit(const Num &node) override
    {
        std::cout << node.value;
    }

    void visit(const Set &node) override {
        std::cout << "new Set([";
        for (size_t i = 0; i < node.elements.size(); i++) {
            node.elements[i]->accept(*this);
            if (i < node.elements.size() - 1)
                std::cout << ", ";
        }
        std::cout << "])";
    }
    
    void visit(const Map &node) override {
        std::cout << "new Map([";
        for (size_t i = 0; i < node.value.size(); i++) {
            std::cout << "[";
            node.value[i].first->accept(*this);
            std::cout << ", ";
            node.value[i].second->accept(*this);
            std::cout << "]";
            if (i < node.value.size() - 1)
                std::cout << ", ";
        }
        std::cout << "])";
    }
    

    void visit(const Tuple &node) override {
        std::cout << "[";
        for (size_t i = 0; i < node.expr.size(); i++) {
            node.expr[i]->accept(*this);
            if (i < node.expr.size() - 1)
                std::cout << ", ";
        }
        std::cout << "]";
    }
    

    void visit(const PolymorphicFuncCall &node) override
    {
        printIndent();
        std::cout << "PolymorphicFuncCall: " << node.name << "\n";
        indent++;

        printIndent();
        std::cout << "Type Arguments:\n";
        indent++;
        for (const auto &typeArg : node.typeArgs)
        {
            typeArg->accept(*this);
        }
        indent--;

        printIndent();
        std::cout << "Arguments:\n";
        indent++;
        for (const auto &arg : node.args)
        {
            arg->accept(*this);
        }
        indent--;

        indent--;
    }

    // Declaration visitors
    void visit(const Decl &node) override
    {
 
        std::cout << "let " << node.name << ";"<<"\n";
    }

    void visit(const FuncDecl &node) override
    {
        printIndent();
        std::cout << "Function Declaration: " << node.name << "\n";
        indent++;

        printIndent();
        std::cout << "Parameters:\n";
        indent++;
        for (const auto &param : node.params)
        {
            param->accept(*this);
        }
        indent--;

        printIndent();
        std::cout << "Return Type (HTTP Code: " << static_cast<int>(node.returnType.first) << "):\n";
        indent++;
        for (const auto &type : node.returnType.second)
        {
            type->accept(*this);
        }
        indent--;

        indent--;
    }

    // void visit(const RecordDecl &node)
    // {
    //     printIndent();
    //     std::cout << "Record Declaration: " << node.recname << "\n";
    //     indent++;
    //     for (const auto &field : node.fields)
    //     {
    //         visit(*field);
    //     }
    //     indent--;
    // }

    // API and Related visitors
    void visit(const APIcall &node) override
    {
        printIndent();
        std::cout << "API Call:\n";
        indent++;

        printIndent();
        std::cout << "Function Call:\n";
        indent++;
        node.call->accept(*this);
        indent--;

        printIndent();
        std::cout << "Response (HTTP Code: " << 123 << "):\n";
        if (node.response.expr){
            indent++;
            node.response.expr->accept(*this);
            indent--;
        }
            

        
    }

    void visit(const API &node) override    
    {
        printIndent();
        std::cout << "API:\n";
        indent++;

        printIndent();
        std::cout << "Precondition:\n";
        indent++;
        node.pre->accept(*this);
        indent--;

        printIndent();
        std::cout << "Call:\n";
        indent++;
        visit(*node.call);
        indent--;

        printIndent();
        // std::cout << "Response (HTTP Code: " << static_cast<int>(node.response.code) << "):\n";
        indent++;
        if(node.response.expr)node.response.expr->accept(*this);
        indent--;

        indent--;
    }
    void visit(const Response &node) override
    {
        printIndent();
        std::cout << "Response (HTTP Code: " << 123 << "):\n";
        cout << "HELLO\n";
        if(node.expr){
            cout<<"HELLO\n";
            indent++;
            node.expr->accept(*this);
            indent--;
        }
        
    }
    // Initialization visitor
    void visit(const Init &node) override
    {
        printIndent();
        std::cout << "Initialization: " << node.varName << "\n";
        indent++;
        node.expr->accept(*this);
        indent--;
    }

    // Spec visitor
    void visit(const Spec &node) override
    {
        printIndent();
        std::cout << "Specification:\n";
        indent++;

        printIndent();
        std::cout << "Globals:\n";
        indent++;
        for (const auto &global : node.globals)
        {
            visit(*global);
        }
        indent--;

        printIndent();
        std::cout << "Initializations:\n";
        indent++;
        for (const auto &init : node.init)
        {
            visit(*init);
        }
        indent--;

        printIndent();
        std::cout << "Functions:\n";
        indent++;
        for (const auto &func : node.functions)
        {
            visit(*func);
        }
        indent--;

        printIndent();
        std::cout << "API Blocks:\n";
        indent++;
        for (const auto &block : node.blocks)
        {
            visit(*block);
        }
        indent--;

        indent--;
    }
    void visit(const Assign &node) override{
        if (node.left) // Check if left is not null
            node.left->accept(*this);
        else
            std::cout << "<null_var>";
        cout<<" = ";

        if(node.right){
            node.right->accept(*this);
        }
        else{
            std::cout << "<null_expr>";
        }
        cout<<";"<<"\n";

    }

    void visit(const FuncCallStmt &node) override{
        node.call->accept(*this);
        cout<<";"<<"\n";
    }
    void visit(const Program &node) override{
        std:: cout << "const S$ = require('S$');"<<"\n";
        for(const auto &decls: node.declarations){
            decls.get()->accept(*this);
        }
        for (const auto &func : node.statements)
        {
            func.get()->accept(*this);
        }
    }
};