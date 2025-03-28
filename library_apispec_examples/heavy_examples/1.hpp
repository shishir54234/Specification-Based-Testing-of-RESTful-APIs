
#pragma once


class heavyexample1
{
    public:
        static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
        // register_admin_ok
        // not in (username , dom(U)) ==> username not in dom(U)
        {
            //   ### 5. Get Student by ID
            // --------------------------------GETSTUDENT_OK--------------------------------
            // PRECONDITION: (t in T and T[t].id in U) AND ((ADMIN in T[t].role AND studentId in U) OR studentId = T[t].id)
            // getStudent(t : Token, studentId : string) ==> (OK, studentData: Student)
            // POSTCONDITION: studentId = studentData.id
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
            and1 = std::make_unique<FuncCall>("inMapVerify", move(and1params));
            //((ADMIN in T[t].role AND studentId in U) OR studentId = T[t].id)
            // T[t]
            accessMap2params.push_back(std::make_unique<Var>("T"));
            accessMap2params.push_back(std::make_unique<Var>("token"));
            mapAcess2 = std::make_unique<FuncCall>("mapAcess", move(accessMap1params));
            // T[t].id
            getId2params.push_back(std::move(mapAcess1));
            getId2 = std::make_unique<FuncCall>("getId", move(getId1params));
            // studentId = T[t].id
            equalsParams1.push_back(std::move(getId2));
            equalsParams1.push_back(std::make_unique<Var>("studentId"));
            equals1 = std::make_unique<FuncCall>("getId", move(equalsParams1));
            // ##((ADMIN in T[t].role AND studentId in U)
            // T[t]
            accessMap3params.push_back(std::make_unique<Var>("T"));
            accessMap3params.push_back(std::make_unique<Var>("token"));
            mapAccess3 = std::make_unique<FuncCall>("mapAcess", move(accessMap1params));
            // T[t].role
            getRoleParams1.push_back(std::move(mapAccess3));
            getRoles = std::make_unique<FuncCall>("getRoles", move(getRoleParams1));
            // ADMIN in T[t].role
            in3Params.push_back(std::move(getRoles));
            in3Params.push_back(std::make_unique<Var>("ADMIN"));
            in3 = std::make_unique<FuncCall>("inList", move(in3Params));
            // studentId in U
            in4Params.push_back(std::make_unique<Var>("studentId"));
            in4Params.push_back(std::make_unique<Var>("U"));
            in4 = std::make_unique<FuncCall>("inMapVerify", move(in4Params));
            // (ADMIN in T[t].role AND studentId in U)
            and2Params.push_back(std::move(in3));
            and2Params.push_back(std::move(in4));
            and2 = std::make_unique<FuncCall>("inMapVerify", move(and2Params));
            //((ADMIN in T[t].role AND studentId in U) OR studentId = T[t].id)
            orParams1.push_back(std::move(and2));
            orParams1.push_back(std::move(equals1));
            or1 = std::make_unique<FuncCall>("OR", move(orParams1));
            //(t in T and T[t].id in U) AND ((ADMIN in T[t].role AND studentId in U) OR studentId = T[t].id)
            and3Params.push_back(std::move(and1));
            and3Params.push_back(std::move(or1));
            and3 = std::make_unique<FuncCall>("AND", move(and3Params));
            // Pre-Condition
            //  pre_args = std::move(and3);
            //  call : getStudent(t : Token, studentId : string) ==> (OK, studentData: Student)
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
            SymbolTable *sym4 = new SymbolTable();
            sym4->symtable.insert(Var("token"));
            sym4->symtable.insert(Var("studentId"));
            root.children.push_back(sym4);
        }
};
