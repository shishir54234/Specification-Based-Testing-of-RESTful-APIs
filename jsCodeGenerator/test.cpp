#include <iostream>
#include <cassert>
#include <memory>
#include "jsCodeGen.h"
#include "../ast.hpp"
#include "../algo.hpp"
using namespace std;

// class Test {
//     protected:
//         virtual Program makeProgram() = 0; // Pure virtual function
//     public:
//         // Test() = default;
//         virtual ~Test() = default;
    
//         void execute() {
//             theProgram = makeProgram();
//             ExpoSECodeGenerator ecg;
//             string code = ecg.generateCode(theProgram);
//             cout << code;
//         }
    
//     protected:
//         Program theProgram;
// };

// class T1 : public Test {
//     public:
//     public:
//     T1() { cout<<"hello\n";
//         /* Constructor Body */ }
//     // T1() : Test() {}
//     protected:
//         Program makeProgram() {
//             vector<unique_ptr<Decl>> globals;
//             unique_ptr<Decl> global1 = make_unique<Decl>("U", make_unique<MapType>(make_unique<TypeConst>("string"), make_unique<TypeConst>("string")));
//             globals.push_back(move(global1));
//             // vector<unique_ptr<TypeDecl>> fields;
//             vector<unique_ptr<Init>> inits;
//             unique_ptr<Expr> map1 = make_unique<Map>(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>());
//             unique_ptr<Init> init1 = make_unique<Init>("U", move(map1));

//             inits.push_back(move(init1));
//             vector<unique_ptr<FuncDecl>> funcDecls;
//             vector<unique_ptr<TypeExpr>> params;
//             params.push_back(make_unique<TypeConst>("string"));
//             params.push_back(make_unique<TypeConst>("string"));
//             vector<unique_ptr<TypeExpr>> resp;
//             auto signup = make_unique<FuncDecl>("signup", move(params), make_pair(HTTPResponseCode::CREATED_201, move(resp)));
//             funcDecls.push_back(move(signup));
//             vector<unique_ptr<TypeExpr>> params1;
//             params1.push_back(make_unique<TypeConst>("string"));
//             params1.push_back(make_unique<TypeConst>("string"));
//             vector<unique_ptr<TypeExpr>> resp1; resp1.push_back(make_unique<TypeConst>("Token"));
//             auto login=make_unique<FuncDecl>("login", move(params1), make_pair(HTTPResponseCode::CREATED_201, move(resp1)));
//             funcDecls.push_back(move(login));

//             vector<unique_ptr<API>> apis;
//             vector<unique_ptr<Expr>> pre_args;
//             pre_args.push_back(make_unique<Var>("username"));
//             vector<unique_ptr<Expr>> h; h.push_back(make_unique<Var>("U"));
//             pre_args.push_back(make_unique<FuncCall>("dom", move(h)));
//             cout<<"Hello\n";
//             auto pre=make_unique<FuncCall>("not in", move(pre_args));

//             vector<unique_ptr<Expr>> call_args;
//             vector<unique_ptr<Expr>> call_args1;
//             call_args1.push_back(make_unique<Var>("username"));
//             call_args1.push_back(make_unique<Var>("password"));

//             auto call=make_unique<APIcall>(make_unique<FuncCall>("signup",move(call_args1)), Response(HTTPResponseCode::CREATED_201, nullptr));
//             vector<unique_ptr<Expr>> post_args;
//             vector<unique_ptr<Expr>> post_args1;
//             vector<unique_ptr<Expr>> e2; e2.push_back(make_unique<Var>("U"));
//             post_args1.push_back(make_unique<FuncCall>("'",move(e2)));

//             post_args1.push_back(make_unique<Var>("username"));
//             post_args.push_back(make_unique<FuncCall>("[]", move(post_args1)));
//             post_args.push_back(make_unique<Var>("password"));

//             auto api = make_unique<API>(move(pre), move(call), Response(HTTPResponseCode::CREATED_201, make_unique<FuncCall>("=", move(post_args))));
//             apis.push_back(move(api));
//             auto spec = make_unique<Spec>(move(globals), move(inits), move(funcDecls), move(apis));
//             // PrintVisitor visitor;
//             // spec->accept(visitor);

//             SymbolTable symtable;
//             symtable.symtable.insert(Var("U"));
//             SymbolTable sym1;
//             sym1.symtable.insert(Var("username"));
//             sym1.symtable.insert(Var("password"));
//             symtable.children.push_back(&sym1);
//             Program p = convert(spec.get(), symtable);
//             // vector<unique_ptr<Stmt>> stmts;
//             // for(auto &stmt:p.statements){
//             //     stmts.push_back(move(stmt));
//             // }

//             // Program *p1=new Program(move(stmts));
//             return p;
//         }
// };

