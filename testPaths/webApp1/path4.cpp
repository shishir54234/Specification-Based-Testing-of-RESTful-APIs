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

    // --- Signup Phase ---
    // Read username
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("")); // placeholder for input
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Read password
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call signup_success(username, password)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto signupSuccessCall = make_unique<FuncCall>("signup_success", move(args));
        auto signupSuccessStmt = make_unique<FuncCallStmt>(move(signupSuccessCall));
        stmts.push_back(move(signupSuccessStmt));
    }

    // --- Login Phase ---
    // Re-read username
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("")); // placeholder for login input
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Re-read password
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("")); // placeholder for login input
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call login_success(username, password)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto loginSuccessCall = make_unique<FuncCall>("login_success", move(args));
        auto loginSuccessStmt = make_unique<FuncCallStmt>(move(loginSuccessCall));
        stmts.push_back(move(loginSuccessStmt));
    }

    // --- Logout Phase ---
    // Call logout()
    {
        vector<unique_ptr<Expr>> args; // no arguments
        args.push_back(make_unique<Var>("username"));
        auto logoutCall = make_unique<FuncCall>("logout", move(args));
        auto logoutStmt = make_unique<FuncCallStmt>(move(logoutCall));
        stmts.push_back(move(logoutStmt));
    }

    // Build the client program
    Program clientProgram(move(stmts), move(decls));

    // -------------------------------
    // 2. Build the API specification AST.
    // -------------------------------
    vector<unique_ptr<API>> apiBlocks;

    // --- Signup Success API ---
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
        auto apiCallFunc = make_unique<FuncCall>("signup_success", move(apiArgs));

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

    // --- Login Success API ---
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
        auto apiCallFunc = make_unique<FuncCall>("login_success", move(apiArgs));

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

    // --- Logout API ---
    {
        // Precondition: in(username, dom(T))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("T"));
        h.push_back(make_unique<Var>("token"));
        preArgs.push_back(make_unique<FuncCall>("mapped_value", move(h)));
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        auto precondition = make_unique<FuncCall>("equals", move(preArgs));

        // API Call: logout(username)
        auto apiCallFunc = make_unique<FuncCall>("logout", move(apiArgs));

        // Postcondition: not_in(T[username], token)
        vector<unique_ptr<Expr>> postArgs;
        postArgs.push_back(make_unique<Var>("token"));
        vector<unique_ptr<Expr>> h2;
        h2.push_back(make_unique<Var>("T"));
        postArgs.push_back(make_unique<FuncCall>("dom", move(h2)));
        // vector<unique_ptr<Expr>> apiArgs;
        // apiArgs.push_back(make_unique<Var>("u"));
        auto postcondition = make_unique<FuncCall>("not_in", move(postArgs));

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // -------------------------------
    // 3. Define Global Maps and Variables (onboard and token are global)
    // -------------------------------
    vector<unique_ptr<Decl>> globals;
    // U: Map from user -> password
    auto domain = make_unique<TypeConst>("string");
    auto range = make_unique<TypeConst>("string");
    auto mapTypeU = make_unique<MapType>(move(domain), make_unique<TypeConst>("string"));
    globals.push_back(make_unique<Decl>("U", move(mapTypeU)));

    // T: Map from user -> token
    auto mapTypeT = make_unique<MapType>(make_unique<TypeConst>("string"), make_unique<TypeConst>("string"));
    globals.push_back(make_unique<Decl>("T", move(mapTypeT)));

    // token: string (global)
    globals.push_back(make_unique<Decl>("token", make_unique<TypeConst>("string")));

    vector<unique_ptr<Init>> inits;
    inits.push_back(make_unique<Init>("U", make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>())));
    inits.push_back(make_unique<Init>("T", make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>())));
    // No initialization for token provided (assumed to be empty by default)

    vector<unique_ptr<FuncDecl>> functions;

    Spec spec(move(globals), move(inits), move(functions), move(apiBlocks));

    cout << "API specification for signup_success, login_success, and logout created.\n";

    // -------------------------------
    // 4. Run IMA algorithm & Print result.
    // -------------------------------
    Program transformed = IMA(clientProgram, spec);
    PrintVisitor printer;
    transformed.accept(printer);

    return 0;
}
