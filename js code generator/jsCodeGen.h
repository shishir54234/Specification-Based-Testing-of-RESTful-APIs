#ifndef CODEGEN_H
#define CODEGEN_H


#include <stack>
#include "../ast.hpp"

using namespace std;
// using namespace ast;

// namespace codegen {
class Visitor {
protected:
    stack<string> strings;
    static string pop(stack<string>&);
    // virtual string string_of_type(Type) = 0;

public:
    virtual void visitDecl(Decl&) = 0;
    virtual void visitfunDecl(fundecl&) = 0;
    virtual void visitTypeExpr(TypeExpr&) = 0;
    virtual void visitTypeConst(TypeConst&) = 0;
    virtual void visitFuncType(FuncType&) = 0;
    virtual void visitMapType(MapType&) = 0;
    virtual void visitTupleType(TupleType&) = 0;
    virtual void visitSetType(SetType&) = 0;
    virtual void visitExpr(Expr&) = 0;
    virtual void visitPolymorphicFuncCall(PolymorphicFuncCall&) = 0;
    virtual void visitVar(Var&) = 0;
    virtual void visitFuncCall(FuncCall&) = 0;
    virtual void visitNum(Num&) = 0;
    virtual void visitSet(Set&) = 0;
    virtual void visitMap(Map&) = 0;
    virtual void visitTuple(Tuple&) = 0;
    virtual void visitFuncDecl(FuncDecl&) = 0;
    virtual void visitInit(Init&) = 0;
    virtual void visitResponse(Response&) = 0;
    virtual void visitAPIcall(APIcall&) = 0;
    virtual void visitAPI(API&) = 0;
    virtual void visitSpec(Spec&) = 0;
    virtual void visitStmt(Stmt&) = 0;
    virtual void visitAssign(Assign&) = 0;
    virtual void visitFuncCallStmt(FuncCallStmt&) = 0;
    virtual void visitProgram(Program&) = 0;

    string retrieve();
};

class CodeGenerator {
protected:
    Visitor *visitor;
    static string indent(string, string, int);
public:
    CodeGenerator(Visitor *v);
    string generateCode(Program&);
    virtual ~CodeGenerator();
};


// class CVisitor : public Visitor {
// public:
//     // virtual string string_of_type(Type);
//     virtual void visitDecl(Decl&);
//     virtual void visitDeclList(DeclList&);
//     virtual void visitEmpty(Empty&);
//     virtual void visitNum(Num&);
//     virtual void visitVar(Var&);
//     virtual void visitAddExpression(AddExpression&);
//     virtual void visitSubExpression(SubExpression&);
//     virtual void visitMulExpression(MulExpression&);
//     virtual void visitDivExpression(DivExpression&);
//     virtual void visitFunctionCall(FunctionCall&);
//     virtual void visitSkipStatement(SkipStatement&);
//     virtual void visitAssignmentStatement(AssignmentStatement&);
//     virtual void visitSequenceStatement(SequenceStatement&);
//     virtual void visitBranchStatement(BranchStatement&);
//     virtual void visitLoopStatement(LoopStatement&);
//     virtual void visitBlockStatement(BlockStatement&);
//     virtual void visitReturnStatement(ReturnStatement&);
//     virtual void visitFunctionDefinition(FunctionDefinition&);
//     virtual void visitParameterList(DeclList&);
//     virtual void visitFunctionDefinitionList(FunctionDefinitionList&);

//     virtual void visitProgram(Program&);
// };

class ExpoSEVisitor : public Visitor {

    public:
        // virtual string string_of_type();
        virtual void visitDecl(Decl&);
        virtual void visitfunDecl(fundecl&);
        virtual void visitTypeExpr(TypeExpr&);
        virtual void visitTypeConst(TypeConst&);
        virtual void visitFuncType(FuncType&);
        virtual void visitMapType(MapType&);
        virtual void visitTupleType(TupleType&);
        virtual void visitSetType(SetType&);
        virtual void visitExpr(Expr&);
        virtual void visitPolymorphicFuncCall(PolymorphicFuncCall&);
        virtual void visitVar(Var&);
        virtual void visitFuncCall(FuncCall&);
        virtual void visitNum(Num&);
        virtual void visitSet(Set&);
        virtual void visitMap(Map&);
        virtual void visitTuple(Tuple&);
        virtual void visitFuncDecl(FuncDecl&);
        virtual void visitInit(Init&);
        virtual void visitResponse(Response&);
        virtual void visitAPIcall(APIcall&);
        virtual void visitAPI(API&);
        virtual void visitSpec(Spec&);
        virtual void visitStmt(Stmt&);
        virtual void visitAssign(Assign&);
        virtual void visitFuncCallStmt(FuncCallStmt&);
        virtual void visitProgram(Program&);
};

class CCodeGenerator : public CodeGenerator {
public:
    CCodeGenerator() : CodeGenerator(new ExpoSEVisitor()) {}
};

// } // namespace codegen
#endif
