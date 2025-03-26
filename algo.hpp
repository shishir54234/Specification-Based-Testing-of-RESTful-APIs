
#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <memory>
using namespace std;
#include "ast.hpp"
#include "visitor.hpp"
#ifndef ALGO_HPP
#define ALGO_HPP
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

    string to_string(){
        string s;
        for(auto &var:symtable){
            s+=var.name;
        }
        return s;
    }
};
unique_ptr<Expr> convert1(unique_ptr<Expr> &expr, SymbolTable *symtable, const string &add)
{
    if (!expr)
    {
        return nullptr;
    }

    if (auto var = dynamic_cast<Var *>(expr.get()))
    {
        
        if (symtable->exists(*var))
        {
            // cout <<"1"<< var->name << endl;
            return make_unique<Var>(var->name + add);
        }
        // cout <<"0"<< var->name << endl;
        return make_unique<Var>(var->name);
    }

    if (auto func = dynamic_cast<FuncCall *>(expr.get()))
    {
        vector<unique_ptr<Expr>> args;
        for (auto &arg : func->args)
        {
            args.push_back(convert1(arg, symtable, add));
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
            elements.push_back(convert1(element, symtable, add));
        }
        return make_unique<Set>(std::move(elements));
    }

    if (auto map = dynamic_cast<Map *>(expr.get()))
    {
        vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> ret;
        for (int i = 0; i < map->value.size(); i++)
        {
            auto key = convert1(reinterpret_cast<unique_ptr<Expr>&>(map->value[i].first), symtable, add);
            auto value = convert1(map->value[i].second, symtable, add);
            ret.push_back(make_pair(std::move(reinterpret_cast<unique_ptr<Var>&>(key)), std::move(value)));
        }
        return make_unique<Map>(std::move(ret));
    }

    if (auto tuple = dynamic_cast<Tuple *>(expr.get()))
    {
        vector<unique_ptr<Expr>> exprs;
        for (auto &exp : tuple->expr)
        {
            exprs.push_back(convert1(exp, symtable, add));
        }
        return make_unique<Tuple>(std::move(exprs));
    }

    // Handle unknown expression type
    throw runtime_error("Unknown expression type in convert function");
}
// U' = U union {}
// ===>
// U_old= U
// api call
// U= U_old union {}
void addthedashexpr(unique_ptr<Expr> &expr, set<string> &res){
    if (!expr)
    {
        return;
    }
    cout<<res.size()<<endl;
    if(res.size()>0)
    {
        cout<<"The res right now has"<<endl;
        for(auto &x: res)
        {
            cout<<x<<" ";
        }
        cout<<endl;
    }

    if (auto var = dynamic_cast<Var *>(expr.get()))
    {
        return;
    }
    if (auto func = dynamic_cast<FuncCall *>(expr.get()))
    {
        if(func->name=="'"){
            auto *v = func->args[0].get();
            if (auto var = dynamic_cast<Var *>(v)) {
                res.insert(var->name);
            }
            
        }
        else{
            vector<unique_ptr<Expr>> args;
            for (auto &arg : func->args)
            {
                addthedashexpr(arg, res);
            }
        }
        return;
    }

    if (auto num = dynamic_cast<Num *>(expr.get()))
    {
        return;
    }

    if (auto set = dynamic_cast<Set *>(expr.get()))
    {
        vector<unique_ptr<Expr>> elements;
        for (auto &element : set->elements)
        {
            addthedashexpr(element, res);
        }
    }

    if (auto map = dynamic_cast<Map *>(expr.get()))
    {
        vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> ret;
        for (int i = 0; i < map->value.size(); i++)
        {
            addthedashexpr(reinterpret_cast<unique_ptr<Expr> &>(map->value[i].first), res);
            addthedashexpr(map->value[i].second, res);
        }
    }

    if (auto tuple = dynamic_cast<Tuple *>(expr.get()))
    {
        vector<unique_ptr<Expr>> exprs;
        for (auto &exp : tuple->expr)
        {
            addthedashexpr(exp, res);
        }
    }
    // cout<<"WHAT YOU DIDNT HANDLE THIS ????"<<endl;
    // // Handle unknown expression type
    throw runtime_error("Unknown expression type in convert function");
}
unique_ptr<Expr> removethedashexpr(unique_ptr<Expr>&expr, set<string> &res, int flag=0)
{
    if(!expr){
        return NULL;
    }
    if (auto var = dynamic_cast<Var *>(expr.get()))
    {
        if(flag)
        {
            return make_unique<Var>(var->name);
        }   
        else if(!flag && (res.find(var->name) != res.end()))
        {
            return make_unique<Var>(var->name+"_old");
        }
        else
        {
            return make_unique<Var>(var->name);
        }        
    }

    if (auto func = dynamic_cast<FuncCall *>(expr.get()))
    {
        if(func->name == "'")
        {
            auto expr=removethedashexpr(func->args[0],res,1);
            return expr;
        }
        vector<unique_ptr<Expr>> args;
        for (auto &arg : func->args)
        {
            args.push_back(removethedashexpr(arg, res));
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
            elements.push_back(removethedashexpr(element,res));
        }
        return make_unique<Set>(std::move(elements));
    }

    if (auto map = dynamic_cast<Map *>(expr.get()))
    {
        vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> ret;
        for (int i = 0; i < map->value.size(); i++)
        {
            auto key = removethedashexpr(reinterpret_cast<unique_ptr<Expr> &>(map->value[i].first), res);
            auto value = removethedashexpr(map->value[i].second, res);
            ret.push_back(make_pair(std::move(reinterpret_cast<unique_ptr<Var> &>(key)), std::move(value)));
        }
        return make_unique<Map>(std::move(ret));
    }

    if (auto tuple = dynamic_cast<Tuple *>(expr.get()))
    {
        vector<unique_ptr<Expr>> exprs;
        for (auto &exp : tuple->expr)
        {
            exprs.push_back(removethedashexpr(exp, res));
        }
        return make_unique<Tuple>(std::move(exprs));
    }

    // Handle unknown expression type
    throw runtime_error("Unknown expression type in convert function");
}
unique_ptr<FuncCallStmt> makeStmt(unique_ptr<Expr> expr)
{
    // Create Var from the expr rather than using make_unique directly
    auto call = dynamic_cast<Var *>(expr.get());
    if (call)
    {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>(call->name));
        auto h=make_unique<FuncCall>("input", move(args));
        return make_unique<FuncCallStmt>(std::move(h));
    }
    return nullptr;
}
void getInputVars(unique_ptr<Expr> &expr,vector<unique_ptr<Expr>> &InputVariables, string toadd, SymbolTable *symtable){
    if (auto var = dynamic_cast<Var *>(expr.get())){
        InputVariables.push_back(convert1(expr,symtable,toadd));
        return;
    }
    if (auto func = dynamic_cast<FuncCall *>(expr.get())){
        for(auto &arg:func->args){
            getInputVars(arg,InputVariables,toadd,symtable);
        }
    }
    if(auto set=dynamic_cast<Set *>(expr.get())){
        for(auto &element:set->elements){
            getInputVars(element,InputVariables,toadd,symtable);
        }   
    }
    if (auto tuple = dynamic_cast<Tuple *>(expr.get()))
    {
        for(auto &exp:tuple->expr){
            getInputVars(exp,InputVariables,toadd,symtable);
        }
    }
    if(auto map1=dynamic_cast<Map *>(expr.get())){
        for(auto &element:map1->value){
            getInputVars(reinterpret_cast<unique_ptr<Expr>&>(element.first), InputVariables, toadd, symtable);
            getInputVars(element.second,InputVariables,toadd,symtable);
        }
    }
    if(auto num=dynamic_cast<Num *>(expr.get())){
        return;
    }
}

