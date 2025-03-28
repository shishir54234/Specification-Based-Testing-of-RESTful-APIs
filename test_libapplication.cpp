#include <iostream>
#include <cassert>
#include <memory>
#include "jsCodeGenerator/jsCodeGen.h"
#include "ast.hpp"
#include "algo.hpp"
#include "library_apispec_examples/Entry.hpp"
using namespace std;

void test_everything(){

    // register admin register student
    
    
    
    auto res=Entry::start();
    auto spec = std::move(res.first);
    auto symtable=res.second;
    PrintVisitor visitor;
    spec->accept(visitor);
    Program p = convert(spec.get(), symtable);
    // vector<unique_ptr<Stmt>> stmts;
    // for(auto &stmt:p.statements){
    //     stmts.push_back(move(stmt));
    // }
    p.accept(visitor);
    ExpoSECodeGenerator ecg;

    cout<<"generating expoSE code:";
    string code = ecg.generateCode(p);
    cout << code;
}


int main(){
    test_everything();
    return 0;
}