// int main() {
//     vector<Test *> testcases = {
//         new T1()
//     };
//     try {
//         for(auto& t : testcases) {
//             t->execute();
//             // delete t;
//         }
//     }
//     catch(const string& m) {
//         cout << "Typechecker exception : " << m << endl;
//     }
//     catch(const char* m) {
//         cout << "Typechecker exception : " << m << endl;
//     }
//     catch(...) {
//         cout << "Typechecker exception : " << endl;
//     }
//     return 0;
// }
int main(){
    std::vector<std::unique_ptr<Decl>> globals;
    std::unique_ptr<Decl> global1 = std::make_unique<Decl>("U", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string")));
    globals.push_back(std::move(global1));
    // vector<std::unique_ptr<TypeDecl>> fields;
    vector<unique_ptr<Init>> inits;
    unique_ptr<Expr> map1 = std::make_unique<Map>(vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>>());
    unique_ptr<Init> init1 = std::make_unique<Init>("U", std::move(map1));

    inits.push_back(std::move(init1));
    vector<unique_ptr<FuncDecl>> funcDecls;
    std::vector<std::unique_ptr<TypeExpr>> params;
    params.push_back(std::make_unique<TypeConst>("string"));
    params.push_back(std::make_unique<TypeConst>("string"));
    vector<unique_ptr<TypeExpr>> resp;
    auto signup = make_unique<FuncDecl>("signup", move(params), std::make_pair(HTTPResponseCode::CREATED_201, move(resp)));
    funcDecls.push_back(std::move(signup));
    std::vector<std::unique_ptr<TypeExpr>> params1;
    params1.push_back(std::make_unique<TypeConst>("string"));
    params1.push_back(std::make_unique<TypeConst>("string"));
    vector<unique_ptr<TypeExpr>> resp1;
    resp1.push_back(std::make_unique<TypeConst>("Token"));
    auto login = make_unique<FuncDecl>("login", move(params1), std::make_pair(HTTPResponseCode::CREATED_201, move(resp1)));
    funcDecls.push_back(std::move(login));

    vector<unique_ptr<API>> apis;
    {
        vector<unique_ptr<Expr>> pre_args;
        pre_args.push_back(std::make_unique<Var>("username"));
        vector<unique_ptr<Expr>> h;
        h.push_back(std::make_unique<Var>("U"));
        pre_args.push_back(std::make_unique<FuncCall>("dom", move(h)));
        // cout<<"Hello\n";
        auto pre = std::make_unique<FuncCall>("not in", move(pre_args));

        vector<unique_ptr<Expr>> call_args;
        vector<unique_ptr<Expr>> call_args1;
        call_args1.push_back(std::make_unique<Var>("username"));
        call_args1.push_back(std::make_unique<Var>("password"));

        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("signup", move(call_args1)), Response(HTTPResponseCode::CREATED_201, nullptr));
        vector<unique_ptr<Expr>> post_args;
        vector<unique_ptr<Expr>> post_args1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(std::make_unique<Var>("U"));
        post_args1.push_back(std::make_unique<FuncCall>("'", move(e2)));

        post_args1.push_back(std::make_unique<Var>("username"));
        post_args.push_back(make_unique<FuncCall>("[]", move(post_args1)));
        post_args.push_back(std::make_unique<Var>("password"));

        auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::CREATED_201, make_unique<FuncCall>("=", move(post_args))));
        apis.push_back(std::move(api));
    }
    {
        vector<unique_ptr<Expr>> pre_args;
        pre_args.push_back(std::make_unique<Var>("username"));
        vector<unique_ptr<Expr>> h;
        h.push_back(std::make_unique<Var>("U"));
        pre_args.push_back(std::make_unique<FuncCall>("dom", move(h)));
        // cout<<"Hello\n";
        auto pre = std::make_unique<FuncCall>("not in", move(pre_args));

        vector<unique_ptr<Expr>> call_args;
        vector<unique_ptr<Expr>> call_args1;
        call_args1.push_back(std::make_unique<Var>("username"));
        call_args1.push_back(std::make_unique<Var>("password"));

        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("signup", move(call_args1)), Response(HTTPResponseCode::CREATED_201, nullptr));
        vector<unique_ptr<Expr>> post_args;
        vector<unique_ptr<Expr>> post_args1;
        vector<unique_ptr<Expr>> e2;
        e2.push_back(std::make_unique<Var>("U"));
        post_args1.push_back(std::make_unique<FuncCall>("'", move(e2)));

        post_args1.push_back(std::make_unique<Var>("username"));
        post_args.push_back(make_unique<FuncCall>("[]", move(post_args1)));
        post_args.push_back(std::make_unique<Var>("password"));

        auto api = std::make_unique<API>(std::move(pre), std::move(call), Response(HTTPResponseCode::CREATED_201, make_unique<FuncCall>("=", move(post_args))));
        apis.push_back(std::move(api));
    }
    auto spec = make_unique<Spec>(move(globals), move(inits), move(funcDecls), move(apis));
    PrintVisitor visitor;
    spec->accept(visitor);
    SymbolTable symtable;
    symtable.symtable.insert(Var("U"));
    SymbolTable sym1;
    sym1.symtable.insert(Var("username"));
    sym1.symtable.insert(Var("password"));
    symtable.children.push_back(&sym1);
    Program p = convert(spec.get(), symtable);
    // vector<unique_ptr<Stmt>> stmts;
    // for(auto &stmt:p.statements){
    //     stmts.push_back(move(stmt));
    // }
    ExpoSECodeGenerator ecg;
    string code = ecg.generateCode(p);
    cout << code;
}