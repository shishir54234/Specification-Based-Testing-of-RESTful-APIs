#include <iostream>
#include <cassert>
#include <memory>
#include "jsCodeGenerator/jsCodeGen.h"
#include "ast.hpp"
#include "algo.hpp"
using namespace std;

void test_everything(){

    // register admin register student
    std::vector<std::unique_ptr<Decl>> globals;

    std::vector<std::unique_ptr<TypeExpr>> tupletypeargs;

    // tupletypeargs.push_back(std::make_unique<TypeConst>("string"));
    // tupletypeargs.push_back(std::make_unique<TypeConst>("string"));// {, std::make_unique<TypeConst>("string")};

    // U : (string -> string) map // Users
    globals.push_back(std::make_unique<Decl>("U", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
    // S : (string -> {string, string}) map // Students
    globals.push_back(std::make_unique<Decl>("S", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TupleType>(std::move(tupletypeargs)))));
    // T : (string -> string)
    globals.push_back(std::make_unique<Decl>("T", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
    // B:(string->Book) How do we write this? Maybe we need custom types? Or do we simplify the Book into a tuple type ?
    // R : Request Set // Requests
    //BS:BookStudent Set // Book-Student Records

    vector<unique_ptr<Init>> inits;
    unique_ptr<Expr> map1 = std::make_unique<Map>(vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>>());
    unique_ptr<Init> init1 = std::make_unique<Init>("U", std::move(map1));


    vector<unique_ptr<FuncDecl>> funcDecls;
    std::vector<std::unique_ptr<TypeExpr>> params;
    params.push_back(std::make_unique<TypeConst>("string"));
    params.push_back(std::make_unique<TypeConst>("string"));
    vector<unique_ptr<TypeExpr>> resp;
    auto signup = make_unique<FuncDecl>("signup", move(params), std::make_pair(HTTPResponseCode::CREATED_201, move(resp)));
    funcDecls.push_back(std::move(signup));
    std::vector<std::unique_ptr<TypeExpr>> params1;
    params1.push_back(std::make_unique<TypeConst>("string"));
    params1.push_back(std::make_unique<TypeConst>("string"));
    vector<unique_ptr<TypeExpr>> resp1;
    resp1.push_back(std::make_unique<TypeConst>("Token"));
    auto login = make_unique<FuncDecl>("login", move(params1), std::make_pair(HTTPResponseCode::CREATED_201, move(resp1)));
    funcDecls.push_back(std::move(login));

        vector<unique_ptr<API>>
            apis;
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
        apis.push_back(std::move(api));
    }
    {
        vector<unique_ptr<Expr>> pre_args;
        vector<unique_ptr<Expr>> pre1;
        
    }

    auto spec = make_unique<Spec>(move(globals), move(inits), move(funcDecls), move(apis));

    PrintVisitor visitor;
    spec->accept(visitor);
    SymbolTable symtable;
    symtable.symtable.insert(Var("U"));
    SymbolTable sym1;
    sym1.symtable.insert(Var("username"));
    sym1.symtable.insert(Var("password"));
    symtable.children.push_back(&sym1);
    SymbolTable sym2;
    sym2.symtable.insert(Var("username"));
    sym2.symtable.insert(Var("password"));
    symtable.children.push_back(&sym2);
    Program p = convert(spec.get(), symtable);
    // vector<unique_ptr<Stmt>> stmts;
    // for(auto &stmt:p.statements){
    //     stmts.push_back(move(stmt));
    // }
    p.accept(visitor);
    ExpoSECodeGenerator ecg;

    cout<<"generating expoSE code:";
    string code = ecg.generateCode(p);
    cout << code;
}


int main(){
    test_everything();
    return 0;
}

