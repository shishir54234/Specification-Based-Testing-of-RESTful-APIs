#include <iostream>
#include <memory>
#include <vector>
#include "ast.hpp"       // Contains our AST node definitions (Var, FuncCall, Assign, FuncCallStmt, Program, etc.)
#include "visitor.hpp"   // For visitors if needed

using namespace std;

int main() {
    vector<unique_ptr<Stmt>> stmts;

    // --- Block 1: Signup ---

    // username = input();
    {
        auto varUsername = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs; // no arguments for input()
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignUsername = make_unique<Assign>(move(varUsername), move(inputCall));
        stmts.push_back(move(assignUsername));
    }

    // password = input();
    {
        auto varPassword = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignPassword = make_unique<Assign>(move(varPassword), move(inputCall));
        stmts.push_back(move(assignPassword));
    }

    // signup(username, password);
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto signupCall = make_unique<FuncCall>("signup", move(args));
        auto signupStmt = make_unique<FuncCallStmt>(move(signupCall));
        stmts.push_back(move(signupStmt));
    }

    // --- Block 2: Login ---

    // username = input();
    {
        auto varUsername = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignUsername = make_unique<Assign>(move(varUsername), move(inputCall));
        stmts.push_back(move(assignUsername));
    }

    // password = input();
    {
        auto varPassword = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignPassword = make_unique<Assign>(move(varPassword), move(inputCall));
        stmts.push_back(move(assignPassword));
    }

    // login(username, password);
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto loginCall = make_unique<FuncCall>("login", move(args));
        auto loginStmt = make_unique<FuncCallStmt>(move(loginCall));
        stmts.push_back(move(loginStmt));
    }

    // Create the final program from the list of statements.
    Program program(move(stmts));

    cout << "Constructed program with " << program.statements.size() << " statements." << endl;

    // (Optional) Use a visitor to print or process the AST.
    
    return 0;
}
