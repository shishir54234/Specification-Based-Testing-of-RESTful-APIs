#include <iostream>
#include <memory>
#include <vector>
#include "../ast.hpp"
#include "../IMA.hpp"
#include "../PrintVisitor.hpp"

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
        // Precondition: not_in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        auto precondition = make_unique<FuncCall>("not_in", move(preArgs));

        // API Call: signup(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("signup", move(apiArgs));

        // Postcondition: U[u] = p (User added)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("U"));
        postArgs1.push_back(make_unique<FuncCall>(" ", move(e2))); // Wrapper for U
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // U[u]
        postArgs.push_back(make_unique<Var>("p"));

        auto postcondition = make_unique<FuncCall>("in", move(postArgs));
        Response response(HTTPResponseCode::CREATED_201, move(postcondition));

        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // --- Login API ---
    {
        // Precondition: in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        auto precondition = make_unique<FuncCall>("in", move(preArgs));

        // API Call: login(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("login", move(apiArgs));

        // Postcondition: T[u] = token (Token stored)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("T"));
        postArgs1.push_back(make_unique<FuncCall>(" ", move(e2))); // Wrapper for T
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // T[u]
        postArgs.push_back(make_unique<Var>("token"));

        auto postcondition = make_unique<FuncCall>("in", move(postArgs));
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

    vector<unique_ptr<Init>> inits;
    inits.push_back(std::make_unique<Init>("U", std::make_unique<Map>(std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>>())));
    inits.push_back(std::make_unique<Init>("T", std::make_unique<Map>(std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>>())));

    Spec spec(move(globals), move(inits), {}, move(apiBlocks));

    // -------------------------------
    // 4. Run IMA algorithm & Print result.
    // -------------------------------
    Program transformed = IMA(clientProgram, spec);
    PrintVisitor printer;
    transformed.accept(printer);

    return 0;
}
