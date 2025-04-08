#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include "ast.hpp"       // Your AST definitions (with deep copy via clone())
#include "ima.hpp"       // Contains renameExprWithMap and Env definition
#include "visitor.hpp"   // Contains PrintVisitor

using namespace std;
using Env = map<string, string>;

int main() {
    // Create a simple expression: not_in(u, U)
    vector<unique_ptr<Expr>> args;
    args.push_back(make_unique<Var>("u"));
    args.push_back(make_unique<Var>("U"));
    unique_ptr<Expr> expr = make_unique<FuncCall>("not_in", move(args));

    // Define an environment mapping:
    // formal "u" maps to "user" and formal "U" maps to "UserSet"
    Env env;
    env["u"] = "user";
    env["U"] = "UserSet";

    // Apply renameExprWithMap on the expression.
    unique_ptr<Expr> renamedExpr = renameExprWithMap(expr.get(), env);

    // Print the renamed expression using PrintVisitor.
    PrintVisitor printer;
    cout << "Renamed Expression AST:" << endl;
    renamedExpr->accept(printer);

    return 0;
}
