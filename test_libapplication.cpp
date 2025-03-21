#include <iostream>
#include <cassert>
#include <memory>
#include "jsCodeGen.h"
#include "../ast.hpp"
#include "../algo.hpp"
using namespace std;

void test_everything(){
    std::vector<std::unique_ptr<Decl>> globals;
    globals.push_back(std::make_unique<Decl>("U", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
    globals.push_back(std::make_unique<Decl>("S", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<RecordType>(std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>>{
        {"name", std::make_unique<TypeConst>("string")},
        {"password", std::make_unique<TypeConst>("string")}
    }))));

    vector<unique_ptr<API>> apis;
    {
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
        vector<unique_ptr<Expr>> pre_args;
        vector<unique_ptr<Expr>> t_args;
        t_args.push_back(std::make_unique<Var>("t"));
        auto token_check = std::make_unique<FuncCall>("->", move(t_args));
        vector<unique_ptr<Expr>> u_args;
        u_args.push_back(std::make_unique<Var>("U"));
        auto user_check = std::make_unique<FuncCall>("in", move(u_args));
        vector<unique_ptr<Expr>> role_check_args;
        role_check_args.push_back(std::make_unique<Var>("ADMIN"));
        role_check_args.push_back(std::make_unique<FuncCall>("role", {}));
        auto role_check = std::make_unique<FuncCall>("in", move(role_check_args));
        vector<unique_ptr<Expr>> id_check_args;
        id_check_args.push_back(std::make_unique<Var>("studentId"));
        id_check_args.push_back(std::make_unique<FuncCall>("id", {}));
        auto id_check = std::make_unique<FuncCall>("=", move(id_check_args));
        pre_args.push_back(std::make_unique<FuncCall>("AND", move(pre_args)));
        auto pre = std::make_unique<FuncCall>("AND", move(pre_args));
        
        vector<unique_ptr<Expr>> call_args;
        call_args.push_back(std::make_unique<Var>("t"));
        call_args.push_back(std::make_unique<Var>("studentId"));
        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("getStudent", move(call_args)), Response(HTTPResponseCode::OK_200, nullptr));
        
        vector<unique_ptr<Expr>> post_args;
        post_args.push_back(std::make_unique<Var>("studentId"));
        post_args.push_back(std::make_unique<FuncCall>("id", {}));
        auto post = make_unique<FuncCall>("=", move(post_args));
        
        auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::OK_200, std::move(post)));
        apis.push_back(std::move(api));
    }

}

