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

    tupletypeargs.push_back(std::make_unique<TypeConst>("string"));
    tupletypeargs.push_back(std::make_unique<TypeConst>("string"));// {, std::make_unique<TypeConst>("string")};

    // U : (string -> string) map // Users
    globals.push_back(std::make_unique<Decl>("U", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
    // S : (string -> {string, string}) map // Students
    globals.push_back(std::make_unique<Decl>("S", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TupleType>(std::move(tupletypeargs)))));
    // T : (string -> string)
    globals.push_back(std::make_unique<Decl>("T", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
    // B:(string->Book) How do we write this? Maybe we need custom types? Or do we simplify the Book into a tuple type ?
    // R : Request Set // Requests
    //BS:BookStudent Set // Book-Student Records

    // U = {}
    vector<unique_ptr<Init>> inits;
    unique_ptr<Expr> map1 = std::make_unique<Map>(vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>>());
    unique_ptr<Init> init1 = std::make_unique<Init>("U", std::move(map1));


    vector<unique_ptr<FuncDecl>> funcDecls;
    std::vector<std::unique_ptr<TypeExpr>> params;
    // register_admin (string x string) ==> (HTTPResponseCode, TypeExpr)
    params.push_back(std::make_unique<TypeConst>("string"));
    params.push_back(std::make_unique<TypeConst>("string"));
    vector<unique_ptr<TypeExpr>> resp;
    auto signup = make_unique<FuncDecl>("register_admin", move(params), std::make_pair(HTTPResponseCode::CREATED_201, move(resp)));
    funcDecls.push_back(std::move(signup));


    std::vector<std::unique_ptr<TypeExpr>> params1;
    params1.push_back(std::make_unique<TypeConst>("string"));
    params1.push_back(std::make_unique<TypeConst>("string"));
    vector<unique_ptr<TypeExpr>> resp1;
    resp1.push_back(std::make_unique<TypeConst>("string"));
    auto login = make_unique<FuncDecl>("login", move(params1), std::make_pair(HTTPResponseCode::CREATED_201, move(resp1)));
    funcDecls.push_back(std::move(login));

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
            vector<unique_ptr<Expr>> e2; // '
            e2.push_back(std::make_unique<Var>("U"));
            post_args1.push_back(std::make_unique<FuncCall>("'", move(e2)));
            post_args1.push_back(std::make_unique<Var>("username"));
            post_args.push_back(make_unique<FuncCall>("[]", move(post_args1)));
            post_args.push_back(std::make_unique<Var>("password"));
            
            auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::CREATED_201, make_unique<FuncCall>("=", move(post_args))));
            apis.push_back(std::move(api));
            
            // adding variables to the symbol table 
            SymbolTable sym1;
            sym1.symtable.insert(Var("username"));
            sym1.symtable.insert(Var("password"));
            symtable.children.push_back(&sym1);
        }
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
         auto fun=make_unique<FuncCall>("register_student", move(call_args1));
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
         SymbolTable sym2;
         sym2.symtable.insert(Var("email"));
         sym2.symtable.insert(Var("name"));
         sym2.symtable.insert(Var("password"));
         symtable.children.push_back(&sym2);

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
         auto call = std::make_unique<APIcall>(make_unique<FuncCall>("authenticate", move(call_args1)), Response(HTTPResponseCode::OK_200, nullptr));

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

         // adding variables to the symbol table 
         SymbolTable sym3;
         sym3.symtable.insert(Var("token"));
         sym3.symtable.insert(Var("username"));
         sym3.symtable.insert(Var("password"));
         symtable.children.push_back(&sym3);
     }
     {
         vector<unique_ptr<Expr>> pre_args;
         vector<unique_ptr<Expr>> pre1;

     }
     {
//         ### 5. Get Student by ID
        // --------------------------------GETSTUDENT_OK--------------------------------
        // PRECONDITION: (t in T and T[t].id in U) AND ((ADMIN in T[t].role AND studentId in U) OR studentId = T[t].id)
        // getStudent(t : Token, studentId : string) ==> (OK, studentData: Student)
        // POSTCONDITION: studentId = studentData.id
        // ----------------------------------------------------------------------------- 
       
        unique_ptr<Expr> pre_args;
         vector<unique_ptr<Expr>> args_in1params;
         vector<unique_ptr<Expr>> and1params,and2Params,and3Params;
         vector<unique_ptr<Expr>> orParams1;
         vector<unique_ptr<Expr>> accessMap1params,accessMap2params,accessMap3params;
         vector<unique_ptr<Expr>> getId1params,getId2params,getId3Params;
         vector<unique_ptr<Expr>> args_in2params;
         vector<unique_ptr<Expr>> in3Params,in4Params;
         unique_ptr<Expr> mapAcess1;
         unique_ptr<Expr> mapAcess2,mapAccess3;
         unique_ptr<Expr> getId1,getId2;
         unique_ptr<Expr> equals1,equals2;
         unique_ptr<Expr> in1,in2,in3,in4;
         unique_ptr<Expr> and1,and2,and3;
         unique_ptr<Expr> or1;
         vector<unique_ptr<Expr>> equalsParams1,equalsParams2;
         vector<unique_ptr<Expr>> getRoleParams1;
         unique_ptr<Expr> getRoles;

        //t in T
        args_in1params.push_back(std::make_unique<Var>("T"));
        args_in1params.push_back(std::make_unique<Var>("token"));
        in1 = std::make_unique<FuncCall>("inMapVerify",move(args_in1params));

        //T[t]
        accessMap1params.push_back(std::make_unique<Var>("T"));
        accessMap1params.push_back(std::make_unique<Var>("token"));  
        mapAcess1 = std::make_unique<FuncCall>("mapAcess",move(accessMap1params));

        //T[t].id 
        getId1params.push_back(std::move(mapAcess1)); 
        getId1 = std::make_unique<FuncCall>("getId",move(getId1params));

        //T[t].id in U
        args_in2params.push_back(move(getId1));
        args_in2params.push_back(std::make_unique<Var>("U"));
        in2 = std::make_unique<FuncCall>("inMapVerify",move(args_in2params));

        //(t in T and T[t].id in U)
        and1params.push_back(move(in1));
        and1params.push_back(move(in2));
        and1 = std::make_unique<FuncCall>("inMapVerify",move(and1params));

        //((ADMIN in T[t].role AND studentId in U) OR studentId = T[t].id)

        //T[t]
        accessMap2params.push_back(std::make_unique<Var>("T"));
        accessMap2params.push_back(std::make_unique<Var>("token"));  
        mapAcess2 = std::make_unique<FuncCall>("mapAcess",move(accessMap1params));

        //T[t].id 
        getId2params.push_back(std::move(mapAcess1)); 
        getId2 = std::make_unique<FuncCall>("getId",move(getId1params));

        //studentId = T[t].id
        equalsParams1.push_back(std::move(getId2));
        equalsParams1.push_back(std::make_unique<Var>("studentId"));
        equals1 = std::make_unique<FuncCall>("getId",move(equalsParams1));

        // ##((ADMIN in T[t].role AND studentId in U)
        //T[t]        
        accessMap3params.push_back(std::make_unique<Var>("T"));
        accessMap3params.push_back(std::make_unique<Var>("token"));  
        mapAccess3 = std::make_unique<FuncCall>("mapAcess",move(accessMap1params));

        //T[t].role
        getRoleParams1.push_back(std::move(mapAccess3));
        getRoles = std::make_unique<FuncCall>("getRoles",move(getRoleParams1));

        //ADMIN in T[t].role 
        in3Params.push_back(std::move(getRoles));
        in3Params.push_back(std::make_unique<Var>("ADMIN"));
        in3 = std::make_unique<FuncCall>("inList",move(in3Params));

        //studentId in U
        in4Params.push_back(std::make_unique<Var>("studentId"));
        in4Params.push_back(std::make_unique<Var>("U"));
        in4 = std::make_unique<FuncCall>("inMapVerify",move(in4Params));

        // (ADMIN in T[t].role AND studentId in U)
        and2Params.push_back(std::move(in3));
        and2Params.push_back(std::move(in4));
        and2 = std::make_unique<FuncCall>("inMapVerify",move(and2Params));

        //((ADMIN in T[t].role AND studentId in U) OR studentId = T[t].id)
        orParams1.push_back(std::move(and2));
        orParams1.push_back(std::move(equals1));
        or1 = std::make_unique<FuncCall>("OR",move(orParams1));

        //(t in T and T[t].id in U) AND ((ADMIN in T[t].role AND studentId in U) OR studentId = T[t].id)
        and3Params.push_back(std::move(and1));
        and3Params.push_back(std::move(or1));
        and3 = std::make_unique<FuncCall>("AND",move(and3Params));

        //Pre-Condition
        // pre_args = std::move(and3);


        // call : getStudent(t : Token, studentId : string) ==> (OK, studentData: Student)
        vector<unique_ptr<Expr>> call_args1;
        call_args1.push_back(std::make_unique<Var>("Token"));
        call_args1.push_back(std::make_unique<Var>("studentId"));
        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("getStudent", move(call_args1)), Response(HTTPResponseCode::OK_200, std::make_unique<Var>("studentData"))); // to be fixed as student is of type var here

        // POSTCONDITION: studentId = studentData.id
        vector<unique_ptr<Expr>> post_args;
        getId3Params.push_back(std::make_unique<Var>("studentData"));
        equalsParams2.push_back(std::make_unique<FuncCall>("getId", move(getId3Params)));
        equalsParams2.push_back(std::make_unique<Var>("studentId"));
        equals2 = std::make_unique<FuncCall>("=", move(equalsParams2));

         // full api:
         auto api = std::make_unique<API>(std::move(and3), std::move(call), Response(HTTPResponseCode::OK_200, move(equals2)));
         apis.push_back(std::move(api));

         // adding variables to the symbol table 
         SymbolTable sym4;
         sym4.symtable.insert(Var("token"));
         sym4.symtable.insert(Var("studentId"));
         symtable.children.push_back(&sym4);
     }

    auto spec = make_unique<Spec>(move(globals), move(inits), move(funcDecls), move(apis));

    PrintVisitor visitor;
    spec->accept(visitor);
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