Program convert(const Spec *apispec, SymbolTable symtable){
    vector<unique_ptr<Stmt>> program_stmts;
     cout<<"we got here"<<apispec->blocks.size()<<endl;
    for(int i=0;i<apispec->blocks.size();i++){
        cout<<"Index: "<<i<<endl;
        auto currtable=symtable.children[i];
        cout<<"Implmentation of to_string function"<<endl;
        cout<<currtable->to_string()<<endl;
        auto currblock = std::move(const_cast<std::unique_ptr<API>&>(apispec->blocks[i]));
        auto pre=std::move(currblock->pre);
        auto call=std::move(currblock->call);
        auto response=std::move(currblock->response);
        auto post = std::move(response.expr);
        
        vector<unique_ptr<Expr>> InputVariables;
        for(int j=0;j<call->call->args.size();j++){
            getInputVars(call->call->args[j], InputVariables, to_string(i), currtable);
        }
        for(int j=0;j<InputVariables.size();j++){
            program_stmts.push_back(makeStmt(std::move(InputVariables[j])));
        }

        cout<<"We made it here"<<endl;
        auto pre1=convert1(pre,currtable,to_string(i));
        auto callexpr = std::make_unique<FuncCall>(call->call->name, std::move(call->call->args));
        auto call1=convert1(reinterpret_cast<unique_ptr<Expr>&>(callexpr),currtable,to_string(i));
        auto post1=convert1(post,currtable,to_string(i));
        cout << "before this it works" << endl;
        set<string> res;
        addthedashexpr(post1, res);
        cout<<"So the dash works.."<<endl;
        for (auto &s : res)
        {
            cout<<"Variables with dash"<<s<<endl;
            auto g = makeStmt(make_unique<Var>(s + "_old"));
            vector<unique_ptr<Expr>> v;
            v.push_back(make_unique<Var>(s + "_old"));
            v.push_back(make_unique<Var>(s));
            unique_ptr<FuncCall> curr = make_unique<FuncCall>("=", move(v));
            unique_ptr<Stmt> funCall = make_unique<FuncCallStmt>(move(curr));
            program_stmts.push_back(move(funCall));
        }
        cout<<"So we could make the stmts"<<endl;
        PrintVisitor p;
        vector<unique_ptr<Expr>> v1;v1.push_back(std::move(pre1));
        
        unique_ptr<FuncCall> p2 = make_unique<FuncCall>("assume", std::move(v1));
        unique_ptr<FuncCallStmt> c2=make_unique<FuncCallStmt>(move(p2));
        program_stmts.push_back(std::move(c2));
        
        auto call2 = std::move(call1);
        unique_ptr<FuncCallStmt> c4 = make_unique<FuncCallStmt>(
            unique_ptr<FuncCall>(static_cast<FuncCall *>(call2.release())));
        program_stmts.push_back(move(c4));
        post1=removethedashexpr(post1,res);
        vector<unique_ptr<Expr>> v2; v2.push_back(std::move(post1));
        unique_ptr<FuncCall> p3=make_unique<FuncCall>("assert", std::move(v2));
        unique_ptr<FuncCallStmt> c3=make_unique<FuncCallStmt>(move(p3));
        program_stmts.push_back(std::move(c3));
        // auto response1=convert(response.first,currtable,to_string(i));
        // currblock->pre=std::move(pre1);
        // currblock->call=std::move(call1);
        // currblock->response=std::move({response1,response.second});
        cout<<"The program statement sizes"<<program_stmts.size()<<"\n";
    }
    
    return Program(std::move(program_stmts));
}

#endif // ALGO_HPP