#include<bits/stdc++.h>
using namespace std;
#include "ast.hpp"
#include "atc_ast.hpp"
class SymbolTable{
    public:
    vector<SymbolTable*> children;
    SymbolTable* par;
    set<Var> symtable;
    bool exists(Var v){
        if(symtable.find(v)==symtable.end()){
            return false;
        }
        else {
            return true;
        }
    }
};
unique_ptr<Expr> convert(unique_ptr<Expr> &expr, SymbolTable *symtable, const string &add)
{
    if (!expr)
    {
        return nullptr;
    }

    if (auto var = dynamic_cast<Var *>(expr.get()))
    {
        if (symtable->exists(*var))
        {
            return make_unique<Var>(var->name + add);
        }
        return make_unique<Var>(var->name);
    }

    if (auto func = dynamic_cast<FuncCall *>(expr.get()))
    {
        vector<unique_ptr<Expr>> args;
        for (auto &arg : func->args)
        {
            args.push_back(convert(arg, symtable, add));
        }
        return make_unique<FuncCall>(func->name, std::move(args));
    }

    if (auto num = dynamic_cast<Num *>(expr.get()))
    {
        return make_unique<Num>(num->value);
    }

    if (auto set = dynamic_cast<Set *>(expr.get()))
    {
        vector<unique_ptr<Expr>> elements;
        for (auto &element : set->elements)
        {
            elements.push_back(convert(element, symtable, add));
        }
        return make_unique<Set>(std::move(elements));
    }

    if (auto map = dynamic_cast<Map *>(expr.get()))
    {
        vector<pair<unique_ptr<Expr>, unique_ptr<Expr>>> ret;
        for (int i = 0; i < map->value.size(); i++)
        {
            auto keyExpr = make_unique<Expr>(*map->value[i].first);
            auto valueExpr = make_unique<Expr>(*map->value[i].second);
            auto key = convert(keyExpr, symtable, add);
            auto value = convert(valueExpr, symtable, add);
            ret.push_back(make_pair(std::move(key), std::move(value)));
        }
        return make_unique<Map>(std::move(ret));
    }

    if (auto tuple = dynamic_cast<Tuple *>(expr.get()))
    {
        vector<unique_ptr<Expr>> exprs;
        for (auto &exp : tuple->expr)
        {
            exprs.push_back(convert(exp, symtable, add));
        }
        return make_unique<Tuple>(std::move(exprs));
    }

    // Handle unknown expression type
    throw runtime_error("Unknown expression type in convert function");
}
unique_ptr<Stmt> makeStmt(unique_ptr<Expr> expr){
    unique_ptr<FuncCall> call=std::make_unique<FuncCall>(std::move(expr));
    unique_ptr<FuncCallStmt> fs=std::make_unique<FuncCallStmt>(std::move(call));
    return std::move(fs);
}

Program convert(Spec &apispec, SymbolTable *symtable){
    vector<unique_ptr<Stmt>> program_stmts;
    for(int i=0;i<apispec.blocks.size();i++){
        auto currtable=symtable->children[i];
        auto currblock=std::move(apispec.blocks[i]);
        auto pre=std::move(currblock->pre);
        auto call=std::move(currblock->call);
        auto response=std::move(currblock->response);
        auto post = std::move(response.second);
        
        vector<unique_ptr<Expr>> InputVariables;

        auto pre1=convert(pre,currtable,to_string(i));
        auto callexpr=std::make_unique<Expr>(*call);
        auto call1=convert(callexpr,currtable,to_string(i));
        auto post1=convert(post,currtable,to_string(i));

        program_stmts.push_back(makeStmt(std::move(pre1)));
        program_stmts.push_back(makeStmt(std::move(call1)));
        program_stmts.push_back(makeStmt(std::move(post1)));
        // auto response1=convert(response.first,currtable,to_string(i));
        // currblock->pre=std::move(pre1);
        // currblock->call=std::move(call1);
        // currblock->response=std::move({response1,response.second});

    }
    return Program(std::move(program_stmts));
}

