#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"

class lightexample3
{
    public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
    // login_ok
    {
        // authenticate user ---> rewrote this (login_ok)
        // PRECONDITION: U[username] = password
        vector<unique_ptr<Expr>> pre_args;
        vector<unique_ptr<Expr>> pre1;
        pre1.push_back(std::make_unique<Var>("U"));
        pre1.push_back(std::make_unique<Var>("username"));
        pre_args.push_back(std::make_unique<FuncCall>("[]", move(pre1)));
        pre_args.push_back(std::make_unique<Var>("password"));
        auto pre = std::make_unique<FuncCall>("=", move(pre_args));
        // call : authenticate(username : string, password : string) ==> (OK, token)
        vector<unique_ptr<Expr>> call_args1;
        call_args1.push_back(std::make_unique<Var>("username"));
        call_args1.push_back(std::make_unique<Var>("password"));
        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("authenticate", move(call_args1)), Response(HTTPResponseCode::OK_200, nullptr));
        // POSTCONDITION: T' = T[token -> username]
        vector<unique_ptr<Expr>> post_args;
        vector<unique_ptr<Expr>> pos1;
        pos1.push_back(make_unique<Var>("T"));
        post_args.push_back(make_unique<FuncCall>("'", move(pos1)));
        vector<unique_ptr<Expr>> pos2;
        vector<unique_ptr<Expr>> pos21;
        pos21.push_back(make_unique<Var>("token"));
        pos21.push_back(make_unique<Var>("username"));
        pos2.push_back(make_unique<Var>("T"));
        pos2.push_back(make_unique<Tuple>(move(pos21)));
        post_args.push_back(make_unique<FuncCall>("union", move(pos2)));
        // full thing:
        auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::OK_200, make_unique<FuncCall>("=", move(post_args))));
        apis.push_back(std::move(api));
        // adding variables to the symbol table
        SymbolTable *sym3 = new SymbolTable;
        sym3->symtable.insert(Var("token"));
        sym3->symtable.insert(Var("username"));
        sym3->symtable.insert(Var("password"));
        root.children.push_back(sym3);
    }
};
