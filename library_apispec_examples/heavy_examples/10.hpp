#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"

class heavyexample10 {
public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root) {
        unique_ptr<Expr> pre_condition, post_condition;
        vector<unique_ptr<Expr>> andParams, inParams1, inParams2, roleParams;
        vector<unique_ptr<Expr>> mapAccessParams, getRoleParams, deleteParams;
        unique_ptr<Expr> mapAccess, getRole, in1, in2, adminCheck, andExpr;

        // Checking (t -> u in T)
        mapAccessParams.push_back(std::make_unique<Var>("T"));
        mapAccessParams.push_back(std::make_unique<Var>("token"));
        mapAccess = std::make_unique<FuncCall>("mapAcess", move(mapAccessParams));
        
        inParams1.push_back(std::move(mapAccess));
        inParams1.push_back(std::make_unique<Var>("U"));
        in1 = std::make_unique<FuncCall>("inMapVerify", move(inParams1));

        // Checking (ADMIN in u.role)
        getRoleParams.push_back(std::make_unique<Var>("T[token]"));
        getRole = std::make_unique<FuncCall>("getRoles", move(getRoleParams));
        
        roleParams.push_back(std::move(getRole));
        roleParams.push_back(std::make_unique<Var>("ADMIN"));
        adminCheck = std::make_unique<FuncCall>("inList", move(roleParams));

        andParams.push_back(std::move(in1));
        andParams.push_back(std::move(adminCheck));
        andExpr = std::make_unique<FuncCall>("AND", move(andParams));

        // API call: deleteBook(token, bookCode) ==> (OK)
        vector<unique_ptr<Expr>> callArgs;
        callArgs.push_back(std::make_unique<Var>("token"));
        callArgs.push_back(std::make_unique<Var>("bookCode"));
        auto call = std::make_unique<APIcall>(
            std::make_unique<FuncCall>("deleteBook", move(callArgs)),
            Response(HTTPResponseCode::OK_200,std::make_unique<Var>("bookData")));

        // Post-condition: B' = B \ {bookCode}
        deleteParams.push_back(std::make_unique<Var>("B"));
        deleteParams.push_back(std::make_unique<Var>("bookCode"));
        post_condition = std::make_unique<FuncCall>("setDifference", move(deleteParams));
        
        auto api = std::make_unique<API>(std::move(andExpr), std::move(call), Response(HTTPResponseCode::OK_200, move(post_condition)));
        apis.push_back(std::move(api));

        // Adding variables to the symbol table
        SymbolTable *sym = new SymbolTable();
        sym->symtable.insert(Var("token"));
        sym->symtable.insert(Var("bookCode"));
        root.children.push_back(sym);
    }
};
