#ifndef VISITOR_H
#define VISITOR_H

#include <iostream>

#include <string>
#include <stack>
#include <vector>
#include <regex>

using namespace std;

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

enum class HTTPResponseCode;

class ExpoSEVisitor {
protected:

    // virtual string string_of_type(Type) = 0;
public:
    stack<string> strings;
    static string pop(stack<string> &);
    string retrieve();
    // virtual string string_of_type();
    // virtual void visitHTTPResponseCode(HTTPResponseCode);
    void visitDecl(Decl &);
    // virtual void visitfunDecl(fundecl&);
    // virtual void visitTypeExpr(TypeExpr&);
    // virtual void visitTypeConst(TypeConst&);
    // virtual void visitFuncType(FuncType&);
    // virtual void visitMapType(MapType&);
    // virtual void visitTupleType(TupleType&);
    // virtual void visitSetType(SetType&);
    void visitExpr(Expr &);
    // virtual void visitPolymorphicFuncCall(PolymorphicFuncCall&);
    void visitVar(Var &);
    void visitFuncCall(FuncCall &);
    void visitNum(Num &);
    void visitSet(Set &);
    void visitMap(Map &);
    // virtual void visitTuple(Tuple&);
    // virtual void visitFuncDecl(FuncDecl&);
    void visitInit(Init &);
    // virtual void visitResponse(Response&);
    // virtual void visitAPIcall(APIcall&);
    //    virtual void visitAPI(API&);
        // virtual void visitSpec(Spec&);
        void visitStmt(Stmt&);
        void visitAssign(Assign&);
        void visitFuncCallStmt(FuncCallStmt&);
        void visitProgram(Program&);
        ~ExpoSEVisitor();
};


#endif