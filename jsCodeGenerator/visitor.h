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

class Visitor {
    protected:
        stack<string> strings;
        static string pop(stack<string>&);
        // virtual string string_of_type(Type) = 0;
    
    public:
        // virtual string string_of_type();
        // virtual void visitHTTPResponseCode(HTTPResponseCode);
        // virtual void visitDecl(Decl&);
        // virtual void visitfunDecl(fundecl&);
        // virtual void visitTypeExpr(TypeExpr&);
        // virtual void visitTypeConst(TypeConst&);
        // virtual void visitFuncType(FuncType&);
        // virtual void visitMapType(MapType&);
        // virtual void visitTupleType(TupleType&);
        // virtual void visitSetType(SetType&);
        virtual void visitExpr(Expr&) = 0;
        // virtual void visitPolymorphicFuncCall(PolymorphicFuncCall&);
        virtual void visitVar(Var&) = 0;
        virtual void visitFuncCall(FuncCall&) = 0;
        virtual void visitNum(Num&) =0 ;
        virtual void visitSet(Set&) =0;
        virtual void visitMap(Map&) =0;
        // virtual void visitTuple(Tuple&);
        // virtual void visitFuncDecl(FuncDecl&);
        virtual void visitInit(Init&)=0;
        // virtual void visitResponse(Response&);
        // virtual void visitAPIcall(APIcall&);
    //    virtual void visitAPI(API&);
        // virtual void visitSpec(Spec&);
        virtual void visitStmt(Stmt&) =0;
        virtual void visitAssign(Assign&) =0;
        virtual void visitFuncCallStmt(FuncCallStmt&) =0;
        virtual void visitProgram(Program&) =0;
        virtual ~Visitor();
        string retrieve();
    };
    
    
class ExpoSEVisitor : public Visitor {

    public:
        // virtual string string_of_type();
        // virtual void visitHTTPResponseCode(HTTPResponseCode);
        // virtual void visitDecl(Decl&);
        // virtual void visitfunDecl(fundecl&);
        // virtual void visitTypeExpr(TypeExpr&);
        // virtual void visitTypeConst(TypeConst&);
        // virtual void visitFuncType(FuncType&);
        // virtual void visitMapType(MapType&);
        // virtual void visitTupleType(TupleType&);
        // virtual void visitSetType(SetType&);
        virtual void visitExpr(Expr&);
        // virtual void visitPolymorphicFuncCall(PolymorphicFuncCall&);
        virtual void visitVar(Var&);
        virtual void visitFuncCall(FuncCall&);
        virtual void visitNum(Num&);
        virtual void visitSet(Set&);
        virtual void visitMap(Map&);
        // virtual void visitTuple(Tuple&);
        // virtual void visitFuncDecl(FuncDecl&);
        virtual void visitInit(Init&);
        // virtual void visitResponse(Response&);
        // virtual void visitAPIcall(APIcall&);
    //    virtual void visitAPI(API&);
        // virtual void visitSpec(Spec&);
        virtual void visitStmt(Stmt&);
        virtual void visitAssign(Assign&);
        virtual void visitFuncCallStmt(FuncCallStmt&);
        virtual void visitProgram(Program&);
        virtual ~ExpoSEVisitor();
};


#endif