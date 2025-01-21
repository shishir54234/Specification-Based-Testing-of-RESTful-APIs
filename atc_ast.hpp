#include <vector>
#include <string>
#include <memory>

// Forward declarations
class Exp;
class Var;
class FuncCall;

// Base class for all statements
class Stmt
{
public:
    virtual ~Stmt() = default;
};

// Assignment statement: l = r
class Assign : public Stmt
{
public:
    Assign(std::unique_ptr<Var> left, std::unique_ptr<Exp> right)
        : left(std::move(left)), right(std::move(right)) {}

    std::unique_ptr<Var> left;
    std::unique_ptr<Exp> right;
};

// Function call statement
class FuncCallStmt : public Stmt
{
public:
    explicit FuncCallStmt(std::unique_ptr<FuncCall> call)
        : call(std::move(call)) {}

    std::unique_ptr<FuncCall> call;
};

// Program is the root of our AST
class Program
{
public:
    explicit Program(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}

    std::vector<std::unique_ptr<Stmt>> statements;
};