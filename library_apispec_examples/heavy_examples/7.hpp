#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../../ast.hpp"
#include "../../algo.hpp"

class heavyexample7
{
public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root)
    {
        vector<unique_ptr<Expr>> args_in1params, accessMap1params, getId1params;
        vector<unique_ptr<Expr>> args_in2params;
        unique_ptr<Expr> mapAccess1, getId1;
        unique_ptr<Expr> in1, in2, and1;
        vector<unique_ptr<Expr>> and1params;

        // t in T
        args_in1params.push_back(std::make_unique<Var>("T"));
        args_in1params.push_back(std::make_unique<Var>("token"));
        in1 = std::make_unique<FuncCall>("inMapVerify", move(args_in1params));

        // T[t]
        accessMap1params.push_back(std::make_unique<Var>("T"));
        accessMap1params.push_back(std::make_unique<Var>("token"));
        mapAccess1 = std::make_unique<FuncCall>("mapAcess", move(accessMap1params));

        // T[t].id
        getId1params.push_back(std::move(mapAccess1));
        getId1 = std::make_unique<FuncCall>("getId", move(getId1params));

        // T[t].id in U
        args_in2params.push_back(move(getId1));
        args_in2params.push_back(std::make_unique<Var>("U"));
        in2 = std::make_unique<FuncCall>("inMapVerify", move(args_in2params));

        // (t -> u in T and u in U)
        and1params.push_back(move(in1));
        and1params.push_back(move(in2));
        and1 = std::make_unique<FuncCall>("AND", move(and1params));

        // API Call: getAllBooks(t : Token) ==> (OK, bookList: List[Book])
        vector<unique_ptr<Expr>> call_args;
        call_args.push_back(std::make_unique<Var>("token"));
        auto call = std::make_unique<APIcall>(
            make_unique<FuncCall>("getAllBooks", move(call_args)),
            Response(HTTPResponseCode::OK_200, std::make_unique<Var>("bookList")));

        // POSTCONDITION: bookList contains all books
        vector<unique_ptr<Expr>> post_args;
        post_args.push_back(std::make_unique<Var>("bookList"));
        post_args.push_back(std::make_unique<Var>("ALL_BOOKS"));
        auto postcondition = std::make_unique<FuncCall>("containsAll", move(post_args));

        // Full API Definition
        auto api = std::make_unique<API>(std::move(and1), std::move(call), Response(HTTPResponseCode::OK_200, move(postcondition)));
        apis.push_back(std::move(api));

        // Adding variables to the symbol table
        SymbolTable *sym5 = new SymbolTable();
        sym5->symtable.insert(Var("token"));
        root.children.push_back(sym5);
    }
};
