
#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"
class heavyexample4
{
    public:
        static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
        {
            // ### 7. Save Student Details
            // --------------------------------SAVESTUDENT_OK--------------------------------
            // PRECONDITION: (t in T and T[t].id in U) 
            // saveStudent(t : Token, studentData : Student) ==> (OK)
            // POSTCONDITION: S' = S[studentData.id -> studentData]
            // -----------------------------------------------------------------------------

            unique_ptr<Expr> pre_args;
            vector<unique_ptr<Expr>> args_in1params;
            vector<unique_ptr<Expr>> and1params, and2Params, and3Params;
            vector<unique_ptr<Expr>> orParams1;
            vector<unique_ptr<Expr>> accessMap1params, accessMap2params, accessMap3params;
            vector<unique_ptr<Expr>> getId1params, getId2params, getId3Params;
            vector<unique_ptr<Expr>> args_in2params;
            vector<unique_ptr<Expr>> in3Params, in4Params;
            unique_ptr<Expr> mapAcess1;
            unique_ptr<Expr> mapAcess2, mapAccess3;
            unique_ptr<Expr> getId1, getId2;
            unique_ptr<Expr> equals1, equals2;
            unique_ptr<Expr> in1, in2, in3, in4;
            unique_ptr<Expr> and1, and2, and3;
            unique_ptr<Expr> or1;
            vector<unique_ptr<Expr>> equalsParams1, equalsParams2;
            vector<unique_ptr<Expr>> getRoleParams1;
            unique_ptr<Expr> getRoles;
            // t in T
            args_in1params.push_back(std::make_unique<Var>("T"));
            args_in1params.push_back(std::make_unique<Var>("token"));
            in1 = std::make_unique<FuncCall>("inMapVerify", move(args_in1params));
            // T[t]
            accessMap1params.push_back(std::make_unique<Var>("T"));
            accessMap1params.push_back(std::make_unique<Var>("token"));
            mapAcess1 = std::make_unique<FuncCall>("mapAcess", move(accessMap1params));
            // T[t].id
            getId1params.push_back(std::move(mapAcess1));
            getId1 = std::make_unique<FuncCall>("getId", move(getId1params));
            // T[t].id in U
            args_in2params.push_back(move(getId1));
            args_in2params.push_back(std::make_unique<Var>("U"));
            in2 = std::make_unique<FuncCall>("inMapVerify", move(args_in2params));
            //(t in T and T[t].id in U)
            and1params.push_back(move(in1));
            and1params.push_back(move(in2));
            and1 = std::make_unique<FuncCall>("AND", move(and1params));

            // Pre-Condition
            //  call : saveStudent(t : Token, studentData : Student) ==> (OK)
            vector<unique_ptr<Expr>> call_args1;
            call_args1.push_back(std::make_unique<Var>("token"));
            call_args1.push_back(std::make_unique<Var>("studentData"));
            auto call = std::make_unique<APIcall>(make_unique<FuncCall>("saveStudent", move(call_args1)), Response(HTTPResponseCode::OK_200,std::make_unique<Var>("studentData"))); // to be fixed as student is of type var here
            // POSTCONDITION: S1 = S U {studentData}
            vector<unique_ptr<Expr>> post_args;
            vector<unique_ptr<Expr>> dashParams,unionParams1;
            unique_ptr<Expr> union1;
            dashParams.push_back(std::make_unique<Var>("S"));
            equalsParams2.push_back(std::make_unique<FuncCall>("\'", move(dashParams)));
            unionParams1.push_back(std::make_unique<Var>("S"));
            unionParams1.push_back(std::make_unique<Var>("studentId"));
            union1 = std::make_unique<FuncCall>("Union",move(unionParams1));
            equalsParams2.push_back(move(union1));
            equals2 = std::make_unique<FuncCall>("equals", move(equalsParams2));


            // vector<unique_ptr<Expr>> post_args;
            // equalsParams2.push_back(std::make_unique<Var>("studentList"));
            // equalsParams2.push_back(std::make_unique<Var>("S"));
            // equals2 = std::make_unique<FuncCall>("equals", move(equalsParams2));
            // full api:
            auto api = std::make_unique<API>(std::move(and1), std::move(call), Response(HTTPResponseCode::OK_200, move(equals2)));
            apis.push_back(std::move(api));
            // adding variables to the symbol table
            SymbolTable *sym7 = new SymbolTable();
            sym7->symtable.insert(Var("token"));
            sym7->symtable.insert(Var("studentData"));
            root.children.push_back(sym7);
        }
};
