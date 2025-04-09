#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"

class heavyexample5
{
public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
    {
        // --------------------------------UPDATESTUDENT_OK--------------------------------
        // PRECONDITION: (t in T and T[t].id in U) AND (studentData.id = u.id)
        // updateStudent(t : Token, studentData : Student) ==> (OK)
        // POSTCONDITION: S' = S[studentData.id -> studentData]
        // --------------------------------------------------------------------------------

        // Extract `u` from `T[t]`
        vector<unique_ptr<Expr>> accessMapParams;
        accessMapParams.push_back(std::make_unique<Var>("T"));
        accessMapParams.push_back(std::make_unique<Var>("token"));
        auto mapAccess = std::make_unique<FuncCall>("mapAccess", move(accessMapParams));

        // Get `u.id` from `u`
        vector<unique_ptr<Expr>> getIdParams;
        getIdParams.push_back(std::move(mapAccess));
        auto getId = std::make_unique<FuncCall>("getId", move(getIdParams));

        cout<<"Yo There"<<endl;
        // Check if `u` is in `U`
        vector<unique_ptr<Expr>> inUParams;
        inUParams.push_back(std::move(getId));
        inUParams.push_back(std::make_unique<Var>("U"));
        auto inU = std::make_unique<FuncCall>("inMapVerify", move(inUParams));

        // Ensure studentData.id = u.id
        vector<unique_ptr<Expr>> equalsParams;
        vector<unique_ptr<Expr>> getID1params;
        getID1params.push_back(std::make_unique<Var>("studentData"));
        equalsParams.push_back(std::make_unique<FuncCall>("getId", move(getID1params)));
        vector<unique_ptr<Expr>>getID2params;
        getID2params.push_back(std::make_unique<Var>("T[token]"));
        equalsParams.push_back(std::make_unique<FuncCall>("getId", move(getID2params)));
        auto equals = std::make_unique<FuncCall>("equals", move(equalsParams));

        // Combine preconditions (t -> u in T and u in U) AND (studentData.id = u.id)
        vector<unique_ptr<Expr>> andParams;
        andParams.push_back(std::move(inU));
        andParams.push_back(std::move(equals));
        auto precondition = std::make_unique<FuncCall>("AND", move(andParams));

        // API Call: updateStudent(t : Token, studentData : Student) ==> (OK)
        vector<unique_ptr<Expr>> callArgs;
        callArgs.push_back(std::make_unique<Var>("token"));
        callArgs.push_back(std::make_unique<Var>("studentData"));
        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("updateStudent", move(callArgs)), Response(HTTPResponseCode::OK_200,std::make_unique<Var>("studentData")));

        // Postcondition: S' = S[studentData.id -> studentData]
        vector<unique_ptr<Expr>> postArgs;
        postArgs.push_back(std::make_unique<Var>("S"));
        vector<unique_ptr<Expr>>getID2args;
        getID2args.push_back(std::make_unique<Var>("studentData"));
        postArgs.push_back(std::make_unique<FuncCall>("getId",move(getID2params)));
        postArgs.push_back(std::make_unique<Var>("studentData"));
        auto postcondition = std::make_unique<FuncCall>("mapUpdate", move(postArgs));

        // Full API object
        auto api = std::make_unique<API>(std::move(precondition), std::move(call), Response(HTTPResponseCode::OK_200, move(postcondition)));
        apis.push_back(std::move(api));

        // Add variables to the symbol table
        SymbolTable *symTable = new SymbolTable();
        symTable->symtable.insert(Var("token"));
        symTable->symtable.insert(Var("studentData"));
        root.children.push_back(symTable);
    }
};
