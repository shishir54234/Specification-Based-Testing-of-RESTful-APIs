#include <stack>
#include <memory>
#include "ASTVis.hpp"
#include "ast.hpp" // include your AST node definitions

using ClonePtr = std::unique_ptr<void>; // This is conceptual; you'll likely need to use type erasure or templates.

// A utility to store cloned objects.
// One common approach is to make the visitor templated or have separate methods for each node type.
// For demonstration, we’ll use a simple class that stores a std::unique_ptr<Base> where Base is the base class for AST nodes,
// but note that your AST nodes do not share a single common base type.

// Instead, we can have a member variable for each node type clone. 
// Alternatively, use a "Any" type (or boost::any) to wrap the clone pointers.
// For simplicity, assume we work on separate visitor methods that build the clone and then store it in a member variable.

class CloneVisitor : public ASTVisitor {
public:
    // Cloned nodes for the most recent visit.
    // Since AST nodes have different types and no common base (except for a few cases),
    // you might create different getters for different nodes or use templates.
    // One common approach is to have a class hierarchy with a common cloneable base.
    // Here, we assume that every AST node is handled correctly by the visitor functions.
    
    // Example for Program clone result:
    std::unique_ptr<Program> clonedProgram;

    // For temporary storage in a stack-like manner:
    std::stack<std::unique_ptr<void>> cloneStack; // using void* is not type safe, so consider type erasure or per-node member.

    // Override visitor methods. For each type, build the clone from visited children.
    
    // Type Expression visitors
    virtual void visit(const TypeConst &node) override {
        auto clone = std::make_unique<TypeConst>(node.name);
        // After creating the clone, store it.
        // You may push it on the cloneStack or set a temporary member.
        lastClone = std::move(clone);
    }
    
