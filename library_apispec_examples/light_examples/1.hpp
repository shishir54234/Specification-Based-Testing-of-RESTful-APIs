#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"
class lightexample1
{
public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
    // register_admin_ok
    // not in (username , dom(U)) ==> username not in dom(U)
    {
        vector<unique_ptr<Expr>> pre_args;
        pre_args.push_back(std::make_unique<Var>("username"));
        vector<unique_ptr<Expr>> h;
        // username not in dom(U)
        h.push_back(std::make_unique<Var>("U"));
        pre_args.push_back(std::make_unique<FuncCall>("dom", move(h)));
        auto pre = std::make_unique<FuncCall>("not in", move(pre_args));
        // register_admin(username,password) ===> (OK)
        vector<unique_ptr<Expr>> call_args1;
        call_args1.push_back(std::make_unique<Var>("username"));
        call_args1.push_back(std::make_unique<Var>("password"));

        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("register_admin", move(call_args1)), Response(HTTPResponseCode::CREATED_201, nullptr));
        // POSTCONDITION: U' = U[username -> password]  ==> U' = U union {username, password} (todo)
        // rn we have written U'[username]=password
        vector<unique_ptr<Expr>> post_args;
        vector<unique_ptr<Expr>> post_args1; // [] (  U, (username,password))
        vector<unique_ptr<Expr>> e2;         // '
        e2.push_back(std::make_unique<Var>("U"));
        post_args1.push_back(std::make_unique<FuncCall>("'", move(e2)));
        post_args1.push_back(std::make_unique<Var>("username"));
        post_args.push_back(make_unique<FuncCall>("[]", move(post_args1)));
        post_args.push_back(std::make_unique<Var>("password"));

        auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::CREATED_201, make_unique<FuncCall>("=", move(post_args))));
        apis.push_back(std::move(api));

        // adding variables to the symbol table
        SymbolTable *sym1 = new SymbolTable();
        sym1->symtable.insert(Var("username"));
        sym1->symtable.insert(Var("password"));
        root.children.push_back(sym1);
    }
};
