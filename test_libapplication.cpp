#include <iostream>
#include <cassert>
#include <memory>
#include "jsCodeGen.h"
#include "../ast.hpp"
#include "../algo.hpp"
using namespace std;

void test_everything(){

    // register admin register student
    std::vector<std::unique_ptr<Decl>> globals;
    // U : (string -> string) map // Users
    globals.push_back(std::make_unique<Decl>("U", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
    // S : (string -> {string, string}) map // Students
    globals.push_back(std::make_unique<Decl>("S", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TupleType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string")))));
    // T : (string -> string)
    globals.push_back(std::make_unique<Decl>("T", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
    // B:(string->Book) How do we write this? Maybe we need custom types? Or do we simplify the Book into a tuple type ?
    // R : Request Set // Requests
    //BS:BookStudent Set // Book-Student Records
        vector<unique_ptr<API>>
            apis;
        SymbolTable symtable;
        symtable.symtable.insert(Var("U"));
        symtable.symtable.insert(Var("S"));
        symtable.symtable.insert(Var("T"));
        symtable.symtable.insert(Var("B"));
        symtable.symtable.insert(Var("R"));
        symtable.symtable.insert(Var("BS"));
        {
            // register_admin
            vector<unique_ptr<Expr>> pre_args;
            pre_args.push_back(std::make_unique<Var>("username"));
            vector<unique_ptr<Expr>> h;
            h.push_back(std::make_unique<Var>("U"));
            pre_args.push_back(std::make_unique<FuncCall>("dom", move(h)));
            auto pre = std::make_unique<FuncCall>("not in", move(pre_args));

            vector<unique_ptr<Expr>> call_args1;
            call_args1.push_back(std::make_unique<Var>("username"));
            call_args1.push_back(std::make_unique<Var>("password"));

            auto call = std::make_unique<APIcall>(make_unique<FuncCall>("register_admin", move(call_args1)), Response(HTTPResponseCode::CREATED_201, nullptr));

            vector<unique_ptr<Expr>> post_args;
            vector<unique_ptr<Expr>> post_args1;
            vector<unique_ptr<Expr>> e2;
            e2.push_back(std::make_unique<Var>("U"));
            post_args1.push_back(std::make_unique<FuncCall>("'", move(e2)));
            post_args1.push_back(std::make_unique<Var>("username"));
            post_args.push_back(make_unique<FuncCall>("[]", move(post_args1)));
            post_args.push_back(std::make_unique<Var>("password"));

            auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::CREATED_201, make_unique<FuncCall>("=", move(post_args))));
            apis.push_back(std::move(api));
    }
    {
        // register_student_ok
        // not in (email , S)
        vector<unique_ptr<Expr>> pre_args;
        pre_args.push_back(std::make_unique<Var>("email"));
        vector<unique_ptr<Expr>> h;
        h.push_back(std::make_unique<Var>("S"));
        pre_args.push_back(std::make_unique<FuncCall>("dom", move(h)));
        auto pre = std::make_unique<FuncCall>("not in", move(pre_args));

        vector<unique_ptr<Expr>> call_args1;
        call_args1.push_back(std::make_unique<Var>("name"));
        call_args1.push_back(std::make_unique<Var>("email"));
        call_args1.push_back(std::make_unique<Var>("password"));

        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("register_student", move(call_args1)), Response(HTTPResponseCode::CREATED_201, nullptr));
        
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
    }
    {
        // authenticate user ---> rewrote this (login_ok)
        // PRECONDITION: U[username] = password
        vector<unique_ptr<Expr>> pre_args;
        vector<unique_ptr<Expr>> pre1;
        pre1.push_back(std::make_unique<Var>("U"));
        pre1.push_back(std::make_unique<Var>("username"));
        pre_args.push_back(std::make_unique<FuncCall>("[]",move(pre1)));
        pre_args.push_back(std::make_unique<Var>("password"));  
        auto pre = std::make_unique<FuncCall>("=", move(pre_args));

        // call : authenticate(username : string, password : string) ==> (OK, token)
        vector<unique_ptr<Expr>> call_args1;
        call_args1.push_back(std::make_unique<Var>("username"));
        call_args1.push_back(std::make_unique<Var>("password"));
        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("register_student", move(call_args1)), Response(HTTPResponseCode::OK_200, nullptr));

        // POSTCONDITION: T' = T[token -> username]
        vector<unique_ptr<Expr>> post_args;
        vector<unique_ptr<Expr>> pos1;
        pos1.push_back(make_unique<Var>("T"));
        post_args.push_back(make_unique<FuncCall>("'",move(pos1)));
        vector<unique_ptr<Expr>> pos2;
        vector<unique_ptr<Expr>> pos21;
        pos21.push_back(make_unique<Var>("token"));
        pos21.push_back(make_unique<Var>("username"));
        pos2.push_back(make_unique<Var>("T"));
        pos2.push_back(make_unique<Tuple>(move(pos21)));
        post_args.push_back(make_unique<FuncCall>("union", move(pos2)));


        // full thing:
        auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::OK_200, make_unique<FuncCall>("=", move(post_args))));
    }

}

