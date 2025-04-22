#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "ASTVis.hpp"
#include "jsCodeGenerator/visitor.h"
// --------------------- PLEASE READ THIS BEFORE PROCEEDING ---------------------------------------

// ASTvisitor ====> virtual class, printervisitor==> implementation of AST visitor and is used to convert the whole thing into a string 
// ExpoSEVisitor Class for jsCode generation.
using namespace std;
#ifndef AST_HPP
#define AST_HPP

enum class HTTPResponseCode
{
    OK_200,
    CREATED_201,
    BAD_REQUEST_400,
    // Add other response codes as needed
};
enum ExpressionType
{
    MAP,
    VAR,
    STRING,
    NUM,
    TUPLE,
    SET,
    FUNCTIONCALL_EXPR,
    POLYMORPHIC_FUNCTIONCALL_EXPR
};

enum TypeExpression
{
    TYPE_CONST,
    TYPE_VARIABLE,
    FUNC_TYPE,
    MAP_TYPE,
    SET_TYPE,
    TUPLE_TYPE
};

enum StatementType
{
    ASSIGN,
    FUNCTIONCALL_STMT,
    DECL,
};

class fundecl
{
public:
    std::string name;
    std::unique_ptr<TypeExpr> params;
    std::unique_ptr<TypeExpr> outp;
    fundecl(std::string name, std::unique_ptr<TypeExpr> param, std::unique_ptr<TypeExpr> outp) : name(std::move(name)), params(std::move(param)), outp(std::move(outp)) {};
};
class TypeExpr
{
public:
    virtual ~TypeExpr() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
    virtual std::unique_ptr<TypeExpr> clone() = 0;
    TypeExpression typeExpression;

protected:
    TypeExpr(TypeExpression typeExpr) : typeExpression(typeExpr) {}
};

// Type Expressions
class TypeConst : public TypeExpr
{
public:
    explicit TypeConst(std::string name) : TypeExpr(TypeExpression::TYPE_CONST), name(std::move(name)) {}
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
    }
    std::unique_ptr<TypeExpr> clone() override
    {
        return std::make_unique<TypeConst>(name);
    }
    std::string name;
};

class FuncType : public TypeExpr
{
public:
    FuncType(std::vector<std::unique_ptr<TypeExpr>> params, std::unique_ptr<TypeExpr> returnType)
        : TypeExpr(TypeExpression::FUNC_TYPE), params(std::move(params)), returnType(std::move(returnType)) {}
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &param : params)
        {
            // visitor->visitTypeExpr(*param);
        }
        // visitor->visitTypeExpr(*returnType);
    }
    std::unique_ptr<TypeExpr> clone() override
    {
        std::vector<std::unique_ptr<TypeExpr>> clonedParams;
        for (auto &param : params)
        {
            clonedParams.push_back(param->clone());
        }
        auto clonedReturnType = returnType ? returnType->clone() : nullptr;
        return std::make_unique<FuncType>(std::move(clonedParams), std::move(clonedReturnType));
    }

    std::vector<std::unique_ptr<TypeExpr>> params;
    std::unique_ptr<TypeExpr> returnType;
};

class MapType : public TypeExpr
{
public:
    MapType(std::unique_ptr<TypeExpr> domain, std::unique_ptr<TypeExpr> range)
        : TypeExpr(TypeExpression::MAP_TYPE), domain(std::move(domain)), range(std::move(range)) {}
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // visitor->visitTypeExpr(*domain);
        // visitor->visitTypeExpr(*range);
    }
    std::unique_ptr<TypeExpr> clone() override
    {
        auto clonedDomain = domain ? domain->clone() : nullptr;
        auto clonedRange = range ? range->clone() : nullptr;
        return std::make_unique<MapType>(std::move(clonedDomain), std::move(clonedRange));
    }

    std::unique_ptr<TypeExpr> domain;
    std::unique_ptr<TypeExpr> range;
};

