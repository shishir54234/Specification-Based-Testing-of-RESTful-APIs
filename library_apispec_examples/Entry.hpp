#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include <memory>
#include "../ast.hpp"
#include "../algo.hpp"
// we have the globals here and we have the init here 
// and we have the functions here and we have the blocks here
#include "heavy_examples/1.hpp"
#include "light_examples/1.hpp"
#include "light_examples/2.hpp"
#include "light_examples/3.hpp"


class Entry
{
    public: 
    pair<unique_ptr<Spec>, SymbolTable> static start()
    {
        std::vector<std::unique_ptr<Decl>> globals;

        std::vector<std::unique_ptr<TypeExpr>> tupletypeargs;

        tupletypeargs.push_back(std::make_unique<TypeConst>("string"));
        tupletypeargs.push_back(std::make_unique<TypeConst>("string")); // {, std::make_unique<TypeConst>("string")};

        // U : (string -> string) map // Users
        globals.push_back(std::make_unique<Decl>("U", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
        // S : (string -> {string, string}) map // Students
        globals.push_back(std::make_unique<Decl>("S", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TupleType>(std::move(tupletypeargs)))));
        // T : (string -> string)
        globals.push_back(std::make_unique<Decl>("T", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"))));
        // B:(string->Book) How do we write this? Maybe we need custom types? Or do we simplify the Book into a tuple type ?
        // R : Request Set // Requests
        // BS:BookStudent Set // Book-Student Records

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
        // lightexample1::example(apis, symtable);
        // lightexample2::example(apis, symtable);
        // lightexample3::example(apis, symtable);
        heavyexample1::example(apis, symtable);

        auto spec = make_unique<Spec>(move(globals), move(inits), move(funcDecls), move(apis));
        return make_pair(std::move(spec), std::move(symtable));
        // spec->accept(*this);
    }

};