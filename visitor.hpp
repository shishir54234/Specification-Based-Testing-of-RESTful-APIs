#pragma once
#include "ast.hpp"

class PrintVisitor : public ASTVisitor
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
    void visit(const TypeConst &node) override
    {
        printIndent();
        std::cout << "TypeConst: " << node.name << '\n';
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
        indent++;
        node.domain->accept(*this);
        indent--;

        printIndent();
        std::cout << "Range:\n";
        indent++;
        node.range->accept(*this);
        indent--;

        indent--;
    }

    void visit(const TupleType &node) override
    {
        printIndent();
        std::cout << "TupleType:\n";
        indent++;
        for (const auto &elem : node.elements)
        {
            elem->accept(*this);
        }
        indent--;
    }

    void visit(const SetType &node) override
    {
        printIndent();
        std::cout << "SetType:\n";
        indent++;
        node.elementType->accept(*this);
        indent--;
    }

    void visit(const Var &node) override
    {
        printIndent();
        std::cout << "Var: " << node.name << '\n';
    }

    void visit(const FuncCall &node) override
    {
        printIndent();
        std::cout << "FuncCall: " << node.name << "\n";
        indent++;
        for (const auto &arg : node.args)
        {
            arg->accept(*this);
        }
        indent--;
    }

    void visit(const Num &node) override
    {
        printIndent();
        std::cout << "Num: " << node.value << '\n';
    }

    void visit(const Set &node) override
    {
        printIndent();
        std::cout << "Set:\n";
        indent++;
        for (const auto &elem : node.elements)
        {
            elem->accept(*this);
        }
        indent--;
    }

    void visit(const Map &node) override
    {
        printIndent();
        std::cout << "Map:\n";
        indent++;
        node.value.first->accept(*this);
        node.value.second->accept(*this);
        indent--;
    }

    void visit(const Tuple &node) override
    {
        printIndent();
        std::cout << "Tuple:\n";
        indent++;
        for (const auto &e : node.expr)
        {
            e->accept(*this);
        }
        indent--;
    }
};