class TupleType : public TypeExpr
{
public:
    explicit TupleType(std::vector<std::unique_ptr<TypeExpr>> elements)
        : TypeExpr(TypeExpression::TUPLE_TYPE), elements(std::move(elements)) {}
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &e : elements)
        {
            // visitor->visitTypeExpr(*e);
        }
    }
    // Clone implementation for TupleType
    std::unique_ptr<TypeExpr> clone() override
    {
        std::vector<std::unique_ptr<TypeExpr>> clonedElements;
        for ( auto &element : elements)
        {
            clonedElements.push_back(element->clone());
        }
        return std::make_unique<TupleType>(std::move(clonedElements));
    }

    std::vector<std::unique_ptr<TypeExpr>> elements;
};

class SetType : public TypeExpr
{
public:
    explicit SetType(std::unique_ptr<TypeExpr> elementType)
        : TypeExpr(TypeExpression::SET_TYPE), elementType(std::move(elementType)) {}
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // visitor->visitTypeExpr(*elementType);
    }
    std::unique_ptr<TypeExpr> clone()  override
    {
        auto clonedElementType = elementType ? elementType->clone() : nullptr;
        return std::make_unique<SetType>(std::move(clonedElementType));
    }

    std::unique_ptr<TypeExpr> elementType;
};
class Decl
{
public:
    Decl(std::string name, std::unique_ptr<TypeExpr> typeExpr)
        : name(std::move(name)), type(std::move(typeExpr)) {}
    virtual ~Decl() = default;
    virtual void accept(ASTVisitor &visitor)
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // visitor->visitTypeExpr(*type);
    }
    // Copy constructor for deep copying.
    Decl( Decl &other)
        : name(other.name)
    {
        if (other.type)
        {
            type = other.type->clone();
        }
    }
    virtual std::unique_ptr<Decl> clone() 
    {
        return std::make_unique<Decl>(*this);
    }

    std::string name;
    std::unique_ptr<TypeExpr> type;
};
// Expressions
class Expr
{
public:
    virtual ~Expr() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual void accept(ExpoSEVisitor *visitor) =0;    virtual std::unique_ptr<Expr> clone() const = 0;


    ExpressionType expressionType;

protected:
    Expr(ExpressionType exprType) : expressionType(exprType) {}
};

class Var : public Expr
{
public:
    explicit Var(std::string name) : Expr(ExpressionType::VAR), name(std::move(name)) {}
    void accept(ASTVisitor &visitor)  override
    {
        visitor.visit(*this);
    }
    bool operator<(const Var &v) const
    {
        return name < v.name;
    }
    void accept(ExpoSEVisitor *visitor) override
    {
    }

    std::unique_ptr<Expr> clone() const override{
        return std::make_unique<Var>(name);
    }


    std::string name;
};

class FuncCall : public Expr
{
public:
    FuncCall(std::string name, std::vector<std::unique_ptr<Expr>> args)
        : Expr(ExpressionType::FUNCTIONCALL_EXPR), name(std::move(name)), args(std::move(args)) {}
    void accept(ASTVisitor &visitor)  override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor) override
    {
        for (auto &arg : args)
        {
            visitor->visitExpr(*arg);
        }
    }
    std::unique_ptr<Expr> clone() const override{
        vector<unique_ptr<Expr>>clonedArgs;
        for(auto &arg:args){
            clonedArgs.push_back(arg->clone());
        }

        return std::make_unique<FuncCall>(name,std::move(clonedArgs));
    }

    std::string name;
    std::vector<std::unique_ptr<Expr>> args;
};

class Num : public Expr
{
public:
    explicit Num(int value) : Expr(ExpressionType::NUM), value(value) {}
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor) override
    {
        visitor->visitNum(*this);
    }

    std::unique_ptr<Expr> clone() const override{
        return make_unique<Num>(value);
    }


    int value;
};

class String : public Expr
{
public:
    explicit String(string value) : Expr(ExpressionType::STRING), value(value) {}
    void accept(ASTVisitor &visitor)  override
    {
        visitor.visit(*this);
    }
    void accept(ExpoSEVisitor *visitor) override
    {
        // visitor->visitString(*this);
    }
    std::unique_ptr<Expr> clone() const override{
        return make_unique<String>(value);
    }
    string value;
};