    virtual void visit(const FuncType &node) override {
        // Clone each parameter recursively
        std::vector<std::unique_ptr<TypeExpr>> clonedParams;
        for (const auto& param : node.params) {
            param->accept(*this);
            // lastClone now holds the clone for this parameter.
            // The underlying type is known to be a TypeExpr. 
            clonedParams.push_back(std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release())));
        }
        
        // Clone the return type
        std::unique_ptr<TypeExpr> clonedReturn;
        if (node.returnType) {
            node.returnType->accept(*this);
            clonedReturn = std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release()));
        }
        
        auto clone = std::make_unique<FuncType>(std::move(clonedParams), std::move(clonedReturn));
        lastClone = std::move(clone);
    }

    virtual void visit(const MapType &node) override {
        node.domain->accept(*this);
        auto clonedDomain = std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release()));
        node.range->accept(*this);
        auto clonedRange = std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release()));
        
        auto clone = std::make_unique<MapType>(std::move(clonedDomain), std::move(clonedRange));
        lastClone = std::move(clone);
    }

    virtual void visit(const TupleType &node) override {
        std::vector<std::unique_ptr<TypeExpr>> clonedElements;
        for (const auto &elem : node.elements) {
            elem->accept(*this);
            clonedElements.push_back(std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release())));
        }
        auto clone = std::make_unique<TupleType>(std::move(clonedElements));
        lastClone = std::move(clone);
    }

    virtual void visit(const SetType &node) override {
        node.elementType->accept(*this);
        auto clonedElementType = std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release()));
        auto clone = std::make_unique<SetType>(std::move(clonedElementType));
        lastClone = std::move(clone);
    }

    // Expression visitors
    virtual void visit(const Var &node) override {
        auto clone = std::make_unique<Var>(node.name);
        lastClone = std::move(clone);
    }

    virtual void visit(const FuncCall &node) override {
        std::vector<std::unique_ptr<Expr>> clonedArgs;
        for (const auto &arg : node.args) {
            arg->accept(*this);
            clonedArgs.push_back(std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release())));
        }
        auto clone = std::make_unique<FuncCall>(node.name, std::move(clonedArgs));
        lastClone = std::move(clone);
    }

    virtual void visit(const Num &node) override {
        auto clone = std::make_unique<Num>(node.value);
        lastClone = std::move(clone);
    }

    virtual void visit(const String &node) override {
        auto clone = std::make_unique<String>(node.value);
        lastClone = std::move(clone);
    }

    virtual void visit(const Set &node) override {
        std::vector<std::unique_ptr<Expr>> clonedElements;
        for (const auto &element : node.elements) {
            element->accept(*this);
            clonedElements.push_back(std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release())));
        }
        auto clone = std::make_unique<Set>(std::move(clonedElements));
        lastClone = std::move(clone);
    }

    virtual void visit(const Map &node) override {
        // Clone all key-value pairs
        std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>> clonedValue;
        for (const auto &pair : node.value) {
            // Clone the key (Var)
            pair.first->accept(*this);
            auto clonedKey = std::unique_ptr<Var>(static_cast<Var*>(lastClone.release()));
            // Clone the expression value
            pair.second->accept(*this);
            auto clonedExpr = std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release()));
            
            clonedValue.emplace_back(std::move(clonedKey), std::move(clonedExpr));
        }
        auto clone = std::make_unique<Map>(std::move(clonedValue));
        lastClone = std::move(clone);
    }

    virtual void visit(const Tuple &node) override {
        std::vector<std::unique_ptr<Expr>> clonedExprs;
        for (const auto &expr : node.expr) {
            expr->accept(*this);
            clonedExprs.push_back(std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release())));
        }
        auto clone = std::make_unique<Tuple>(std::move(clonedExprs));
        lastClone = std::move(clone);
    }

    // Declaration visitors
    virtual void visit(const Decl &node) override {
        // Clone the type expression associated with the declaration
        node.type->accept(*this);
        auto clonedType = std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release()));
        auto clone = std::make_unique<Decl>(node.name, std::move(clonedType));
        lastClone = std::move(clone);
    }

    virtual void visit(const FuncDecl &node) override {
        std::vector<std::unique_ptr<TypeExpr>> clonedParams;
        for (const auto &param : node.params) {
            param->accept(*this);
            clonedParams.push_back(std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release())));
        }
        // Clone the return type vector.
        std::vector<std::unique_ptr<TypeExpr>> clonedReturnTypes;
        for (const auto &typeExpr : node.returnType.second) {
            typeExpr->accept(*this);
            clonedReturnTypes.push_back(std::unique_ptr<TypeExpr>(static_cast<TypeExpr*>(lastClone.release())));
        }
        auto clone = std::make_unique<FuncDecl>(node.name, std::move(clonedParams), 
            std::make_pair(node.returnType.first, std::move(clonedReturnTypes)));
        lastClone = std::move(clone);
    }

    // API and Related visitors
    virtual void visit(const APIcall &node) override {
        // Clone the function call
        node.call->accept(*this);
        auto clonedCall = std::unique_ptr<FuncCall>(static_cast<FuncCall*>(lastClone.release()));
        // Clone the Response object. Since Response contains an Expr, clone that:
        node.response.expr->accept(*this);
        auto clonedExpr = std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release()));
        Response clonedResponse(node.response.code, std::move(clonedExpr));

        auto clone = std::make_unique<APIcall>(std::move(clonedCall), std::move(clonedResponse));
        lastClone = std::move(clone);
    }

    virtual void visit(const API &node) override {
        // Clone precondition expression:
        node.pre->accept(*this);
        auto clonedPre = std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release()));
        // Clone the API call:
        node.call->accept(*this);
        auto clonedCall = std::unique_ptr<APIcall>(static_cast<APIcall*>(lastClone.release()));
        // Clone the response:
        node.response.expr->accept(*this);
        auto clonedRespExpr = std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release()));
        Response clonedResponse(node.response.code, std::move(clonedRespExpr));
        
        auto clone = std::make_unique<API>(std::move(clonedPre), std::move(clonedCall), std::move(clonedResponse));
        lastClone = std::move(clone);
    }

    virtual void visit(const Response &node) override {
        node.expr->accept(*this);
        auto clonedExpr = std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release()));
        auto clone = std::make_unique<Response>(node.code, std::move(clonedExpr));
        lastClone = std::move(clone);
    }

    // Initialization visitor
    virtual void visit(const Init &node) override {
        node.expr->accept(*this);
        auto clonedExpr = std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release()));
        auto clone = std::make_unique<Init>(node.varName, std::move(clonedExpr));
        lastClone = std::move(clone);
    }

    // Specification visitor (if you’re using it)
    virtual void visit(const Spec &node) override {
        // Assuming Spec contains globals, init, functions, blocks.
        // This depends on your actual AST design.
    }

    // Statement visitors
    virtual void visit(const Assign &node) override {
        node.left->accept(*this);
        auto clonedLeft = std::unique_ptr<Var>(static_cast<Var*>(lastClone.release()));
        node.right->accept(*this);
        auto clonedRight = std::unique_ptr<Expr>(static_cast<Expr*>(lastClone.release()));
        auto clone = std::make_unique<Assign>(std::move(clonedLeft), std::move(clonedRight));
        lastClone = std::move(clone);
    }

    virtual void visit(const FuncCallStmt &node) override {
        node.call->accept(*this);
        auto clonedCall = std::unique_ptr<FuncCall>(static_cast<FuncCall*>(lastClone.release()));
        auto clone = std::make_unique<FuncCallStmt>(std::move(clonedCall));
        lastClone = std::move(clone);
    }

    virtual void visit(const Program &node) override {
        // Clone declarations
        std::vector<std::unique_ptr<Decl>> clonedDecls;
        for (const auto &decl : node.declarations) {
            decl->accept(*this);
            clonedDecls.push_back(std::unique_ptr<Decl>(static_cast<Decl*>(lastClone.release())));
        }
        // Clone statements
        std::vector<std::unique_ptr<Stmt>> clonedStmts;
        for (const auto &stmt : node.statements) {
            stmt->accept(*this);
            clonedStmts.push_back(std::unique_ptr<Stmt>(static_cast<Stmt*>(lastClone.release())));
        }
        auto clone = std::make_unique<Program>(std::move(clonedStmts), std::move(clonedDecls));
        lastClone = std::move(clone);
        // Optionally, store in a public member
        clonedProgram = std::unique_ptr<Program>(static_cast<Program*>(lastClone.get()));
    }

    // Utility: Returns the last cloned node pointer.
    // In a more type-safe design, you might template this.
    std::unique_ptr<void> takeClone() {
        return std::move(lastClone);
    }
    
private:
    // A temporary storage for the last clone produced by a visitor method.
    std::unique_ptr<void> lastClone;
};
