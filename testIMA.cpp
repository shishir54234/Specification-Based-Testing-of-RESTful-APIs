// #include <iostream>
// #include <memory>
// #include <vector>
// #include "ast.hpp"   
// #include "IMA.hpp"    
// #include "PrintVisitor.hpp"
// #include "jsCodeGen.hpp"

// using namespace std;

// int main() {
//     // -------------------------------
//     // 1. Build the client program AST.
//     // -------------------------------
//     vector<unique_ptr<Stmt>> stmts;
//     vector<unique_ptr<Decl>> decls;
//     //statemet 1: string username;
//     {
//         auto stringType = std::make_unique<TypeConst>("string");
//         auto userDecl = std::make_unique<Decl>("username", stringType->clone());
//         decls.push_back(move(userDecl));
//     }

//     // Statement 2: username = input(var)
//     {
//         auto lhs = make_unique<Var>("username");
//         vector<unique_ptr<Expr>> inputArgs;
//         inputArgs.push_back(make_unique<String>("username"));
//         auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
//         auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
//         stmts.push_back(move(assignStmt));
//     }

//     // statement 3: string password;
//     {
//         auto stringType = std::make_unique<TypeConst>("string");
//         auto userDecl = std::make_unique<Decl>("password", stringType->clone());
//         decls.push_back(move(userDecl));

//     }

//     // Statement 4: password = input(var)
//     {
//         auto lhs = make_unique<Var>("password");
//         vector<unique_ptr<Expr>> inputArgs;
//         inputArgs.push_back(make_unique<String>("password"));
//         auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
//         auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
//         stmts.push_back(move(assignStmt));
//     }

//     // Statement 3: signup(user, pass)
//     {
//         vector<unique_ptr<Expr>> args;
//         args.push_back(make_unique<Var>("username"));
//         args.push_back(make_unique<Var>("password"));
//         auto signupCall = make_unique<FuncCall>("signup", move(args));
//         auto signupStmt = make_unique<FuncCallStmt>(move(signupCall));
//         stmts.push_back(move(signupStmt));
//     }
//     Program clientProgram(move(stmts),move(decls));

//     // -------------------------------
//     // 2. Build the API specification AST.
//     // -------------------------------
//     // Precondition: not_in(u, U)
//     vector<unique_ptr<Expr>> preArgs;
//     preArgs.push_back(make_unique<Var>("u"));
//     preArgs.push_back(make_unique<Var>("U"));
//     auto precondition = make_unique<FuncCall>("not_in", move(preArgs));

//     // API Call: signup(u, p)
//     vector<unique_ptr<Expr>> apiArgs;
//     apiArgs.push_back(make_unique<Var>("u"));
//     apiArgs.push_back(make_unique<Var>("p"));
//     auto apiCallFunc = make_unique<FuncCall>("signup", move(apiArgs));

//     // Postcondition: mapping(u, p, U)
//     vector<unique_ptr<Expr>> postArgs;
//     postArgs.push_back(make_unique<Var>("u"));
//     postArgs.push_back(make_unique<Var>("p"));
//     postArgs.push_back(make_unique<Var>("U"));
//     auto postcondition = make_unique<FuncCall>("mapping", move(postArgs));

//     // Create the Response object for the postcondition
//     Response response(HTTPResponseCode::CREATED_201, move(postcondition));

//     // Create the API Call node
//     auto apiCall = make_unique<APIcall>(std::move(apiCallFunc), std::move(response));

//     // Create the API Block
//     auto apiBlock = make_unique<API>(
//         std::move(precondition), 
//         std::move(apiCall),
//         std::move(response)
//     );

//     // Build the Spec
//     vector<unique_ptr<Decl>> globals;

//     auto domain = std::make_unique<TypeConst>("string");
//     auto range = std::make_unique<TypeConst>("string");
    
//     auto mapType = std::make_unique<MapType>(std::move(domain), std::move(range));
    
//     // Create a unique_ptr<Decl> and store it in globals
//     globals.push_back(std::make_unique<Decl>("U", std::move(mapType)));
    

