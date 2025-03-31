#pragma once
#include <iostream>
#ifndef ASTVIS_HPP
#define ASTVIS_HPP
// Forward declarations
class TypeConst;
class FuncType;
class MapType;
class TupleType;
class SetType;
class Expr;
class TypeExpr;
class Var;
class FuncCall;
class Num;
class String;
class Set;
class Map;
class Tuple;
class PolymorphicFuncCall;
class Decl;
class FuncDecl;
class APIcall;
class Response;
class API;
class Init;
class Spec;

class Stmt;
class Assign;
class FuncCallStmt;

class Program;

class ASTVisitor
{
public:
    virtual ~ASTVisitor() = default;

    // Type Expression visitors
    virtual void visit(const TypeConst &node) = 0;
    virtual void visit(const FuncType &node) = 0;
    virtual void visit(const MapType &node) = 0;
    virtual void visit(const TupleType &node) = 0;
    virtual void visit(const SetType &node) = 0;

    // Expression visitors
    virtual void visit(const Var &node) = 0;
    virtual void visit(const FuncCall &node) = 0;
    virtual void visit(const Num &node) = 0;
    virtual void visit(const String &node) = 0;
    virtual void visit(const Set &node) = 0;
    virtual void visit(const Map &node) = 0;
    virtual void visit(const Tuple &node) = 0;
    virtual void visit(const PolymorphicFuncCall &node) = 0;

    // Declaration visitors
    virtual void visit(const Decl &node) = 0;
    virtual void visit(const FuncDecl &node) = 0;

    // API and Related visitors
    virtual void visit(const APIcall &node) = 0;
    virtual void visit(const API &node) = 0;
    virtual void visit(const Response &node) = 0;
    // Initialization visitor
    virtual void visit(const Init &node) = 0;

    // Specification visitor
    virtual void visit(const Spec &node) = 0;

    // Statement visitors
    // virtual void visit(const Stmt &node) = 0;
    virtual void visit(const Assign &node) = 0;
    virtual void visit(const FuncCallStmt &node) = 0;
    virtual void visit(const Program &node) = 0;
};
#endif