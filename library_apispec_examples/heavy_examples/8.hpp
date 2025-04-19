#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"

class heavyexample8
{
public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
    {
        // Precondition: (t -> u in T and u in U)
        vector<unique_ptr<Expr>> args_in1params, args_in2params;
        unique_ptr<Expr> in1, in2, and1;
        
        // t in T
        args_in1params.push_back(std::make_unique<Var>("T"));
        args_in1params.push_back(std::make_unique<Var>("token"));
        in1 = std::make_unique<FuncCall>("inMapVerify", move(args_in1params));
        
        // T[t] (access user u)
        vector<unique_ptr<Expr>> accessMapParams;
        accessMapParams.push_back(std::make_unique<Var>("T"));
        accessMapParams.push_back(std::make_unique<Var>("token"));
        auto mapAccess = std::make_unique<FuncCall>("mapAcess", move(accessMapParams));
        
        // u in U
        args_in2params.push_back(std::move(mapAccess));
        args_in2params.push_back(std::make_unique<Var>("U"));
        in2 = std::make_unique<FuncCall>("inMapVerify", move(args_in2params));
        
        // Combine conditions: (t -> u in T and u in U)
        vector<unique_ptr<Expr>> andParams;
        andParams.push_back(std::move(in1));
        andParams.push_back(std::move(in2));
        and1 = std::make_unique<FuncCall>("AND", move(andParams));
        
        // API Call: getBook(t : Token, bookCode : string) ==> (OK, bookData: Book)
        vector<unique_ptr<Expr>> call_args;
        call_args.push_back(std::make_unique<Var>("token"));
        call_args.push_back(std::make_unique<Var>("bookCode"));
        auto call = std::make_unique<APIcall>(
            make_unique<FuncCall>("getBook", move(call_args)),
            Response(HTTPResponseCode::OK_200, std::make_unique<Var>("bookData"))
        );
        
        // Postcondition: bookCode = bookData.code
        vector<unique_ptr<Expr>> equalsParams;
        equalsParams.push_back(std::make_unique<Var>("bookCode"));
        getCode
        equalsParams.push_back(std::make_unique<FuncCall>("getCode", vector<unique_ptr<Expr>>{std::make_unique<Var>("bookData")}));
        auto postCondition = std::make_unique<FuncCall>("equals", move(equalsParams));
        
        // Construct API object
        auto api = std::make_unique<API>(std::move(and1), std::move(call), Response(HTTPResponseCode::OK_200, move(postCondition)));
        apis.push_back(std::move(api));
        
        // Add variables to the symbol table
        SymbolTable *sym = new SymbolTable();
        sym->symtable.insert(Var("token"));
        sym->symtable.insert(Var("bookCode"));
        root.children.push_back(sym);
    }
};
