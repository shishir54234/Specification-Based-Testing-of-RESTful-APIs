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

    // --- LOGIN FAIL ---
    // 1) Read username
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("")); // placeholder
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // 2) Read password
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // 3) Call login_notok(username, password)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto loginFailCall = make_unique<FuncCall>("login_notok", move(args));
        auto loginFailStmt = make_unique<FuncCallStmt>(move(loginFailCall));
        stmts.push_back(move(loginFailStmt));
    }

    // --- LOGIN SUCCESS ---
    // 4) Read username again
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // 5) Read password again
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // 6) Call login_ok(username, password)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto loginOkCall = make_unique<FuncCall>("login_ok", move(args));
        auto loginOkStmt = make_unique<FuncCallStmt>(move(loginOkCall));
        stmts.push_back(move(loginOkStmt));
    }

    // --- ONBOARD (if onboard=false) ---
    // 7) Call do_onboard()
    {
        vector<unique_ptr<Expr>> args;
        auto onboardCall = make_unique<FuncCall>("do_onboard", move(args));
        auto onboardStmt = make_unique<FuncCallStmt>(move(onboardCall));
        stmts.push_back(move(onboardStmt));
    }

    // --- MAP PAGE (if onboard=true) ---
    // 8) Call go_map()
    {
        vector<unique_ptr<Expr>> args;
        auto goMapCall = make_unique<FuncCall>("go_map", move(args));
        auto goMapStmt = make_unique<FuncCallStmt>(move(goMapCall));
        stmts.push_back(move(goMapStmt));
    }

    // --- LOGOUT ---
    // 9) Call logout()
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        auto logoutCall = make_unique<FuncCall>("logout", move(args));
        auto logoutStmt = make_unique<FuncCallStmt>(move(logoutCall));
        stmts.push_back(move(logoutStmt));
    }

    // Create the client program AST
    Program clientProgram(move(stmts), move(decls));

    // -------------------------------
    // 2. Build the API specification AST.
    // -------------------------------
    vector<unique_ptr<API>> apiBlocks;

    // 1. login_notok(u, p)
    {
        // Precondition: in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        auto precondition = make_unique<FuncCall>("in", move(preArgs));

        // API Call: login_notok(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("login_notok", move(apiArgs));

        // Postcondition: not_in(T[u], token)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("T"));
        postArgs1.push_back(make_unique<FuncCall>(" ", move(e2))); // wrapper for T
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // T[u]
        postArgs.push_back(make_unique<Var>("token"));
        auto postcondition = make_unique<FuncCall>("not_in", move(postArgs));

        Response response(HTTPResponseCode::BAD_REQUEST_400, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // 2. login_ok(u, p)
    {
        // Precondition: in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        auto precondition = make_unique<FuncCall>("in", move(preArgs));

        // API Call: login_ok(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("login_ok", move(apiArgs));

        // Postcondition: in(T[u], token)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("T"));
        postArgs1.push_back(make_unique<FuncCall>(" ", move(e2)));
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // T[u]
        postArgs.push_back(make_unique<Var>("token"));
        auto postcondition = make_unique<FuncCall>("in", move(postArgs));

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // 3. do_onboard()
    {
        // Precondition: onboard = false (represented as onboard_false())
        auto precondition = make_unique<FuncCall>("onboard_false", vector<unique_ptr<Expr>>{});

        // API Call: do_onboard()
        auto apiCallFunc = make_unique<FuncCall>("do_onboard", vector<unique_ptr<Expr>>{});

        // Postcondition: onboard = true (represented as onboard_true())
        auto postcondition = make_unique<FuncCall>("onboard_true", vector<unique_ptr<Expr>>{});

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // 4. go_map()
    {
        // Precondition: onboard = true (represented as onboard_true())
        auto precondition = make_unique<FuncCall>("onboard_true", vector<unique_ptr<Expr>>{});

        // API Call: go_map()
        auto apiCallFunc = make_unique<FuncCall>("go_map", vector<unique_ptr<Expr>>{});

        // Postcondition: in(location, "map")
        vector<unique_ptr<Expr>> postArgs;
        postArgs.push_back(make_unique<Var>("location"));
        postArgs.push_back(make_unique<Var>("\"map\""));
        auto postcondition = make_unique<FuncCall>("in", move(postArgs));

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // 5. logout()
    {
        // Precondition: in(username, dom(T))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("T"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        auto precondition = make_unique<FuncCall>("in", move(preArgs));

        // API Call: logout(username)
        auto apiCallFunc = make_unique<FuncCall>("logout", move(apiArgs));

        // Postcondition: not_in(T[username], token)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("T"));
        postArgs1.push_back(make_unique<FuncCall>(" ", move(e2))); // wrapper for T
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // T[username]
        postArgs.push_back(make_unique<Var>("token"));
        auto postcondition = make_unique<FuncCall>("not_in", move(postArgs));

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // ------------- Globals / Spec -------------
    vector<unique_ptr<Decl>> globals;
    // U: Map from user -> password
    auto mapTypeU = make_unique<MapType>(
        make_unique<TypeConst>("string"),
        make_unique<TypeConst>("string"));
    globals.push_back(make_unique<Decl>("U", move(mapTypeU)));

    // T: Map from user -> token
    auto mapTypeT = make_unique<MapType>(
        make_unique<TypeConst>("string"),
        make_unique<TypeConst>("string"));
    globals.push_back(make_unique<Decl>("T", move(mapTypeT)));

    // onboard: boolean
    globals.push_back(make_unique<Decl>("onboard", make_unique<TypeConst>("bool")));

    // location: string
    globals.push_back(make_unique<Decl>("location", make_unique<TypeConst>("string")));

    vector<unique_ptr<Init>> inits;
    // Initialize U, T as empty maps
    inits.push_back(make_unique<Init>("U", make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>())));
    inits.push_back(make_unique<Init>("T", make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>())));
    // onboard := false (using Var as a literal substitute)
    {
        auto rhs = make_unique<Var>("false");
        inits.push_back(make_unique<Init>("onboard", move(rhs)));
    }
    // location := "" (using Var as a literal substitute)
    {
        auto rhs = make_unique<Var>("\"\"");
        inits.push_back(make_unique<Init>("location", move(rhs)));
    }

    vector<unique_ptr<FuncDecl>> functions;

    Spec spec(move(globals), move(inits), move(functions), move(apiBlocks));

    cout << "API specification for login_notok, login_ok, do_onboard, go_map, logout created.\n";

    // Print the client program AST
    PrintVisitor printer;
    // clientProgram.accept(printer);
    Program transformed = IMA(clientProgram, spec);
    transformed.accept(printer);

    return 0;
}
