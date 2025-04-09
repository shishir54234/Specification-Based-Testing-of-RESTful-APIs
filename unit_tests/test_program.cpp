#include <cassert>
#include <iostream>
#include "../ast.hpp"
#include "../ASTVis.hpp"

using namespace std;

int main() {
    // -------------------------------
    // 1. Build the client program AST.
    // -------------------------------
    vector<unique_ptr<Stmt>> stmts;
    vector<unique_ptr<Decl>> decls;

    // Declare variable: string username;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto userDecl = make_unique<Decl>("username", stringType->clone());
        decls.push_back(move(userDecl));
    }

    // Declare variable: string password;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto passDecl = make_unique<Decl>("password", stringType->clone());
        decls.push_back(move(passDecl));
    }
    // Declare variable: string username2;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto userDecl = make_unique<Decl>("username2", stringType->clone());
        decls.push_back(move(userDecl));
    }

    // Declare variable: string password2;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto passDecl = make_unique<Decl>("password2", stringType->clone());
        decls.push_back(move(passDecl));
    }

    // Declare variable: string username3;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto userDecl = make_unique<Decl>("username3", stringType->clone());
        decls.push_back(move(userDecl));
    }

    // Declare variable: string password3;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto passDecl = make_unique<Decl>("password3", stringType->clone());
        decls.push_back(move(passDecl));
    }

    // Declare variable: string username4;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto userDecl = make_unique<Decl>("username4", stringType->clone());
        decls.push_back(move(userDecl));
    }

    // Declare variable: string password4;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto passDecl = make_unique<Decl>("password4", stringType->clone());
        decls.push_back(move(passDecl));
    }

    // --- Signup Phase ---
    {
        // username = input("username");
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("username"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    {
        // password = input("password");
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("password"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    {
        // Signup API call: signup(username, password);
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto apicall = make_unique<FuncCall>("signup", move(args));
        auto apistmt = make_unique<FuncCallStmt>(move(apicall));
        stmts.push_back(move(apistmt));
    }
    // --- Re-enter username and password for signup success ---
    {
        auto lhs = make_unique<Var>("username2");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("username2"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    {
        auto lhs = make_unique<Var>("password2");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("password2"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username2"));
        args.push_back(make_unique<Var>("password2"));
        auto apicall = make_unique<FuncCall>("signup", move(args));
        auto apistmt = make_unique<FuncCallStmt>(move(apicall));
        stmts.push_back(move(apistmt));
    }
    // --- Login Phase ---
    {
        auto lhs = make_unique<Var>("username3");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("username3"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    {
        auto lhs = make_unique<Var>("password3");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("password3"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username3"));
        args.push_back(make_unique<Var>("password3"));
        auto apicall = make_unique<FuncCall>("login", move(args));
        auto apistmt = make_unique<FuncCallStmt>(move(apicall));
        stmts.push_back(move(apistmt));
    }
    {
        auto lhs = make_unique<Var>("username4");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("username4"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    {
        auto lhs = make_unique<Var>("password4");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("password4"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username4"));
        args.push_back(make_unique<Var>("password4"));
        auto apicall = make_unique<FuncCall>("login", move(args));
        auto apistmt = make_unique<FuncCallStmt>(move(apicall));
        stmts.push_back(move(apistmt));
    }

    Program clientProgram(move(stmts), move(decls));

    // -------------------------------
    // 2. Clone the program AST.
    // -------------------------------
    // Using the copy constructor, which is implemented to perform deep copying.
    Program clonedProgram(clientProgram);

    // -------------------------------
    // 3. Verify the clone.
    // -------------------------------
    // Check that the number of declarations and statements match.
    assert(clientProgram.declarations.size() == clonedProgram.declarations.size());
    assert(clientProgram.statements.size() == clonedProgram.statements.size());

    // For each declaration, verify that the names match and that the underlying pointers differ.
    for (size_t i = 0; i < clientProgram.declarations.size(); i++) {
        const auto& originalDecl = clientProgram.declarations[i];
        const auto& clonedDecl   = clonedProgram.declarations[i];
        // Check that the declaration names match.
        assert(originalDecl->name == clonedDecl->name);
        // Ensure that they are not the same object in memory.
        assert(originalDecl.get() != clonedDecl.get());
    }

    // Similarly, you can check for a few statements.
    // For example, check the first Assign statement.
    {
        // Find the first statement that is an Assign.
        Assign* origAssign = nullptr;
        Assign* cloneAssign = nullptr;
        for (const auto& stmt : clientProgram.statements) {
            origAssign = dynamic_cast<Assign*>(stmt.get());
            if (origAssign != nullptr) break;
        }
        for (const auto& stmt : clonedProgram.statements) {
            cloneAssign = dynamic_cast<Assign*>(stmt.get());
            if (cloneAssign != nullptr) break;
        }
        // If found, then check that the variable names and other properties match.
        if (origAssign && cloneAssign) {
            // Compare left-hand side variable names.
            assert(origAssign->left->name == cloneAssign->left->name);
            // Compare that the underlying pointers differ.
            assert(origAssign->left.get() != cloneAssign->left.get());
        }
    }
    
    cout << "Program clone test passed successfully!" << endl;
    return 0;
}
