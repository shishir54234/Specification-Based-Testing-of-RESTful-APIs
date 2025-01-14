#include <iostream>
#include <cassert>
#include <memory>
#include "ast.hpp"
// Assuming the AST classes are in "ast.hpp"

// Helper function to create test data
std::unique_ptr<TypeExpr> createSampleTypeExpr()
{
    return std::make_unique<TypeConst>("int");
}

std::unique_ptr<Expr> createSampleExpr()
{
    return std::make_unique<Num>(42);
}

void testDecl()
{
    std::cout << "Testing Decl..." << std::endl;

    auto typeExpr = createSampleTypeExpr();
    Decl decl("testVar", std::move(typeExpr));

    assert(decl.name == "testVar");
    assert(decl.type != nullptr);

    std::cout << "Decl test passed" << std::endl;
}

void testFuncType()
{
    std::cout << "Testing FuncType..." << std::endl;

    std::vector<std::unique_ptr<TypeExpr>> params;
    params.push_back(std::make_unique<TypeConst>("int"));
    params.push_back(std::make_unique<TypeConst>("string"));

    auto returnType = std::make_unique<TypeConst>("bool");

    FuncType funcType(std::move(params), std::move(returnType));

    assert(funcType.params.size() == 2);
    assert(funcType.returnType != nullptr);

    std::cout << "FuncType test passed" << std::endl;
}
void testSignup()
{
    std::cout << "Testing Signup Function..." << std::endl;

    // Create parameter types for the function type
    auto paramTypes = std::vector<std::unique_ptr<TypeExpr>>();
    paramTypes.push_back(std::make_unique<TypeConst>("string"));
    paramTypes.push_back(std::make_unique<TypeConst>("string"));

    // Create return tuple type elements
    auto tupleElements = std::vector<std::unique_ptr<TypeExpr>>();
    tupleElements.push_back(std::make_unique<TypeConst>("Token"));
    tupleElements.push_back(std::make_unique<TypeConst>("HttpResponse"));

    // Create the tuple type for return value
    auto tupleType = std::make_unique<TupleType>(std::move(tupleElements));

    // Create the function type
    auto funcDecl = std::make_unique<FuncDecl>("signup_ok",std::move(paramTypes), std::move(tupleType));

    // Create function parameters
    auto params = std::vector<std::unique_ptr<Decl>>();
    params.push_back(std::make_unique<Decl>(
        "username",
        std::make_unique<TypeConst>("string")));
    params.push_back(std::make_unique<Decl>(
        "password",
        std::make_unique<TypeConst>("string")));

    // Create return tuple type for the function declaration
    auto returnTupleElements = std::vector<std::unique_ptr<TypeExpr>>();
    returnTupleElements.push_back(std::make_unique<TypeConst>("Token"));
    returnTupleElements.push_back(std::make_unique<TypeConst>("HttpResponse"));
    auto returnTupleType = std::make_unique<TupleType>(std::move(returnTupleElements));

    // Create the function declaration
    FuncDecl signupFunc(
        "signup",
        std::move(params),
        std::make_pair(HTTPResponseCode::CREATED_201, std::move(returnTupleType)));

    // Assertions
    assert(signupFunc.name == "signup");
    assert(signupFunc.params.size() == 2);
    assert(signupFunc.returnType.first == HTTPResponseCode::CREATED_201);
    assert(dynamic_cast<TupleType *>(signupFunc.returnType.second.get()) != nullptr);

    std::cout << "Signup Function test passed" << std::endl;
}
void testAPI()
{
    std::cout << "Testing API..." << std::endl;

    // Create precondition
    auto pre = std::make_unique<Var>("isAuthenticated");

    // Create function call with arguments
    std::vector<std::unique_ptr<Expr>> args;
    args.push_back(std::make_unique<Num>(1));
    args.push_back(std::make_unique<Var>("userId"));
    auto call = std::make_unique<FuncCall>("getUserData", std::move(args));

    // Create response
    auto postExpr = std::make_unique<Var>("userData");
    auto response = std::make_pair(HTTPResponseCode::OK_200, std::move(postExpr));

    API api(std::move(pre), std::move(call), std::move(response));

    assert(api.pre != nullptr);
    assert(api.call != nullptr);
    assert(api.response.first == HTTPResponseCode::OK_200);

    std::cout << "API test passed" << std::endl;
}

void testSpec()
{
    std::cout << "Testing Spec..." << std::endl;

    // Create globals
    std::vector<std::unique_ptr<Decl>> globals;
    globals.push_back(std::make_unique<Decl>("globalVar", createSampleTypeExpr()));

    // Create types
    std::vector<std::unique_ptr<TypeDecl>> types;
    std::vector<std::unique_ptr<Decl>> fields;
    fields.push_back(std::make_unique<Decl>("field1", createSampleTypeExpr()));
    types.push_back(std::make_unique<RecordDecl>("TestRecord", std::move(fields)));

    // Create init
    auto init = std::make_unique<Init>("initVar", createSampleExpr());

    // Create functions
    std::vector<std::unique_ptr<FuncDecl>> functions;
    std::vector<std::unique_ptr<Decl>> params;
    params.push_back(std::make_unique<Decl>("param1", createSampleTypeExpr()));
    auto returnType = std::make_pair(HTTPResponseCode::OK_200, createSampleTypeExpr());
    functions.push_back(std::make_unique<FuncDecl>("testFunc", std::move(params), std::move(returnType)));

    // Create blocks (placeholder)
    std::vector<std::unique_ptr<Block>> blocks;
    blocks.push_back(std::make_unique<Block>());

    Spec spec(std::move(globals), std::move(types), std::move(init),
              std::move(functions), std::move(blocks));

    assert(spec.globals.size() == 1);
    assert(spec.types.size() == 1);
    assert(spec.init != nullptr);
    assert(spec.functions.size() == 1);
    assert(spec.blocks.size() == 1);

    std::cout << "Spec test passed" << std::endl;
}

// Complex test that builds a more realistic AST
void testComplexExample()
{
    std::cout << "Testing complex example..." << std::endl;

    // Create a function type: (int, string) -> bool
    std::vector<std::unique_ptr<TypeExpr>> paramTypes;
    paramTypes.push_back(std::make_unique<TypeConst>("int"));
    paramTypes.push_back(std::make_unique<TypeConst>("string"));
    auto returnType = std::make_unique<TypeConst>("bool");
    auto funcType = std::make_unique<FuncType>(std::move(paramTypes), std::move(returnType));

    // Create a map type: Map<string, int>
    auto mapType = std::make_unique<MapType>(
        std::make_unique<TypeConst>("string"),
        std::make_unique<TypeConst>("int"));

    // Create a function call expression
    std::vector<std::unique_ptr<Expr>> callArgs;
    callArgs.push_back(std::make_unique<Num>(123));
    callArgs.push_back(std::make_unique<Var>("username"));
    auto funcCall = std::make_unique<FuncCall>("validateUser", std::move(callArgs));

    // Create a map expression
    auto mapExpr = std::make_unique<Map>(
        std::make_pair(
            std::make_unique<Var>("key"),
            std::make_unique<Num>(42)));

    std::cout << "Complex example test passed" << std::endl;
}

int main()
{
    try
    {
        testSignup();
        testDecl();
        testFuncType();
        testAPI();
        testSpec();
        testComplexExample();

        std::cout << "\nAll tests passed successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}