class Set : public Expr
{
public:
    explicit Set(std::vector<std::unique_ptr<Expr>> elements)
        : Expr(ExpressionType::SET), elements(std::move(elements)) {}
    void accept(ASTVisitor &visitor)  override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor) override
    {
        for (auto &e : elements)
        {
            visitor->visitExpr(*e);
        }
    }

    std::unique_ptr<Expr> clone() const override{
        vector<std::unique_ptr<Expr>> clonedElements;
        for(auto &element:elements){
            clonedElements.push_back(element->clone());
        }

        return make_unique<Set>(std::move(clonedElements));
    }

    std::vector<std::unique_ptr<Expr>> elements;
};

// Var* leftCloneRaw = dynamic_cast<Var*>(leftCloneBase.release());
// if (!leftCloneRaw) {
//     throw std::runtime_error("Clone of left-hand side did not produce a Var instance");
// }
// std::unique_ptr<Var> clonedLeft(leftCloneRaw);

class Map : public Expr
{
public:
    explicit Map(std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>>) : Expr(ExpressionType::MAP), value(std::move(value)) {}
    void accept(ASTVisitor &visitor)  override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor) override
    {
        for (auto &v : value)
        {
            visitor->visitVar(*(v.first));
            visitor->visitExpr(*(v.second));
        }
    }

    std::unique_ptr<Expr> clone() const override {
        std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>> clonedValue;
    
        for (const auto& element : value) {
            auto clonedExpr = element.first->clone();
            auto rawPtr = dynamic_cast<Var*>(clonedExpr.get());
    
            if (!rawPtr) {
                throw std::runtime_error("Map key is not of type Var");
            }
    
            std::unique_ptr<Var> clonedVar(static_cast<Var*>(clonedExpr.release()));
            // std::unique_ptr<Var> clonedVar(clonedExpr);
            std::unique_ptr<Expr> clonedExprValue = element.second->clone();
    
            clonedValue.emplace_back(std::move(clonedVar), std::move(clonedExprValue));
        }
    
        return std::make_unique<Map>(std::move(clonedValue));
    }
    
    


    std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>> value;
};

class Tuple : public Expr
{
public:
    explicit Tuple(std::vector<std::unique_ptr<Expr>> exprs) : Expr(ExpressionType::TUPLE), expr(std::move(expr)) {}
    void accept(ASTVisitor &visitor)  override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &e : expr)
        {
            visitor->visitExpr(*e);
        }
    }

    std::unique_ptr<Expr> clone() const override{
        std::vector<std::unique_ptr<Expr>> cloneexpr;
        for(auto &e:expr){
            cloneexpr.push_back(e->clone());
        }
        return make_unique<Tuple>(std::move(cloneexpr));
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
    void accept(ASTVisitor &visitor) 
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // for(auto& param: params){
        //     visitor->visitTypeExpr(*param);
        // }
        // visitor->visitHTTPResponseCode(returnType.first);
        for(auto& te: returnType.second){
            // visitor->visitTypeExpr(*te);
        }
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
    void accept(ASTVisitor &visitor) 
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitExpr(*expr);
    }

    std::string varName;
    std::unique_ptr<Expr> expr;
};
class Response
{
public:
    HTTPResponseCode code;
    std::unique_ptr<Expr> expr;
    Response(HTTPResponseCode code, std::unique_ptr<Expr> expr) : code(code), expr(std::move(expr)) {};
    void accept(ASTVisitor &visitor) 
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // visitor->visitHTTPResponseCode(code);
        visitor->visitExpr(*expr);
    }
};
class APIcall
{
public:
    std::unique_ptr<FuncCall> call;
    Response response;
    void accept(ASTVisitor &visitor) 
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitFuncCall(*call);
        // visitor->visitResponse(response);
    }

    APIcall(std::unique_ptr<FuncCall> Call, Response Response) : call(std::move(Call)), response(std::move(Response)) {};
};
// API
class API
{
public:
    API(std::unique_ptr<Expr> precondition,
        std::unique_ptr<APIcall> functionCall,
        Response response)
        : pre(std::move(precondition)), call(std::move(functionCall)), response(std::move(response)) {}
    void accept(ASTVisitor &visitor) 
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitExpr(*pre);
        // visitor->visitAPIcall(*call);
        // visitor->visitResponse(response);
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
        : globals(std::move(globals)), init(std::move(init)), functions(std::move(functions)), blocks(std::move(blocks))
    {
    }
    void accept(ASTVisitor &visitor) 
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // for(auto& global: globals) {
        //     visitor->visitDecl(*global);
        // }

