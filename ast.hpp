#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "ASTVis.hpp"
#include "js code generator/jsCodeGen.h"
using namespace std;
#ifndef AST_HPP
#define AST_HPP
enum class HTTPResponseCode
{
    OK_200,
    CREATED_201,
    // Add other response codes as needed
};

// Forward declarations
// class TypeExpr;
// class Expr;
// class FuncCall;

// Base classes for different types of declarations
class Decl
{
public:
    Decl(std::string name, std::unique_ptr<TypeExpr> typeExpr)
        : name(std::move(name)), type(std::move(typeExpr)) {}
    virtual ~Decl() = default;
    virtual void accept(ASTVisitor &visitor) {
        cout<<"hey\n";
        visitor.visit(*this);
    }
    std::string name;
    std::unique_ptr<TypeExpr> type;
};
class fundecl{
    public:
    std::string name;
    std::unique_ptr<TypeExpr> params;
    std::unique_ptr<TypeExpr> outp;
    fundecl(std::string name, std::unique_ptr<TypeExpr> param, std::unique_ptr<TypeExpr> outp): name(std::move(name)), params(std::move(param)), outp(std::move(outp)){};
};
class TypeExpr
{
public:
    virtual ~TypeExpr() = default;
    virtual void accept(ASTVisitor &visitor) const = 0;
};

// class VariantConstructor
// {
// public:
//     void accept(ASTVisitor &visitor) const
//     {
//         visitor.visit(*this);
//     }
//     // Add necessary fields
// };

// Type Expressions
class TypeConst : public TypeExpr
{
public:
    explicit TypeConst(std::string name) : name(std::move(name)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::string name;
};

// class TypeVariable : public TypeExpr
// {
// public:
//     // Constructor for a basic type variable with just a name
//     explicit TypeVariable(std::string name)
//         : name(std::move(name)),
//           constraint(nullptr) {}

//     // Constructor for a type variable with a constraint
//     TypeVariable(std::string name, std::unique_ptr<TypeExpr> constraint)
//         : name(std::move(name)),
//           constraint(std::move(constraint)) {}

//     // Copy constructor (needed for type variable management)
//     TypeVariable(const TypeVariable &other)
//         : name(other.name),
//           constraint(other.constraint ? other.constraint->clone() : nullptr) {}

//     // Clone method for polymorphic copying
//     virtual std::unique_ptr<TypeExpr> clone() const override
//     {
//         return std::make_unique<TypeVariable>(*this);
//     }

//     // Check if this type variable has a constraint
//     bool hasConstraint() const
//     {
//         return constraint != nullptr;
//     }

//     // Getter for the constraint (if any)
//     const TypeExpr *getConstraint() const
//     {
//         return constraint.get();
//     }

//     // Optional: Add a constraint after construction
//     void addConstraint(std::unique_ptr<TypeExpr> newConstraint)
//     {
//         constraint = std::move(newConstraint);
//     }

//     std::string name;
//     std::unique_ptr<TypeExpr> constraint; // Optional constraint on the type variable
// };

class FuncType : public TypeExpr
{
public:
    FuncType(std::vector<std::unique_ptr<TypeExpr>> params, std::unique_ptr<TypeExpr> returnType)
        : params(std::move(params)), returnType(std::move(returnType)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::vector<std::unique_ptr<TypeExpr>> params;
    std::unique_ptr<TypeExpr> returnType;
};

class MapType : public TypeExpr
{
public:
    MapType(std::unique_ptr<TypeExpr> domain, std::unique_ptr<TypeExpr> range)
        : domain(std::move(domain)), range(std::move(range)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::unique_ptr<TypeExpr> domain;
    std::unique_ptr<TypeExpr> range;
};

class TupleType : public TypeExpr
{
public:
    explicit TupleType(std::vector<std::unique_ptr<TypeExpr>> elements)
        : elements(std::move(elements)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::vector<std::unique_ptr<TypeExpr>> elements;
};

class SetType : public TypeExpr
{
public:
    explicit SetType(std::unique_ptr<TypeExpr> elementType)
        : elementType(std::move(elementType)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::unique_ptr<TypeExpr> elementType;
};

// Type Declarations
// class TypeDecl
// {
// public:
//     virtual ~TypeDecl() = default;
//     virtual void accept(ASTVisitor &visitor) const
//     {
//         visitor.visit(*this);
//     }
// };

// class VariantDecl : public TypeDecl
// {
// public:
//     std::vector<VariantConstructor> constructors;
//     void accept(ASTVisitor &visitor) const override
//     {
//         visitor.visit(*this);
//     }
// };

// class RecordDecl : public TypeDecl
// {
// public:
//     RecordDecl(std::string name, std::vector<std::unique_ptr<Decl>> fields)
//         : recname(std::move(name)), fields(std::move(fields)) {}

//     std::string recname;
//     std::vector<std::unique_ptr<Decl>> fields;
//     void accept(ASTVisitor &visitor) const override
//     {
//         visitor.visit(*this);
//     }
// };

// Expressions
class Expr
{
public:
    virtual ~Expr() = default;
    virtual void accept(ASTVisitor& visitor) const = 0;
};
class PolymorphicFuncCall : public Expr
{
public:
    PolymorphicFuncCall(
        std::string name,
        std::vector<std::unique_ptr<TypeExpr>> typeArgs,
        std::vector<std::unique_ptr<Expr>> args) : name(std::move(name)),
                                                   typeArgs(std::move(typeArgs)),
                                                   args(std::move(args)) {}

