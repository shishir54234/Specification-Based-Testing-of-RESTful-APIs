// #include <cassert>
// #include <iostream>
// #include "../ast.hpp"      // Your AST definitions
// #include "../ASTVis.hpp"   // Your visitor interface
// #include "../CloneVisitor.hpp" // The header where you define your CloneVisitor

// // Helper function to create a simple Program AST.
// std::unique_ptr<Program> createSampleAST() {
//     // Create a simple declaration, e.g. a variable declaration.
//     auto declType = std::make_unique<TypeConst>("int");
//     auto decl = std::make_unique<Decl>("myVar", std::move(declType));

//     // Create a statement: assignment: myVar = 42;
//     auto varNode = std::make_unique<Var>("myVar");
//     auto numNode = std::make_unique<Num>(42);
//     auto assignStmt = std::make_unique<Assign>(std::move(varNode), std::move(numNode));
    
//     // Build the program AST with one declaration and one statement.
//     std::vector<std::unique_ptr<Decl>> declarations;
//     declarations.push_back(std::move(decl));
    
//     std::vector<std::unique_ptr<Stmt>> statements;
//     statements.push_back(std::move(assignStmt));
    
//     return std::make_unique<Program>(std::move(statements), std::move(declarations));
// }

// int main() {
//     // 1. Create the original AST.
//     auto originalAST = createSampleAST();

//     // (Optional) Print or inspect originalAST if you have a printer visitor.
//     // e.g., PrinterVisitor printer;
//     // originalAST->accept(printer);
    
//     // 2. Create and use the clone visitor to clone the AST.
//     CloneVisitor cloneVisitor;
//     originalAST->accept(cloneVisitor);
    
//     // Retrieve the cloned AST from the visitor.
//     // For example, if your CloneVisitor has a public member "clonedProgram":
//     std::unique_ptr<Program> clonedAST = std::move(cloneVisitor.clonedProgram);
    
//     // 3. Validate the clone.
//     // For basic testing, you might compare:
//     //    - The data stored in each node (e.g. variable names, literal values) is equal.
//     //    - The pointers are distinct (deep copy).
    
//     // Example assertions:
//     // Check that the declarations exist.
//     assert(originalAST->declarations.size() == clonedAST->declarations.size());
//     // Compare the name in the first declaration.
//     assert(originalAST->declarations[0]->name == clonedAST->declarations[0]->name);
    
//     // Check that the statements exist.
//     assert(originalAST->statements.size() == clonedAST->statements.size());
//     // For the assignment statement, cast to Assign and check its parts.
//     auto *origAssign = dynamic_cast<Assign*>(originalAST->statements[0].get());
//     auto *cloneAssign = dynamic_cast<Assign*>(clonedAST->statements[0].get());
//     assert(origAssign != nullptr && cloneAssign != nullptr);
    
//     // Check the variable names.
//     assert(origAssign->left->name == cloneAssign->left->name);
//     // Check the numerical literal.
//     auto *origNum = dynamic_cast<Num*>(origAssign->right.get());
//     auto *cloneNum = dynamic_cast<Num*>(cloneAssign->right.get());
//     assert(origNum != nullptr && cloneNum != nullptr);
//     assert(origNum->value == cloneNum->value);
    
//     // Ensure they are different objects (deep clone).
//     assert(origAssign->left.get() != cloneAssign->left.get());
//     assert(origAssign->right.get() != cloneAssign->right.get());
    
//     std::cout << "CloneVisitor test passed successfully!" << std::endl;
//     return 0;
// }
