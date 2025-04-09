// test_decl_clone.cpp

#include "../ast.hpp"
#include <cassert>
#include <iostream>

int main() {
   
    auto originalDecl = std::make_unique<Decl>("test_decl", std::make_unique<TypeConst>("int"));

    auto clonedDecl = originalDecl->clone();

    assert(clonedDecl->name == originalDecl->name);

    auto originalType = dynamic_cast<TypeConst*>(originalDecl->type.get());
    auto clonedType   = dynamic_cast<TypeConst*>(clonedDecl->type.get());
    assert(originalType != nullptr && clonedType != nullptr);
    assert(originalType->name == clonedType->name);

    assert(originalDecl->type.get() != clonedDecl->type.get());

    std::cout << "Decl clone test passed." << std::endl;
    return 0;
}