    std::string name;                                // Name of the polymorphic function
    std::vector<std::unique_ptr<TypeExpr>> typeArgs; // Type arguments for polymorphism
    std::vector<std::unique_ptr<Expr>> args;         // Regular arguments
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }
};
class Var : public Expr
{
public:
    explicit Var(std::string name) : name(std::move(name)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    bool operator<(const Var &other) const
    {
        return name < other.name; // Lexicographical comparison
    }

    std::string name;
};

class FuncCall : public Expr
{
public:
    FuncCall(std::string name, std::vector<std::unique_ptr<Expr>> args)
        : name(std::move(name)), args(std::move(args)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::string name;
    std::vector<std::unique_ptr<Expr>> args;
};

class Num : public Expr
{
public:
    explicit Num(int value) : value(value) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    int value;
};

class Set : public Expr
{
public:
    explicit Set(std::vector<std::unique_ptr<Expr>> elements)
        : elements(std::move(elements)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::vector<std::unique_ptr<Expr>> elements;
};

class Map : public Expr
{
public:
    explicit Map(std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>>) : value(std::move(value)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>> value;
};

class Tuple : public Expr
{
public:
    explicit Tuple(std::vector<std::unique_ptr<Expr>> exprs) : expr(std::move(expr)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
    }
    std::vector<std::unique_ptr<Expr>> expr;
};

// Function Declaration
class FuncDecl
{
public:
    FuncDecl(std::string name,
             std::vector<std::unique_ptr<TypeExpr>> params,
             std::pair<HTTPResponseCode, vector<std::unique_ptr<TypeExpr>>> returnType)
        : name(std::move(name)), params(std::move(params)), returnType(std::move(returnType)) {}
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }
    std::string name;
    std::vector<std::unique_ptr<TypeExpr>> params;
    std::pair<HTTPResponseCode, vector<std::unique_ptr<TypeExpr>>> returnType;
};

// Initialization
class Init
{
public:

    Init(std::string varName, std::unique_ptr<Expr> expression)
        : varName(std::move(varName)), expr(std::move(expression)) {}
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }
    std::string varName;
    std::unique_ptr<Expr> expr;
};
class Response{
    public:
    HTTPResponseCode code;
    std::unique_ptr<Expr> expr;
    Response(HTTPResponseCode code, std::unique_ptr<Expr> expr): code(code), expr(std::move(expr)){};
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }
};
class APIcall{
    public:
    std::unique_ptr<FuncCall> call;
    Response response;
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }
    APIcall(std::unique_ptr<FuncCall> call, Response response): call(std::move(call)), response(std::move(response)){};
};
// API
class API
{
public:
    API(std::unique_ptr<Expr> precondition,
        std::unique_ptr<APIcall> functionCall,
        Response response)
        : pre(std::move(precondition)), call(std::move(functionCall)), response(std::move(response)) {}
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }
    std::unique_ptr<Expr> pre;
    std::unique_ptr<APIcall> call;
    Response response;
};

// Block class (placeholder as it wasn't fully specified in the grammar)
// Top-level Spec class
class Spec
{
public:
    Spec(std::vector<std::unique_ptr<Decl>> globals,
        //  std::vector<std::unique_ptr<TypeDecl>> types,
         std::vector<unique_ptr<Init>> init,
         std::vector<std::unique_ptr<FuncDecl>> functions,
         std::vector<std::unique_ptr<API>> blocks)
        : globals(std::move(globals)), init(std::move(init)), functions(std::move(functions)), blocks(std::move(blocks)) {}
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    void accept(Visitor *visitor) {
        for(auto& global: globals) {
            visitor.visitDecl(*global);
        }

        for(auto& i: init){
            visitor.visitInit(*i);
        }

        for(auto& function: functions){
            visitor.visitFunDecl(*function);
        }

        for(auto& block: blocks){
            visitor.visitBlock(*block);
            visitor.visitFunDecl(*block);
        }
    }

    std::vector<std::unique_ptr<Decl>> globals;
    // std::vector<std::unique_ptr<TypeDecl>> types;
    std::vector<unique_ptr<Init>> init;
    std::vector<std::unique_ptr<FuncDecl>> functions;
    std::vector<std::unique_ptr<API>> blocks;
    
};

class Stmt
{
public:
    virtual ~Stmt() = default;
    virtual void accept(ASTVisitor &visitor) const = 0;
};

// Assignment statement: l = r
class Assign : public Stmt
{
public:
    Assign(std::unique_ptr<Var> left, std::unique_ptr<Expr> right)
        : left(std::move(left)), right(std::move(right)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(Visitor *visitor){
        visitor.visitVar(*left);
        visitor.visitExpr(*right);
    }
    std::unique_ptr<Var> left;
    std::unique_ptr<Expr> right;
};

// Function call statement
class FuncCallStmt : public Stmt
{
public:
    explicit FuncCallStmt(std::unique_ptr<FuncCall> call)
        : call(std::move(call)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(Visitor &visitor) {
       visitor.visitFuncCall(*call);
    }
    
    std::unique_ptr<FuncCall> call;
};

// Program is the root of our AST
class Program
{
public:
    explicit Program(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
    void accept(ASTVisitor &visitor)
    {
        visitor.visit(*this);
    }

    void accept(Visitor *visitor) {
        for (auto& stmt : statements) { // Use const reference to avoid unnecessary copies
            visitor->visitStmt(*stmt);
        }
    }

    std::vector<std::unique_ptr<Stmt>> statements;
};
#endif