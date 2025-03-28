#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"

class lightexample2
{
public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
    // register_admin_ok
    // not in (username , dom(U)) ==> username not in dom(U)
    {
        // register_student_ok
        // not in (email , dom(S)) ==> email not in dom(S)
        vector<unique_ptr<Expr>> pre_args;
        pre_args.push_back(std::make_unique<Var>("email"));
        vector<unique_ptr<Expr>> h;
        h.push_back(std::make_unique<Var>("S"));
        pre_args.push_back(std::make_unique<FuncCall>("dom", move(h)));
        auto pre = std::make_unique<FuncCall>("not in", move(pre_args));

        //  register_student(name : string, email : string, password : string) ==> (OK)
        vector<unique_ptr<Expr>> call_args1;
        call_args1.push_back(std::make_unique<Var>("name"));
        call_args1.push_back(std::make_unique<Var>("email"));
        call_args1.push_back(std::make_unique<Var>("password"));
        auto fun = make_unique<FuncCall>("register_student", move(call_args1));
        auto call = std::make_unique<APIcall>(std::move(fun), Response(HTTPResponseCode::CREATED_201, nullptr));

        // S'[email] = {name,password}
        vector<unique_ptr<Expr>> post_args;
        vector<unique_ptr<Expr>> post_args1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(std::make_unique<Var>("S"));
        post_args1.push_back(std::make_unique<FuncCall>("'", move(e2)));
        post_args1.push_back(std::make_unique<Var>("email"));
        post_args.push_back(make_unique<FuncCall>("[]", move(post_args1)));

        vector<unique_ptr<Expr>> record_fields;
        record_fields.push_back(std::make_unique<Var>("name"));
        record_fields.push_back(std::make_unique<Var>("password"));
        post_args.push_back(std::make_unique<Tuple>(move(record_fields)));

        auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::CREATED_201, make_unique<FuncCall>("=", move(post_args))));
        apis.push_back(std::move(api));

        // adding variables to the symbol table
        SymbolTable *sym2 = new SymbolTable();
        sym2->symtable.insert(Var("email"));
        sym2->symtable.insert(Var("name"));
        sym2->symtable.insert(Var("password"));
        root.children.push_back(sym2);
    }
};