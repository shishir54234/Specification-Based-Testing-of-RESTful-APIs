#include <iostream>
#include <memory>
#include <vector>
#include "../ast.hpp"
#include "../IMA.hpp"
#include "../PrintVisitor.hpp"

using namespace std;

// Four API blocks are defined :

// Signup Notok API:

// Precondition: not_in(u, dom(U)) (user is not registered).

// API Call: signup_notok(u, p).

// Postcondition: not_in(U[u], p) (user is still not added).

// Response: BAD_REQUEST.

// Signup Success API:

// Precondition: not_in(u, dom(U)).

// API Call: signup_success(u, p).

// Postcondition: in(U[u], p) (user is added to U).

// Response: CREATED.

// Login Notok API:

// Precondition: in(u, dom(U)) (user exists).

// API Call: login_notok(u, p).

// Postcondition: not_in(T[u], token) (no token assigned).

// Response: BAD_REQUEST.

// Login Success API:

// Precondition: in(u, dom(U)).

// API Call: login_success(u, p).

// Postcondition: in(T[u], token) (token assigned).

// Response: OK.

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
    // Read username for signup attempt (first attempt, will fail)
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Read password for signup attempt (first attempt, will fail)
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call signup_notok(username, password) → signup failure path
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto signupNotokCall = make_unique<FuncCall>("signup_notok", move(args));
        auto signupNotokStmt = make_unique<FuncCallStmt>(move(signupNotokCall));
        stmts.push_back(move(signupNotokStmt));
    }
    // Re-read username for successful signup
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Re-read password for successful signup
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call signup_success(username, password) → signup success path
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto signupSuccessCall = make_unique<FuncCall>("signup_success", move(args));
        auto signupSuccessStmt = make_unique<FuncCallStmt>(move(signupSuccessCall));
        stmts.push_back(move(signupSuccessStmt));
    }

    // --- Login Phase ---
    // Re-read username for login failure attempt
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Re-read password for login failure attempt
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call login_notok(username, password) → login failure path
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto loginNotokCall = make_unique<FuncCall>("login_notok", move(args));
        auto loginNotokStmt = make_unique<FuncCallStmt>(move(loginNotokCall));
        stmts.push_back(move(loginNotokStmt));
    }
    // Re-read username for login success attempt
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Re-read password for login success attempt
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>(""));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call login_success(username, password) → login success path
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto loginSuccessCall = make_unique<FuncCall>("login_success", move(args));
        auto loginSuccessStmt = make_unique<FuncCallStmt>(move(loginSuccessCall));
        stmts.push_back(move(loginSuccessStmt));
    }

    Program clientProgram(move(stmts), move(decls));

    // -------------------------------
    // 2. Build the API specification AST.
    // -------------------------------
    vector<unique_ptr<API>> apiBlocks;

    // --- Signup Notok API ---
    {
        // Precondition: not_in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs, pre;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        pre.push_back(make_unique<FuncCall>("not_in", move(preArgs)));
        auto precondition = make_unique<FuncCall>("is_false", move(pre));

        // API Call: signup_notok(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("signup_notok", move(apiArgs));

        // Postcondition: not_in( U[u], p ) i.e. user is not added to U.
        vector<unique_ptr<Expr>> postArgs, postArgs1, post;
        vector<unique_ptr<Expr>> e2;
        // e2.push_back(make_unique<Var>("U"));
        postArgs1.push_back(make_unique<Var>("U")); // wrapper for U
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("mapped_value", move(postArgs1))); // represents U[u]
        postArgs.push_back(make_unique<Var>("p"));
        post.push_back(make_unique<FuncCall>("equals", move(postArgs)));
        auto postcondition = make_unique<FuncCall>("is_false", move(post));
        Response response(HTTPResponseCode::BAD_REQUEST_400, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // --- Signup Success API ---
    {
        // Precondition: not_in(u, dom(U))
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

    // --- Login Notok API ---
    {
        // Precondition: in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("p"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        h.push_back(make_unique<Var>("u"));
        preArgs.push_back(make_unique<FuncCall>("mapped_value", move(h)));
        vector<unique_ptr<Expr>> preArgs0, pre;
        preArgs0.push_back(make_unique<FuncCall>("equals", move(preArgs)));
        vector<unique_ptr<Expr>> h2;
        h2.push_back(make_unique<Var>("T"));
        h2.push_back(make_unique<Var>("token"));
        preArgs0.push_back(make_unique<FuncCall>("in_dom", move(h2)));
        pre.push_back(make_unique<FuncCall>("and_operator", move(preArgs0)));
        auto precondition = make_unique<FuncCall>("is_false", move(pre));

        // API Call: login_notok(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("login_notok", move(apiArgs));

        // Postcondition: not_in( T[u], token ) i.e. no token is assigned.
        vector<unique_ptr<Expr>> postArgs, postArgs1, post;
        vector<unique_ptr<Expr>> e2;
        // e2.push_back(make_unique<Var>("T"));
        postArgs1.push_back(make_unique<Var>("T")); // wrapper for T
        postArgs1.push_back(make_unique<Var>("token"));
        postArgs.push_back(make_unique<FuncCall>("mapped_value", move(postArgs1))); // represents T[u]
        postArgs.push_back(make_unique<Var>("u"));
        post.push_back(make_unique<FuncCall>("equals", move(postArgs)));
        auto postcondition = make_unique<FuncCall>("is_false", move(post));
        Response response(HTTPResponseCode::BAD_REQUEST_400, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // --- Login Success API ---
    {
        // Precondition: in(u, dom(U))
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

    // -------------------------------
    // 3. Define Global Maps
    // -------------------------------
    vector<unique_ptr<Decl>> globals;

    auto domain = make_unique<TypeConst>("string");
    auto range = make_unique<TypeConst>("string");

    auto mapTypeU = make_unique<MapType>(move(domain), make_unique<TypeConst>("string"));
    auto mapTypeT = make_unique<MapType>(make_unique<TypeConst>("string"), make_unique<TypeConst>("string"));
    auto token = make_unique<TypeConst>("string");

    globals.push_back(make_unique<Decl>("U", move(mapTypeU)));
    globals.push_back(make_unique<Decl>("T", move(mapTypeT)));
    globals.push_back(make_unique<Decl>("token", move(token)));

    vector<unique_ptr<Init>> inits;
    inits.push_back(make_unique<Init>("U", make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>())));
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

//