//     vector<unique_ptr<Init>> inits;
   
//     std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>> mapEntries;

//     auto MapExpr = std::make_unique<Map>(std::move(mapEntries));

//     inits.push_back(std::make_unique<Init>("U", std::move(MapExpr)));


//     vector<unique_ptr<FuncDecl>> functions;

//     auto usernameType = std::make_unique<TypeConst>("string");
//     auto passwordType = std::make_unique<TypeConst>("string");
    
//     std::vector<std::unique_ptr<TypeExpr>> params;
//     params.push_back(std::move(usernameType));
//     params.push_back(std::move(passwordType));
    
//     // Step 2: Define return type (HTTP CREATED_201 + string response)
//     auto returnType = std::make_unique<TypeConst>("string");
//     std::vector<std::unique_ptr<TypeExpr>> returnTypes;
//     returnTypes.push_back(std::move(returnType));
    
//     // Step 3: Create the FuncDecl for signup as a unique_ptr
//     auto signup = std::make_unique<FuncDecl>(
//         "signup",
//         std::move(params),
//         std::make_pair(HTTPResponseCode::CREATED_201, std::move(returnTypes))
//     );
    
//     // Step 4: Add to the vector
//     functions.push_back(std::move(signup));
    

//     vector<unique_ptr<API>> apiBlocks;
//     apiBlocks.push_back(std::move(apiBlock));

//     Spec spec(
//         move(globals), 
//         move(inits), 
//         move(functions), 
//         move(apiBlocks)
//     );

//     // -------------------------------
//     // 3. Run the IMA algorithm.
//     // -------------------------------
//     Program transformed = IMA(clientProgram, spec);

//     // -------------------------------
//     // 4. Print the transformed program.
//     // -------------------------------
//     jsCodeGen printer;
//     transformed.accept(printer);


//     return 0;
// }

