#pragma once
#include <iostream>

// Forward declarations
class TypeConst;
class FuncType;
class MapType;
class TupleType;
class SetType;
class Var;
class FuncCall;
class Num;
class Set;
class Map;
class Tuple;

class ASTVisitor
{
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(const TypeConst &node) = 0;
    virtual void visit(const FuncType &node) = 0;
    virtual void visit(const MapType &node) = 0;
    virtual void visit(const TupleType &node) = 0;
    virtual void visit(const SetType &node) = 0;
    virtual void visit(const Var &node) = 0;
    virtual void visit(const FuncCall &node) = 0;
    virtual void visit(const Num &node) = 0;
    virtual void visit(const Set &node) = 0;
    virtual void visit(const Map &node) = 0;
    virtual void visit(const Tuple &node) = 0;
};