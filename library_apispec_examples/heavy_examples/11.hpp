// ### 13. Save Book
// --------------------------------SAVEBOOK_OK--------------------------------
// PRECONDITION: (t -> u in T and u in U) AND (ADMIN in u.role)
// saveBook(t : Token, bookData : Book) ==> (OK)
// POSTCONDITION: B' = B[bookData.code -> bookData]
// -----------------------------------------------------------------------------

#include <memory>
#include <vector>
#include "../../ast.hpp"
#include "../../algo.hpp"

class heavyexample11{
public:
    static void example(vector<unique_ptr<API>> &apis, SymbolTable &root) {
        vector<unique_ptr<Expr>> args_in1params, args_in2params;
        vector<unique_ptr<Expr>> accessMap1params, getId1params, getRoleParams;
        vector<unique_ptr<Expr>> andParams;
        vector<unique_ptr<Expr>> in1Params, in2Params;
        unique_ptr<Expr> mapAccess1, getId1, getRoles;
        unique_ptr<Expr> in1, in2, and1;

        // t -> u in T
        args_in1params.push_back(std::make_unique<Var>("T"));
        args_in1params.push_back(std::make_unique<Var>("token"));
        in1 = std::make_unique<FuncCall>("inMapVerify", move(args_in1params));
        
        // u in U
        accessMap1params.push_back(std::make_unique<Var>("T"));
        accessMap1params.push_back(std::make_unique<Var>("token"));
        mapAccess1 = std::make_unique<FuncCall>("mapAcess", move(accessMap1params));

        getId1params.push_back(std::move(mapAccess1));
        getId1 = std::make_unique<FuncCall>("getId", move(getId1params));
        
        args_in2params.push_back(std::move(getId1));
        args_in2params.push_back(std::make_unique<Var>("U"));
        in2 = std::make_unique<FuncCall>("inMapVerify", move(args_in2params));

        // ADMIN in u.role
        getRoleParams.push_back(std::move(mapAccess1));
        getRoles = std::make_unique<FuncCall>("getRoles", move(getRoleParams));
        
        in1Params.push_back(std::move(getRoles));
        in1Params.push_back(std::make_unique<Var>("ADMIN"));
        in1 = std::make_unique<FuncCall>("inList", move(in1Params));

        // Precondition: (t -> u in T and u in U) AND (ADMIN in u.role)
        andParams.push_back(std::move(in1));
        andParams.push_back(std::move(in2));
        and1 = std::make_unique<FuncCall>("AND", move(andParams));

        // API Call: saveBook(token, bookData)
        vector<unique_ptr<Expr>> call_args;
        call_args.push_back(std::make_unique<Var>("token"));
        call_args.push_back(std::make_unique<Var>("bookData"));
        auto call = std::make_unique<APIcall>(make_unique<FuncCall>("saveBook", move(call_args)), Response(HTTPResponseCode::OK_200,std::make_unique<Var>("book")));

        // POSTCONDITION: B' = B[bookData.code -> bookData]
        vector<unique_ptr<Expr>> post_args;
        post_args.push_back(std::make_unique<Var>("B"));
        post_args.push_back(std::make_unique<Var>("bookData"));
        auto postcondition = std::make_unique<FuncCall>("updateMap", move(post_args));

        // Define API
        auto api = std::make_unique<API>(std::move(and1), std::move(call), Response(HTTPResponseCode::OK_200, move(postcondition)));
        apis.push_back(std::move(api));

        // Adding variables to the symbol table
        SymbolTable *symTable = new SymbolTable();
        symTable->symtable.insert(Var("token"));
        symTable->symtable.insert(Var("bookData"));
        root.children.push_back(symTable);
    }
};