        for(auto& i: init){
            visitor->visitInit(*i);
        }

        for(auto& function: functions){
            // visitor->visitFuncDecl(*function);
        }

        for(auto& block: blocks){
            // visitor->visitAPI(*block);
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
    virtual void accept(ASTVisitor &visitor)  = 0;
    virtual void accept(ExpoSEVisitor *visitor)  = 0;
    StatementType statementType;
    virtual std::unique_ptr<Stmt> clone() const=0;
    // virtual std::unique_ptr<Stmt> clone() const = 0;
protected:
    Stmt(StatementType type) : statementType(type) {}
};

// Assignment statement: l = r
class Assign : public Stmt
{
public:
    Assign(std::unique_ptr<Var> left, std::unique_ptr<Expr> right)
        : Stmt(StatementType::ASSIGN), left(std::move(left)), right(std::move(right)) {}
    void accept(ASTVisitor &visitor)  override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)  override
    {
        visitor->visitVar(*left);
        visitor->visitExpr(*right);
    }

    std::unique_ptr<Stmt> clone() const override {
        // Clone the left-hand side; left->clone() returns a unique_ptr<Expr>.
        std::unique_ptr<Expr> leftCloneBase = left->clone();
        // Cast the result to a unique_ptr<Var>.
        Var* leftCloneRaw = dynamic_cast<Var*>(leftCloneBase.release());
        if (!leftCloneRaw) {
            throw std::runtime_error("Clone of left-hand side did not produce a Var instance");
        }
        std::unique_ptr<Var> clonedLeft(leftCloneRaw);
    
        // Clone the right-hand side normally (returns a unique_ptr<Expr>).
        std::unique_ptr<Expr> clonedRight = right->clone();
    
        // Return a new Assign node constructed with the cloned children.
        return std::make_unique<Assign>(std::move(clonedLeft), std::move(clonedRight));
    }
    
    std::unique_ptr<Var> left;
    std::unique_ptr<Expr> right;
};

// Function call statement
class FuncCallStmt : public Stmt
{
public:
    explicit FuncCallStmt(std::unique_ptr<FuncCall> call)
        : Stmt(StatementType::FUNCTIONCALL_STMT), call(std::move(call)) {}
    void accept(ASTVisitor &visitor)  override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)  override
    {
        cout<<"Hey funcCall\n"; 
        visitor->visitFuncCall(*call);
    }
    std::unique_ptr<Stmt> clone() const override {
        // Clone the function call; call->clone() returns a unique_ptr<Expr>.
        std::unique_ptr<Expr> callCloneBase = call->clone();
        // Cast the result to a unique_ptr<FuncCall>.
        FuncCall* callCloneRaw = dynamic_cast<FuncCall*>(callCloneBase.release());
        if (!callCloneRaw) {
            throw std::runtime_error("Clone of FuncCall did not produce a FuncCall instance");
        }
        std::unique_ptr<FuncCall> clonedCall(callCloneRaw);
    
        // Return a new FuncCallStmt node with the cloned FuncCall.
        return std::make_unique<FuncCallStmt>(std::move(clonedCall));
    }
    std::unique_ptr<FuncCall> call;
};

// Program is the root of our AST
class Program
{
public:
    explicit Program(std::vector<std::unique_ptr<Stmt>> Statements, vector<std::unique_ptr<Decl>> Declarations)
        : statements(std::move(Statements)), declarations(std::move(Declarations)) {}
    void accept(ASTVisitor &visitor)
    {
        visitor.visit(*this);
    }
    void accept(ExpoSEVisitor *visitor)
    {

        // for (auto &decl : declarations)
        // {
        //     visitor->visitDecl(*decl);
        // }

        for (auto &stmt : statements)
        { // Use  reference to avoid unnecessary copies
            visitor->visitStmt(*stmt);
        }
    }

    std::vector<std::unique_ptr<Stmt>> statements;
};

#endif