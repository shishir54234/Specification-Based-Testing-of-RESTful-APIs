#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "ASTVis.hpp"
enum class HTTPResponseCode
{
    OK_200,
    CREATED_201,
    // Add other response codes as needed
};

// Forward declarations
class TypeExpr;
class Expr;
class FuncCall;

// Base classes for different types of declarations
class Decl
{
public:
    Decl(std::string name, std::unique_ptr<TypeExpr> typeExpr)
        : name(std::move(name)), type(std::move(typeExpr)) {}
    virtual ~Decl() = default;

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

class VariantConstructor
{
public:
    // Add necessary fields
};

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
class TypeDecl
{
public:
    virtual ~TypeDecl() = default;
};

class VariantDecl : public TypeDecl
{
public:
    std::vector<VariantConstructor> constructors;
};

class RecordDecl : public TypeDecl
{
public:
    RecordDecl(std::string name, std::vector<std::unique_ptr<Decl>> fields)
        : recname(std::move(name)), fields(std::move(fields)) {}

    std::string recname;
    std::vector<std::unique_ptr<Decl>> fields;
};

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
};
class Var : public Expr
{
public:
    explicit Var(std::string name) : name(std::move(name)) {}
    void accept(ASTVisitor& visitor) const override {
        visitor.visit(*this);
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
             std::vector<std::unique_ptr<Decl>> params,
             std::pair<HTTPResponseCode, std::unique_ptr<TypeExpr>> returnType)
        : name(std::move(name)), params(std::move(params)), returnType(std::move(returnType)) {}

    std::string name;
    std::vector<std::unique_ptr<Decl>> params;
    std::pair<HTTPResponseCode, std::unique_ptr<TypeExpr>> returnType;
};

// Initialization
class Init
{
public:

    Init(std::string varName, std::unique_ptr<Expr> expression)
        : varName(std::move(varName)), expr(std::move(expression)) {}

    std::string varName;
    std::unique_ptr<Expr> expr;
};

// API
class API
{
public:
    API(std::unique_ptr<Expr> precondition,
        std::unique_ptr<FuncCall> functionCall,
        std::pair<HTTPResponseCode, std::unique_ptr<Expr>> response)
        : pre(std::move(precondition)), call(std::move(functionCall)), response(std::move(response)) {}

    std::unique_ptr<Expr> pre;
    std::unique_ptr<FuncCall> call;
    std::pair<HTTPResponseCode, std::unique_ptr<Expr>> response;
};

// Block class (placeholder as it wasn't fully specified in the grammar)
class Block
{
public:
    
    // Implementation details
};

// Top-level Spec class
class Spec
{
public:
    Spec(std::vector<std::unique_ptr<Decl>> globals,
         std::vector<std::unique_ptr<TypeDecl>> types,
         std::unique_ptr<Init> init,
         std::vector<std::unique_ptr<FuncDecl>> functions,
         std::vector<std::unique_ptr<API>> blocks)
        : globals(std::move(globals)), types(std::move(types)), init(std::move(init)), functions(std::move(functions)), blocks(std::move(blocks)) {}

    std::vector<std::unique_ptr<Decl>> globals;
    std::vector<std::unique_ptr<TypeDecl>> types;
    std::unique_ptr<Init> init;
    std::vector<std::unique_ptr<FuncDecl>> functions;
    std::vector<std::unique_ptr<API>> blocks;
};