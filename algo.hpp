#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <memory>
using namespace std;
#include "ast.hpp"
#include "PrintVisitor.hpp"
#ifndef ALGO_HPP
#define ALGO_HPP
class SymbolTable
{
public:
    vector<SymbolTable *> children;
    SymbolTable *par;
    set<Var> symtable;
    bool exists(Var v)
    {
        if (symtable.find(v) == symtable.end())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    string to_string(){
        string s="";
        cout<<"Number of elements here are"<<symtable.size()<<endl;
        for(auto &var:symtable){
            cout<<var.name<<" ";
            s+=var.name;
        }
        cout<<"The final string is"<<s<<" "<<endl;
        return s;
    }
};
class TypeMap
{
   public:
    TypeMap* par;
    vector<TypeMap *> children;
    // Mapping from a variable name to its type 
    map<string, TypeExpr *>mapping;
};
// linear
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
            return make_unique<Var>(var->name + add);
        }
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
            auto key = convert1(reinterpret_cast<unique_ptr<Expr> &>(map->value[i].first), symtable, add);
            auto value = convert1(map->value[i].second, symtable, add);
            ret.push_back(make_pair(std::move(reinterpret_cast<unique_ptr<Var> &>(key)), std::move(value)));
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
    // throw runtime_error("Unknown expression type in convert function");
}
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
        return;
    }

    if (auto map = dynamic_cast<Map *>(expr.get()))
    {
        vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> ret;
        for (int i = 0; i < map->value.size(); i++)
        {
            addthedashexpr(reinterpret_cast<unique_ptr<Expr> &>(map->value[i].first), res);
            addthedashexpr(map->value[i].second, res);
        }
        return;
    }

    if (auto tuple = dynamic_cast<Tuple *>(expr.get()))
    {
        vector<unique_ptr<Expr>> exprs;
        for (auto &exp : tuple->expr)
        {
            addthedashexpr(exp, res);
        }
        return;
    }
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
        auto h = make_unique<FuncCall>("input", move(args));
        return make_unique<FuncCallStmt>(std::move(h));
    }
    return nullptr;
}
void getInputVars(unique_ptr<Expr> &expr,vector<unique_ptr<Expr>> &InputVariables, string toadd, SymbolTable *symtable, TypeMap &final, TypeMap &typemap){
    if(!expr){
        return;
    }
    if (auto var = dynamic_cast<Var *>(expr.get())){
        if(symtable->exists(*var))
        {
            // we know its an input variable 
            // we need to get its type
            auto it=typemap.mapping.find(var->name);
            if(it!=typemap.mapping.end())
            {
                auto type=it->second;
                // we need to add the type to the final type map 
                final.mapping[var->name+toadd]=type;
            }
        }
        InputVariables.push_back(convert1(expr,symtable,toadd));
        return;
    }
    if (auto func = dynamic_cast<FuncCall *>(expr.get()))
    {
        for (auto &arg : func->args)
        {
            getInputVars(arg, InputVariables, toadd, symtable, final, typemap);
        }
    }
    if (auto set = dynamic_cast<Set *>(expr.get()))
    {
        for (auto &element : set->elements)
        {
            getInputVars(element, InputVariables, toadd, symtable, final, typemap);
        }
    }
    if (auto tuple = dynamic_cast<Tuple *>(expr.get()))
    {
        for (auto &exp : tuple->expr)
        {
            getInputVars(exp, InputVariables, toadd, symtable,final,typemap);
        }
    }
    if (auto map1 = dynamic_cast<Map *>(expr.get()))
    {
        for (auto &element : map1->value)
        {
            getInputVars(reinterpret_cast<unique_ptr<Expr> &>(element.first), InputVariables, toadd, symtable, final, typemap);
            getInputVars(element.second, InputVariables, toadd, symtable, final, typemap);
        }
    }
    if (auto num = dynamic_cast<Num *>(expr.get()))
    {
        return;
    }
}
// all couts are debug statements ignore them 
// all couts are debug statements ignore them 
Program convert(const Spec *apispec, SymbolTable symtable, TypeMap typemap=TypeMap()){
    
    TypeMap finaltm=TypeMap();

    vector<unique_ptr<Stmt>> program_stmts;
     cout<<"we got here"<<apispec->blocks.size()<<endl;
    for(int i=0;i<apispec->blocks.size();i++){
        TypeMap *itm=new TypeMap();
        cout<<"Index: "<<i<<endl;
        SymbolTable *currtable = symtable.children[i];
        cout<<"Implmentation of to_string function"<<endl;
        // cout<<symtable.children[i]<<endl;
        cout<<currtable->to_string()<<endl;
        // take the current block its pre condition somewhere, 
        //its post condition somewhere, and its call, response also in variables 
        auto currblock = std::move(const_cast<std::unique_ptr<API>&>(apispec->blocks[i]));

        auto pre=std::move(currblock->pre);
        
        auto call=std::move(currblock->call);
         
        auto response=std::move(currblock->response);
        
        auto post = std::move(response.expr);
        
        cout<<"Came here1\n";
        // This section sees to that the input variables are made into appropriate statements 
        vector<unique_ptr<Expr>> InputVariables;
        for (int j = 0; j < call->call->args.size(); j++)
        {
            getInputVars(call->call->args[j], InputVariables, 
            to_string(i), currtable, finaltm, *(itm));

        }
        cout << "Came here2\n";
        // Making Statements for the input variables 
        // Making Statements for the input variables 
        for(int j=0;j<InputVariables.size();j++){
            program_stmts.push_back(makeStmt(std::move(InputVariables[j])));
        }

        cout<<"We made it here"<<endl;
        // we change the variables names here appropriately for e.g. adding uid --> uid + (i), but not changing
        // the global variables
        // we change the variables names here appropriately for e.g. adding uid --> uid + (i), but not changing
        // the global variables
        auto pre1=convert1(pre,currtable,to_string(i));
        auto callexpr = std::make_unique<FuncCall>(call->call->name, std::move(call->call->args));
        auto call1=convert1(reinterpret_cast<unique_ptr<Expr>&>(callexpr),currtable,to_string(i));
        auto post1=convert1(post,currtable,to_string(i));
        cout << "before this it works" << endl;

        // we get those global variable names where we have to add the ' to them 

        // we get those global variable names where we have to add the ' to them 
        set<string> res;
        PrintVisitor p;
        post1->accept(p);
        addthedashexpr(post1, res);
        cout<<"So the dash works.."<<endl;
        
        
        vector<unique_ptr<Expr>> v1;v1.push_back(std::move(pre1));
        // Making the Precondition Statement 
        unique_ptr<FuncCall> p2 = make_unique<FuncCall>("assume", std::move(v1));
        unique_ptr<FuncCallStmt> c2 = make_unique<FuncCallStmt>(move(p2));
        program_stmts.push_back(std::move(c2));
        
        // Making statemnents for the variables with a dash 
        for (auto &s : res)
        {
            cout << "Variables with dash" << s << endl;
            cout << "Variables with dash" << s << endl;
            auto g = makeStmt(make_unique<Var>(s + "_old"));
            vector<unique_ptr<Expr>> v;
            v.push_back(make_unique<Var>(s + "_old"));
            v.push_back(make_unique<Var>(s));
            unique_ptr<FuncCall> curr = make_unique<FuncCall>("=", move(v));
            unique_ptr<Stmt> funCall = make_unique<FuncCallStmt>(move(curr));
            program_stmts.push_back(move(funCall));
        }
        cout<<"So we could make the stmts"<<endl;


        
        auto call2 = std::move(call1);
        unique_ptr<FuncCallStmt> c4 = make_unique<FuncCallStmt>(
            unique_ptr<FuncCall>(static_cast<FuncCall *>(call2.release())));
        program_stmts.push_back(move(c4));

        // Since the variables that need to be replaced with the old , from ' to no dash would lie in the postexpression
        // we run the post condition through the removethedash expr

        // Since the variables that need to be replaced with the old , from ' to no dash would lie in the postexpression
        // we run the post condition through the removethedash expr
        post1=removethedashexpr(post1,res);

        // just making program statements now 

        // just making program statements now 
        vector<unique_ptr<Expr>> v2; v2.push_back(std::move(post1));
        unique_ptr<FuncCall> p3=make_unique<FuncCall>("assert", std::move(v2));
        unique_ptr<FuncCallStmt> c3=make_unique<FuncCallStmt>(move(p3));
        program_stmts.push_back(std::move(c3));
        cout<<"The program statement sizes"<<program_stmts.size()<<"\n";
    }

    return Program(std::move(program_stmts));
}

#endif // ALGO_HPP