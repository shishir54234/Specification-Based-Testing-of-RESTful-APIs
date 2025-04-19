#include <iostream>
#include <memory>
#include <vector>
#include "../../ast.hpp"
#include "../../IMA.hpp"
#include "../../PrintVisitor.hpp"
#include "../../jsCodeGen.hpp"

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

    // Call getProducts(token)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        auto getProductsCall = make_unique<FuncCall>("getProducts", move(args));
        auto getProductsStmt = make_unique<FuncCallStmt>(move(getProductsCall));
        stmts.push_back(move(getProductsStmt));
    }

    // Call viewProducts(token)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("productID"));
        auto getProductsCall = make_unique<FuncCall>("viewProduct", move(args));
        auto getProductsStmt = make_unique<FuncCallStmt>(move(getProductsCall));
        stmts.push_back(move(getProductsStmt));
    }

    // Call AddTowishlist(u,id)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("productID"));
        args.push_back(make_unique<Var>("username"));
        auto getProductsCall = make_unique<FuncCall>("addToWishlist", move(args));
        auto getProductsStmt = make_unique<FuncCallStmt>(move(getProductsCall));
        stmts.push_back(move(getProductsStmt));
    }

    // Call viewWishlist(u)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>("username"));
        auto getProductsCall = make_unique<FuncCall>("viewWishlist", move(args));
        auto getProductsStmt = make_unique<FuncCallStmt>(move(getProductsCall));
        stmts.push_back(move(getProductsStmt));
    }

    Program clientProgram(move(stmts), move(decls));

    // -------------------------------
    // 2. Build the API specification AST.
    // -------------------------------
    vector<unique_ptr<API>> apiBlocks;

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

    // --- Get Prods API ---
    {
        // Precondition: in(u, dom(U))
        vector<unique_ptr<Expr>> preArgs;
        vector<unique_ptr<Expr>> preArgs0;
        preArgs0.push_back(make_unique<Var>("u"));
        vector<unique_ptr<Expr>> h2;
        h2.push_back(make_unique<Var>("T"));
        h2.push_back(make_unique<Var>("token"));
        preArgs0.push_back(make_unique<FuncCall>("mapped_value", move(h2)));
        auto precondition = make_unique<FuncCall>("equals", move(preArgs0));

        // API Call: login_success(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("getProducts", move(apiArgs));

        // Postcondition: T[u] = token (Token stored in T)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        // vector<unique_ptr<Expr>> e2;
        // // e2.push_back(make_unique<Var>("T"));
        // postArgs1.push_back(make_unique<Var>("T")); // wrapper for T
        // postArgs1.push_back(make_unique<Var>("token"));
        // postArgs.push_back(make_unique<FuncCall>("mapped_value", move(postArgs1))); // represents T[u]
        // postArgs.push_back(make_unique<Var>("u"));
        auto postcondition = make_unique<FuncCall>("true", move(postArgs));

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }
    // --- View Prod API ---

    {
        // Precondition: in(u, dom(U))
        // equals(u, mapped_value(T, token))
        vector<unique_ptr<Expr>> eqArgs;
        eqArgs.push_back(make_unique<Var>("u"));

        vector<unique_ptr<Expr>> mappedArgs;
        mappedArgs.push_back(make_unique<Var>("T"));
        mappedArgs.push_back(make_unique<Var>("token"));
        eqArgs.push_back(make_unique<FuncCall>("mapped_value", move(mappedArgs)));

        auto equalsExpr = make_unique<FuncCall>("equals", move(eqArgs));

        // in(prodId, dom(ProductIdMap))
        vector<unique_ptr<Expr>> domArgs;
        domArgs.push_back(make_unique<Var>("ProductIdMap"));
        auto domCall = make_unique<FuncCall>("dom", move(domArgs));

        vector<unique_ptr<Expr>> inArgs;
        inArgs.push_back(make_unique<Var>("prodId"));
        inArgs.push_back(move(domCall));
        auto inExpr = make_unique<FuncCall>("in", move(inArgs));

        // Combine with and_operator
        vector<unique_ptr<Expr>> preconditions;
        preconditions.push_back(move(equalsExpr));
        preconditions.push_back(move(inExpr));

        auto precondition = make_unique<FuncCall>("and_operator", move(preconditions));

        // auto precondition = make_unique<FuncCall>("equals", move(preArgs0));

        // API Call: login_success(u, p)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("prodId"));
        // apiArgs.push_back(make_unique<Var>("p"));
        auto apiCallFunc = make_unique<FuncCall>("viewProduct", move(apiArgs));

        // Postcondition: T[u] = token (Token stored in T)
        vector<unique_ptr<Expr>> postArgs, postArgs1;
        // vector<unique_ptr<Expr>> e2;
        // // e2.push_back(make_unique<Var>("T"));
        // postArgs1.push_back(make_unique<Var>("T")); // wrapper for T
        // postArgs1.push_back(make_unique<Var>("token"));
        // postArgs.push_back(make_unique<FuncCall>("mapped_value", move(postArgs1))); // represents T[u]
        // postArgs.push_back(make_unique<Var>("u"));
        auto postcondition = make_unique<FuncCall>("true", move(postArgs));

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // Add to wishlist
    {
        // === PRECONDITION ===
        // equals(u, mapped_value(T, token))
        vector<unique_ptr<Expr>> eqArgs;
        eqArgs.push_back(make_unique<Var>("u"));

        vector<unique_ptr<Expr>> mappedArgs;
        mappedArgs.push_back(make_unique<Var>("T"));
        mappedArgs.push_back(make_unique<Var>("token"));
        eqArgs.push_back(make_unique<FuncCall>("mapped_value", move(mappedArgs)));

        auto equalsExpr = make_unique<FuncCall>("equals", move(eqArgs));

        // in(prodId, dom(ProductIdMap))
        vector<unique_ptr<Expr>> domArgs;
        domArgs.push_back(make_unique<Var>("ProductIdMap"));
        auto domCall = make_unique<FuncCall>("dom", move(domArgs));

        vector<unique_ptr<Expr>> inArgs;
        inArgs.push_back(make_unique<Var>("prodId"));
        inArgs.push_back(move(domCall));
        auto inExpr = make_unique<FuncCall>("in", move(inArgs));

        // Combine both with and_operator
        vector<unique_ptr<Expr>> preconditions;
        preconditions.push_back(move(equalsExpr));
        preconditions.push_back(move(inExpr));
        auto precondition = make_unique<FuncCall>("and_operator", move(preconditions));

        // === API CALL ===
        // addToWishlist(prodId, u)
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("prodId"));
        apiArgs.push_back(make_unique<Var>("u"));
        auto apiCallFunc = make_unique<FuncCall>("addToWishlist", move(apiArgs));

        // === POSTCONDITION ===
        // in(prodId, dom(Wishlist[u]))
        vector<unique_ptr<Expr>> wishlistIndexArgs;
        wishlistIndexArgs.push_back(make_unique<Var>("Wishlist"));
        wishlistIndexArgs.push_back(make_unique<Var>("u"));
        auto wishlistAccess = make_unique<FuncCall>("getMapAtMatch", move(wishlistIndexArgs));

        vector<unique_ptr<Expr>> postInArgs;
        postInArgs.push_back(make_unique<Var>("prodId"));
        vector<unique_ptr<Expr>> tmp;
        tmp.push_back(move(wishlistAccess));
        postInArgs.push_back(make_unique<FuncCall>("dom", move(tmp)));
        auto postcondition = make_unique<FuncCall>("in", move(postInArgs));

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // --- View Wishlist API ---
    {
        // === PRECONDITION ===
        // equals(u, mapped_value(T, token))
        vector<unique_ptr<Expr>> eqArgs;
        eqArgs.push_back(make_unique<Var>("u"));

        vector<unique_ptr<Expr>> mappedArgs;
        mappedArgs.push_back(make_unique<Var>("T"));
        mappedArgs.push_back(make_unique<Var>("token"));
        eqArgs.push_back(make_unique<FuncCall>("mapped_value", move(mappedArgs)));

        auto precondition = make_unique<FuncCall>("equals", move(eqArgs));

        // === API CALL ===
        vector<unique_ptr<Expr>> apiArgs;
        apiArgs.push_back(make_unique<Var>("u"));
        auto apiCallFunc = make_unique<FuncCall>("viewWishlist", move(apiArgs));

        // === POSTCONDITION ===
        vector<unique_ptr<Expr>> postTrueArgs;
        vector<unique_ptr<Expr>> innerElems;
        innerElems.push_back(make_unique<String>("prodID"));
        innerElems.push_back(make_unique<String>("prodAtrib"));
        auto innerTuple = make_unique<Tuple>(std::move(innerElems));

        // 2) Build the outer tuple: tuple( innerTuple )
        vector<unique_ptr<Expr>> outerElems;
        outerElems.push_back(std::move(innerTuple));
        auto nestedTuple = make_unique<Tuple>(move(outerElems));

        // 3) If you need to push it into a vector somewhere:
        vector<unique_ptr<Expr>> someList;
        someList.push_back(std::move(nestedTuple));
        auto postcondition = make_unique<FuncCall>("true", move(someList));

        Response response(HTTPResponseCode::OK_200, move(postcondition));
        auto apiCall = make_unique<APIcall>(move(apiCallFunc), move(response));
        apiBlocks.push_back(make_unique<API>(move(precondition), move(apiCall), move(response)));
    }

    // -------------------------------
    // 3. Define Global Maps
    // -------------------------------
    vector<unique_ptr<Decl>> globals;

    // U: username → password
    globals.push_back(make_unique<Decl>(
        "U",
        make_unique<MapType>(
            make_unique<TypeConst>("string"),
            make_unique<TypeConst>("string"))));

    // T: username → token
    globals.push_back(make_unique<Decl>(
        "T",
        make_unique<MapType>(
            make_unique<TypeConst>("string"),
            make_unique<TypeConst>("string"))));

    // ProductIdMap: productId → ProductObject
    // where ProductObject is Tuple(name:string, description:string, price:int, sellerId:string, category:string)
    vector<unique_ptr<TypeExpr>> prodFields;
    prodFields.push_back(make_unique<TypeConst>("string")); // name
    prodFields.push_back(make_unique<TypeConst>("string")); // description
    prodFields.push_back(make_unique<TypeConst>("int"));    // price
    prodFields.push_back(make_unique<TypeConst>("string")); // sellerId
    prodFields.push_back(make_unique<TypeConst>("string")); // category
    auto tupleTypeProd = make_unique<TupleType>(move(prodFields));

    globals.push_back(make_unique<Decl>(
        "ProductIdMap",
        make_unique<MapType>(
            make_unique<TypeConst>("string"), // productId
            move(tupleTypeProd)               // ProductObject tuple type
            )));

    // --- NEW: Wishlist: username → Tuple(productId1, productId2, …) ---
    {
        vector<unique_ptr<TypeExpr>> tupleElems;
        tupleElems.push_back(make_unique<TypeConst>("string"));
        auto tupleType = make_unique<TupleType>(std::move(tupleElems));
        auto mapType = make_unique<MapType>(
            make_unique<TypeConst>("string"),
            std::move(tupleType));
        globals.push_back(make_unique<Decl>("Wishlist", std::move(mapType)));
    }

    // Now initialize them:
    vector<unique_ptr<Init>> inits;

    // U entries
    vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> uEntries;
    uEntries.emplace_back(make_unique<Var>("alice"), make_unique<String>("alicepass"));
    uEntries.emplace_back(make_unique<Var>("bob"), make_unique<String>("bob123"));
    uEntries.emplace_back(make_unique<Var>("charlie"), make_unique<String>("qwerty"));
    inits.push_back(make_unique<Init>("U", make_unique<Map>(move(uEntries))));

    // T empty
    inits.push_back(make_unique<Init>("T", make_unique<Map>(
                                               vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>{})));

    // ProductIdMap entries (dummy products)
    vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> prodEntries;
    {
        // Product 1
        vector<unique_ptr<Expr>> attrs1;
        attrs1.push_back(make_unique<String>("Running Shoe"));       // name
        attrs1.push_back(make_unique<String>("Lightweight runner")); // description
        attrs1.push_back(make_unique<Num>(120));                     // price
        attrs1.push_back(make_unique<String>("seller123"));          // sellerId
        attrs1.push_back(make_unique<String>("Footwear"));           // category
        auto prod1 = make_unique<Tuple>(move(attrs1));
        prodEntries.emplace_back(make_unique<Var>("prod1"), move(prod1));
    }
    {
        // Product 2
        vector<unique_ptr<Expr>> attrs2;
        attrs2.push_back(make_unique<String>("Leather Wallet"));
        attrs2.push_back(make_unique<String>("Genuine leather"));
        attrs2.push_back(make_unique<Num>(45));
        attrs2.push_back(make_unique<String>("seller456"));
        attrs2.push_back(make_unique<String>("Accessories"));
        auto prod2 = make_unique<Tuple>(move(attrs2));
        prodEntries.emplace_back(make_unique<Var>("prod2"), move(prod2));
    }
    inits.push_back(make_unique<Init>("ProductIdMap", make_unique<Map>(move(prodEntries))));

    // Wishlist init with two users, each having a tuple of product‐IDs:
    {
        vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> wishEntries;

        // alice has prod1 and prod2
        {
            vector<unique_ptr<Expr>> items;
            items.push_back(make_unique<String>("prod1"));
            items.push_back(make_unique<String>("prod2"));
            auto tup = make_unique<Tuple>(move(items));
            wishEntries.emplace_back(make_unique<Var>("alice"), move(tup));
        }

        // bob has prod3
        {
            vector<unique_ptr<Expr>> items;
            items.push_back(make_unique<String>("prod3"));
            auto tup = make_unique<Tuple>(move(items));
            wishEntries.emplace_back(make_unique<Var>("bob"), move(tup));
        }

        inits.push_back(make_unique<Init>("Wishlist", make_unique<Map>(move(wishEntries))));
    }

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