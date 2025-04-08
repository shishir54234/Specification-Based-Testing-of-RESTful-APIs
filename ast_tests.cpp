#include <iostream>
#include <cassert>
#include <memory>
#include "PrintVisitor.hpp"
#include "algo.hpp"
using namespace std;
// Assuming the AST classes are in "ast.hpp"

// Helper function to create test data
void testEverything(){
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
    vector<unique_ptr<TypeExpr>> resp1; resp1.push_back(std::make_unique<TypeConst>("Token"));
    auto login=make_unique<FuncDecl>("login", move(params1), std::make_pair(HTTPResponseCode::CREATED_201, move(resp1)));
    funcDecls.push_back(std::move(login));

    vector<unique_ptr<API>> apis;
    if(1){
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
    if(1){
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
    auto spec = std::make_unique<Spec>(move(globals), move(inits), move(funcDecls), move(apis));
    PrintVisitor visitor;
    spec->accept(visitor);

    SymbolTable symtable;
    symtable.symtable.insert(Var("U"));
    SymbolTable sym1;
    sym1.symtable.insert(Var("username"));
    sym1.symtable.insert(Var("password"));
    symtable.children.push_back(&sym1);
    SymbolTable sym2;
    sym2.symtable.insert(Var("username"));
    sym2.symtable.insert(Var("password"));
    symtable.children.push_back(&sym2);

    Program p = convert(spec.get(), symtable);
    PrintVisitor visitor1;
    p.accept(visitor1);

}
void test1(){
    // auto typeConst = std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string"));
 
    // // Create visitor
    // PrintVisitor visitor;
    // // Visit the node - this should print to console
    // typeConst->accept(visitor);
    // auto decltest = std::make_unique<Decl>("U", std::make_unique<MapType>(std::make_unique<TypeConst>("string"), std::make_unique<TypeConst>("string")));
    // decltest->accept(visitor);
}
// void test2(){
//     vector<unique_ptr<FuncDecl>> funcDecls;
//     std::vector<std::unique_ptr<TypeExpr>> params;
//     params.push_back(std::make_unique<TypeConst>("string"));
//     params.push_back(std::make_unique<TypeConst>("string"));
//     vector<unique_ptr<TypeExpr>> resp;
//     auto signup = make_unique<FuncDecl>("signup", move(params), std::make_pair(HTTPResponseCode::CREATED_201, move(resp)));
//     funcDecls.push_back(std::move(signup));
//     std::vector<std::unique_ptr<TypeExpr>> params1;
//     params1.push_back(std::make_unique<TypeConst>("string"));
//     params1.push_back(std::make_unique<TypeConst>("string"));
//     vector<unique_ptr<TypeExpr>> resp1;
//     resp1.push_back(std::make_unique<TypeConst>("Token"));
//     auto login = make_unique<FuncDecl>("login", move(params1), std::make_pair(HTTPResponseCode::CREATED_201, resp1));
//     // funcDecls.push_back(std::move(login));
// }
int main()
{
    try
    {
        testEverything();
        // test1();
        // test2();
        std::cout << "\nAll tests passed successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}