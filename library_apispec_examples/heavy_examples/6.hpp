#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"

class heavyexample6
{
public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
    {
        // PRECONDITION: (t -> u in T and userId in U and u in U)
        vector<unique_ptr<Expr>> accessMapParams, getIdParams, andParams1, andParams2;
        vector<unique_ptr<Expr>> inParams1, inParams2;
        unique_ptr<Expr> mapAccess, getId, in1, in2, in3, and1, and2;
        vector<unique_ptr<Expr>> equalsParams;
        unique_ptr<Expr> equals;

        // T[t] = u
        accessMapParams.push_back(std::make_unique<Var>("T"));
        accessMapParams.push_back(std::make_unique<Var>("token"));
        mapAccess = std::make_unique<FuncCall>("mapAccess", move(accessMapParams));

        // u in U
        inParams1.push_back(std::move(mapAccess));
        inParams1.push_back(std::make_unique<Var>("U"));
        in1 = std::make_unique<FuncCall>("inMapVerify", move(inParams1));

        // userId in U
        inParams2.push_back(std::make_unique<Var>("userId"));
        inParams2.push_back(std::make_unique<Var>("U"));
        in2 = std::make_unique<FuncCall>("inMapVerify", move(inParams2));

        // (u in U and userId in U)
        andParams1.push_back(move(in1));
        andParams1.push_back(move(in2));
        and1 = std::make_unique<FuncCall>("AND", move(andParams1));

        // Final precondition: (t -> u in T and userId in U and u in U)
        andParams2.push_back(std::move(and1));
        and2 = std::make_unique<FuncCall>("AND", move(andParams2));

        // API Call: getStudentByUserId(t : Token, userId : string) ==> (OK, studentData: Student)
        vector<unique_ptr<Expr>> callArgs;
        callArgs.push_back(std::make_unique<Var>("token"));
        callArgs.push_back(std::make_unique<Var>("userId"));
        auto call = std::make_unique<APIcall>(
            std::make_unique<FuncCall>("getStudentByUserId", move(callArgs)),
            Response(HTTPResponseCode::OK_200, std::make_unique<Var>("studentData"))
        );

        // POSTCONDITION: studentData.id = userId
        getIdParams.push_back(std::make_unique<Var>("studentData"));
        getId = std::make_unique<FuncCall>("getId", move(getIdParams));
        equalsParams.push_back(std::move(getId));
        equalsParams.push_back(std::make_unique<Var>("userId"));
        equals = std::make_unique<FuncCall>("equals", move(equalsParams));

        // Full API definition
        auto api = std::make_unique<API>(std::move(and2), std::move(call), Response(HTTPResponseCode::OK_200, move(equals)));
        apis.push_back(std::move(api));

        // Adding variables to the symbol table
        SymbolTable *sym4 = new SymbolTable();
        sym4->symtable.insert(Var("token"));
        sym4->symtable.insert(Var("userId"));
        root.children.push_back(sym4);
    }
};
