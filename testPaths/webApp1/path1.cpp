#include <iostream>
#include <memory>
#include <vector>
#include "../../ast.hpp"
#include "../../IMA.hpp"
#include "../../PrintVisitor.hpp"

using namespace std;

int main()
{
    // -------------------------------
    // 1. Build the client program AST.
    // -------------------------------
    vector<unique_ptr<Stmt>> stmts;
    vector<unique_ptr<Decl>> decls;

    // Statement 1: string username;
    {
        auto stringType = std::make_unique<TypeConst>("string");
        auto userDecl = std::make_unique<Decl>("username", stringType->clone());
        decls.push_back(move(userDecl));
    }

    // Statement 2: username = input()
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    // Statement 3: string password;
    {
        auto stringType = std::make_unique<TypeConst>("string");
        auto passDecl = std::make_unique<Decl>("password", stringType->clone());
        decls.push_back(move(passDecl));
    }

    // Statement 4: password = input()
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    // Statement 5: signup(username, password)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto signupCall = make_unique<FuncCall>("signup", move(args));
        auto signupStmt = make_unique<FuncCallStmt>(move(signupCall));
        stmts.push_back(move(signupStmt));
    }

    // Statement 6: username = input() (Again for login)
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    // Statement 7: password = input() (Again for login)
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    // Statement 8: login(username, password)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto loginCall = make_unique<FuncCall>("login", move(args));
        auto loginStmt = make_unique<FuncCallStmt>(move(loginCall));
        stmts.push_back(move(loginStmt));
    }

    Program clientProgram(move(stmts), move(decls));

    // -------------------------------
    // 2. Build the API specification AST.
    // -------------------------------

    vector<unique_ptr<API>> apiBlocks;

    // --- Signup API ---
    {
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        auto precondition = make_unique<FuncCall>("not_in", move(preArgs));

        // API Call: signup_success(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("signup", move(apiArgs));

        // Postcondition: U[u] = p (User added to U)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        // e2.push_back(make_unique<Var>("U"));
        postArgs1.push_back(make_unique<Var>("U")); // wrapper for U
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("mapped_value", move(postArgs1))); // represents U[u]
        postArgs.push_back(make_unique<Var>("p"));
        auto postcondition = make_unique<FuncCall>("equals", move(postArgs));
        Response response(HTTPResponseCode::CREATED_201, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // --- Login API ---
    {
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("p"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        h.push_back(make_unique<Var>("u"));
        preArgs.push_back(make_unique<FuncCall>("mapped_value", move(h)));
        vector<unique_ptr<Expr>> preArgs0;
        preArgs0.push_back(make_unique<FuncCall>("equals", move(preArgs)));
        vector<unique_ptr<Expr>> h2;
        h2.push_back(make_unique<Var>("T"));
        h2.push_back(make_unique<Var>("token"));
        preArgs0.push_back(make_unique<FuncCall>("in_dom", move(h2)));
        auto precondition = make_unique<FuncCall>("and_operator", move(preArgs0));

        // API Call: login_success(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("login", move(apiArgs));

        // Postcondition: T[u] = token (Token stored in T)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        // e2.push_back(make_unique<Var>("T"));
        postArgs1.push_back(make_unique<Var>("T")); // wrapper for T
        postArgs1.push_back(make_unique<Var>("token"));
        postArgs.push_back(make_unique<FuncCall>("mapped_value", move(postArgs1))); // represents T[u]
        postArgs.push_back(make_unique<Var>("u"));
        auto postcondition = make_unique<FuncCall>("equals", move(postArgs));
        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // -------------------------------
    // 3. Define Global Maps
    // -------------------------------
    vector<unique_ptr<Decl>> globals;

    auto domain = std::make_unique<TypeConst>("string");
    auto range = std::make_unique<TypeConst>("string");

    auto mapTypeU = std::make_unique<MapType>(std::move(domain), std::make_unique<TypeConst>("string"));
    auto mapTypeT = std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"));

    globals.push_back(std::make_unique<Decl>("U", std::move(mapTypeU)));
    globals.push_back(std::make_unique<Decl>("T", std::move(mapTypeT)));
    // Step 1: Create vector and use emplace_back
    vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> uEntries;

    uEntries.emplace_back(make_unique<Var>("alice"), make_unique<String>("alicepass"));
    uEntries.emplace_back(make_unique<Var>("bob"), make_unique<String>("bob123"));
    uEntries.emplace_back(make_unique<Var>("charlie"), make_unique<String>("qwerty"));

    // Step 2: Move into Map

    vector<unique_ptr<Init>> inits;
    inits.push_back(make_unique<Init>("U", make_unique<Map>(std::move(uEntries))));

    inits.push_back(make_unique<Init>("T", make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>())));

    Spec spec(move(globals), move(inits), {}, move(apiBlocks));

    // -------------------------------
    // 4. Run IMA algorithm & Print result.
    // -------------------------------

    Program transformed = IMA(clientProgram, spec);
    PrintVisitor printer;
    transformed.accept(printer);

    return 0;
}
