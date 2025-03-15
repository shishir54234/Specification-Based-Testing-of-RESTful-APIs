// test.cpp
#include <iostream>
#include <memory>
#include <vector>
#include "ast.hpp"    // Your AST definitions
#include "ima.hpp"    // Contains the IMA(...) function
#include "visitor.hpp"// Contains PrintVisitor

using namespace std;

int main() {
    // -------------------------------
    // 1. Build the client program AST.
    // -------------------------------
    vector<unique_ptr<Stmt>> stmts;

    // Statement 1: user = input(var)
    {
        auto lhs = make_unique<Var>("user");
        vector<unique_ptr<Expr>> inputArgs;
        // Here "var" is a placeholder (could be an empty or dummy variable).
        inputArgs.push_back(make_unique<Var>("var"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    // Statement 2: pass = input(var)
    {
        auto lhs = make_unique<Var>("pass");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("var"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    // Statement 3: signup(user, pass)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("user"));
        args.push_back(make_unique<Var>("pass"));
        auto signupCall = make_unique<FuncCall>("signup", move(args));
        auto signupStmt = make_unique<FuncCallStmt>(move(signupCall));
        stmts.push_back(move(signupStmt));
    }
    Program clientProgram(move(stmts));

    // -------------------------------
    // 2. Build the API specification AST.
    // -------------------------------
    // We assume the signup API spec uses formal parameters "u" and "p"
    // with precondition: not_in(u, U)
    // and postcondition: mapping(u, p, U)
    // (After renaming via the environment, these become "user not in U" and "user->pass in U".)

    // Build precondition: not_in(u, U)
    vector<unique_ptr<Expr>> preArgs;
    preArgs.push_back(make_unique<Var>("u"));
    preArgs.push_back(make_unique<Var>("U"));
    auto preExpr = make_unique<FuncCall>("not_in", move(preArgs));

    // Build API call: signup(u, p)
    vector<unique_ptr<Expr>> apiCallArgs;
    apiCallArgs.push_back(make_unique<Var>("u"));
    apiCallArgs.push_back(make_unique<Var>("p"));
    auto apiCallFunc = make_unique<FuncCall>("signup", move(apiCallArgs));

    // Build postcondition: mapping(u, p, U)
    vector<unique_ptr<Expr>> postArgs;
    postArgs.push_back(make_unique<Var>("u"));
    postArgs.push_back(make_unique<Var>("p"));
    postArgs.push_back(make_unique<Var>("U"));
    auto postExpr = make_unique<FuncCall>("mapping", move(postArgs));

    // Create the Response object with the postcondition expression.
    Response resp(HTTPResponseCode::CREATED_201, move(postExpr));

    // Build the APIcall node using the signup call and the response.
    auto apiCall = make_unique<APIcall>(move(apiCallFunc), std::move(resp));

    // Specify the formal parameters for the API block.
    vector<string> formalParams = {"u", "p"};

    // Create the API block using the precondition, APIcall, and formal parameters.
    // Note: The API constructor is assumed to take (precondition, APIcall, response, formalParams).
    // If your API constructor is different, adjust accordingly.
    auto apiBlock = make_unique<API>(
        move(preExpr),
        move(apiCall),
        // We recreate a Response for the API block (postcondition is the same)
        Response(HTTPResponseCode::CREATED_201, 
            make_unique<FuncCall>("mapping", vector<unique_ptr<Expr>>{
                make_unique<Var>("u"),
                make_unique<Var>("p"),
                make_unique<Var>("U")
            })
        ),
        formalParams
    );

    // Build the Spec. For simplicity, we leave globals, inits, and functions empty.
    vector<unique_ptr<Decl>> globals;
    vector<unique_ptr<Init>> inits;
    vector<unique_ptr<FuncDecl>> functions;
    vector<unique_ptr<API>> apiBlocks;
    apiBlocks.push_back(move(apiBlock));

    Spec spec(move(globals), move(inits), move(functions), move(apiBlocks));

    // -------------------------------
    // 3. Run the IMA algorithm.
    // -------------------------------
    // This should transform the client program by inserting the API precondition (as assert)
    // before the signup call and the postcondition (as assume) after the signup call.
    Program transformed = IMA(clientProgram, spec);

    // -------------------------------
    // 4. Print the transformed program.
    // -------------------------------
    PrintVisitor printer;
    transformed.accept(&printer);

    return 0;
}