#include <iostream>
#include <memory>
#include <vector>
#include "ast.hpp"
#include "IMA.hpp"
#include "PrintVisitor.hpp"
#include "jsCodeGen.hpp"
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
        // Declare variable: string username;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto userDecl = make_unique<Decl>("username2", stringType->clone());
        decls.push_back(move(userDecl));
    }

    // Declare variable: string password;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto passDecl = make_unique<Decl>("password2", stringType->clone());
        decls.push_back(move(passDecl));
    }
        // Declare variable: string username;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto userDecl = make_unique<Decl>("username3", stringType->clone());
        decls.push_back(move(userDecl));
    }

    // Declare variable: string password;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto passDecl = make_unique<Decl>("password3", stringType->clone());
        decls.push_back(move(passDecl));
    }
        // Declare variable: string username;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto userDecl = make_unique<Decl>("username4", stringType->clone());
        decls.push_back(move(userDecl));
    }

    // Declare variable: string password;
    {
        auto stringType = make_unique<TypeConst>("string");
        auto passDecl = make_unique<Decl>("password4", stringType->clone());
        decls.push_back(move(passDecl));
    }

    // --- Signup Phase ---
    {
        auto lhs = make_unique<Var>("username");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("username"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Read password for signup attempt (first attempt, will fail)
    {
        auto lhs = make_unique<Var>("password");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("password"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }

    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        args.push_back(make_unique<Var>("password"));
        auto apicall = make_unique<FuncCall>("signup", move(args));
        auto apistmt = make_unique<FuncCallStmt>(move(apicall));
        stmts.push_back(move(apistmt));
    }
    // Re-read username for successful signup
    {
        auto lhs = make_unique<Var>("username2");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("username2"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Re-read password for successful signup
    {
        auto lhs = make_unique<Var>("password2");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("password2"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call signup_success(username, password) → signup success path
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
    // Re-read password for login failure attempt
    {
        auto lhs = make_unique<Var>("password3");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("password3"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call login_notok(username, password) → login failure path
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username3"));
        args.push_back(make_unique<Var>("password3"));
        auto apicall = make_unique<FuncCall>("login", move(args));
        auto apistmt = make_unique<FuncCallStmt>(move(apicall));
        stmts.push_back(move(apistmt));
    }
    // Re-read username for login success attempt
    {
        auto lhs = make_unique<Var>("username4");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("username4"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Re-read password for login success attempt
    {
        auto lhs = make_unique<Var>("password4");
        vector<unique_ptr<Expr>> inputArgs;
        inputArgs.push_back(make_unique<Var>("password4"));
        auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
        auto assignStmt = make_unique<Assign>(move(lhs), move(inputCall));
        stmts.push_back(move(assignStmt));
    }
    // Call login_success(username, password) → login success path
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
    // 2. Build the API specification AST.
    // -------------------------------
    vector<unique_ptr<API>> apiBlocks;

    // --- Signup Notok API ---
    {
        // Precondition: not_in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        auto precondition = make_unique<FuncCall>("_in", move(preArgs));

        // API Call: signup_notok(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("signup", move(apiArgs));

        // Postcondition: not_in( U[u], p ) i.e. user is not added to U.
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("U"));
        // postArgs1.push_back(make_unique<FuncCall>(" ", move(e2))); // wrapper for U
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // represents U[u]
        postArgs.push_back(make_unique<Var>("p"));
        auto postcondition = make_unique<FuncCall>("not_in", move(postArgs));
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
        auto apiCallFunc = make_unique<FuncCall>("signup", move(apiArgs));

        // Postcondition: U[u] = p (User added to U)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("U"));
        // postArgs1.push_back(make_unique<FuncCall>(" ", move(e2))); // wrapper for U
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // represents U[u]
        postArgs.push_back(make_unique<Var>("p"));
        auto postcondition = make_unique<FuncCall>("_in", move(postArgs));
        Response response(HTTPResponseCode::CREATED_201, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // --- Login Notok API ---
    {
        // Precondition: in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        auto precondition = make_unique<FuncCall>("_in", move(preArgs));

        // API Call: login_notok(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("login", move(apiArgs));

        // Postcondition: not_in( T[u], token ) i.e. no token is assigned.
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("T"));
        // postArgs1.push_back(make_unique<FuncCall>(" ", move(e2))); // wrapper for T
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // represents T[u]
        postArgs.push_back(make_unique<Var>("token"));
        auto postcondition = make_unique<FuncCall>("not_in", move(postArgs));
        Response response(HTTPResponseCode::BAD_REQUEST_400, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // --- Login Success API ---
    {
        // Precondition: in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        preArgs.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h;
        h.push_back(make_unique<Var>("U"));
        preArgs.push_back(make_unique<FuncCall>("dom", move(h)));
        auto precondition = make_unique<FuncCall>("not_in", move(preArgs));

        // API Call: login_success(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("login", move(apiArgs));

        // Postcondition: T[u] = token (Token stored in T)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(make_unique<Var>("T"));
        // postArgs1.push_back(make_unique<FuncCall>(" ", move(e2))); // wrapper for T
        postArgs1.push_back(make_unique<Var>("u"));
        postArgs.push_back(make_unique<FuncCall>("[]", move(postArgs1))); // represents T[u]
        postArgs.push_back(make_unique<Var>("token"));
        auto postcondition = make_unique<FuncCall>("_in", move(postArgs));
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

    globals.push_back(make_unique<Decl>("U", move(mapTypeU)));
    globals.push_back(make_unique<Decl>("T", move(mapTypeT)));

    vector<unique_ptr<Init>> inits;
    inits.push_back(make_unique<Init>("U", make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>())));
    inits.push_back(make_unique<Init>("T", make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>())));

    //     inits.push_back(std::make_unique<Init>("U", std::move(MapExpr)));
    Spec spec(move(globals), move(inits), {}, move(apiBlocks));

    // -------------------------------
    // 4. Run IMA algorithm & Print result.
    // -------------------------------
    Program transformed = IMA(clientProgram, spec);
    jsCodeGen printer;
    transformed.accept(printer);
    return 0;
}