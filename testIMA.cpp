#include <iostream>
#include <memory>
#include <vector>
#include "ast.hpp"    // Your AST definitions
#include "IMA.hpp"    // Contains the IMA(...) function
#include "visitor.hpp"// Contains PrintVisitor

using namespace std;

int main() {
    // -------------------------------
    // 1. Build the client program AST.
    // -------------------------------
    vector<unique_ptr<Stmt>> stmts;

    // Statement 1: user = input(var)
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    // Statement 2: pass = input(var)
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    // Statement 3: signup(user, pass)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto signupCall = make_unique<FuncCall>("signup", move(args));
        auto signupStmt = make_unique<FuncCallStmt>(move(signupCall));
        stmts.push_back(move(signupStmt));
    }
    Program clientProgram(move(stmts));

    // -------------------------------
    // 2. Build the API specification AST.
    // -------------------------------
    // Precondition: not_in(u, U)
    vector<unique_ptr<Expr>> preArgs;
    preArgs.push_back(make_unique<Var>("u"));
    preArgs.push_back(make_unique<Var>("U"));
    auto precondition = make_unique<FuncCall>("not_in", move(preArgs));

    // API Call: signup(u, p)
    vector<unique_ptr<Expr>> apiArgs;
    apiArgs.push_back(make_unique<Var>("u"));
    apiArgs.push_back(make_unique<Var>("p"));
    auto apiCallFunc = make_unique<FuncCall>("signup", move(apiArgs));

    // Postcondition: mapping(u, p, U)
    vector<unique_ptr<Expr>> postArgs;
    postArgs.push_back(make_unique<Var>("u"));
    postArgs.push_back(make_unique<Var>("p"));
    postArgs.push_back(make_unique<Var>("U"));
    auto postcondition = make_unique<FuncCall>("mapping", move(postArgs));

    // Create the Response object for the postcondition
    Response response(HTTPResponseCode::CREATED_201, move(postcondition));

    // Create the API Call node
    auto apiCall = make_unique<APIcall>(std::move(apiCallFunc), std::move(response));

    // Create the API Block
    auto apiBlock = make_unique<API>(
        std::move(precondition), 
        std::move(apiCall),
        std::move(response)
    );

    // Build the Spec
    vector<unique_ptr<Decl>> globals;

    auto domain = std::make_unique<TypeConst>("string");
    auto range = std::make_unique<TypeConst>("string");
    
    auto mapType = std::make_unique<MapType>(std::move(domain), std::move(range));
    
    // Create a unique_ptr<Decl> and store it in globals
    globals.push_back(std::make_unique<Decl>("U", std::move(mapType)));
    

    vector<unique_ptr<Init>> inits;

    vector<unique_ptr<FuncDecl>> functions;

    auto usernameType = std::make_unique<TypeConst>("string");
    auto passwordType = std::make_unique<TypeConst>("string");
    
    std::vector<std::unique_ptr<TypeExpr>> params;
    params.push_back(std::move(usernameType));
    params.push_back(std::move(passwordType));
    
    // Step 2: Define return type (HTTP CREATED_201 + string response)
    auto returnType = std::make_unique<TypeConst>("string");
    std::vector<std::unique_ptr<TypeExpr>> returnTypes;
    returnTypes.push_back(std::move(returnType));
    
    // Step 3: Create the FuncDecl for signup as a unique_ptr
    auto signup = std::make_unique<FuncDecl>(
        "signup",
        std::move(params),
        std::make_pair(HTTPResponseCode::CREATED_201, std::move(returnTypes))
    );
    
    // Step 4: Add to the vector
    functions.push_back(std::move(signup));
    

    vector<unique_ptr<API>> apiBlocks;
    apiBlocks.push_back(std::move(apiBlock));

    Spec spec(
        move(globals), 
        move(inits), 
        move(functions), 
        move(apiBlocks)
    );

    // -------------------------------
    // 3. Run the IMA algorithm.
    // -------------------------------
    Program transformed = IMA(clientProgram, spec);

    // -------------------------------
    // 4. Print the transformed program.
    // -------------------------------
    PrintVisitor printer;
    transformed.accept(printer);


    return 0;
}
