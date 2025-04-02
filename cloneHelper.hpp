#ifndef CLONEHELPER_HPP
#define CLONEHELPER_HPP

#include "ast.hpp"
#include <memory>
#include <vector>
#include <string>

namespace cloneHelper {

// Helper function to clone an expression based on its dynamic type.
inline std::unique_ptr<Expr> cloneExpr(const Expr &expr) {
    switch(expr.expressionType) {
        case ExpressionType::VAR: {
            const Var* varPtr = dynamic_cast<const Var*>(&expr);
            if (varPtr) {
                return std::make_unique<Var>(varPtr->name);
            }
            break;
        }
        case ExpressionType::NUM: {
            const Num* numPtr = dynamic_cast<const Num*>(&expr);
            if (numPtr) {
                return std::make_unique<Num>(numPtr->value);
            }
            break;
        }
        case ExpressionType::STRING: {
            const String* strPtr = dynamic_cast<const String*>(&expr);
            if (strPtr) {
                return std::make_unique<String>(strPtr->value);
            }
            break;
        }
        case ExpressionType::FUNCTIONCALL_EXPR: {
            const FuncCall* fcPtr = dynamic_cast<const FuncCall*>(&expr);
            if (fcPtr) {
                // Call cloneFuncCall (defined below)
                std::vector<std::unique_ptr<Expr>> clonedArgs;
                for (const auto &arg : fcPtr->args) {
                    clonedArgs.push_back(cloneExpr(*arg));
                }
                return std::make_unique<FuncCall>(fcPtr->name, std::move(clonedArgs));
            }
            break;
        }
        // Add additional cases for other expression types as needed.
        default:
            break;
    }
    return nullptr;
}

// Helper function to clone a function call (used within expressions and statements)
inline std::unique_ptr<FuncCall> cloneFuncCall(const FuncCall &funcCall) {
    std::vector<std::unique_ptr<Expr>> clonedArgs;
    for (const auto &arg : funcCall.args) {
        clonedArgs.push_back(cloneExpr(*arg));
    }
    return std::make_unique<FuncCall>(funcCall.name, std::move(clonedArgs));
}

// Helper function to clone a statement based on its type.
inline std::unique_ptr<Stmt> cloneStmt(const Stmt &stmt) {
    switch(stmt.statementType) {
        case StatementType::ASSIGN: {
            const Assign* assignPtr = dynamic_cast<const Assign*>(&stmt);
            if (assignPtr) {
                // Clone left-hand side (a Var) and right-hand side expression.
                auto leftClone = std::make_unique<Var>(assignPtr->left->name);
                auto rightClone = cloneExpr(*assignPtr->right);
                return std::make_unique<Assign>(std::move(leftClone), std::move(rightClone));
            }
            break;
        }
        case StatementType::FUNCTIONCALL_STMT: {
            const FuncCallStmt* fcStmtPtr = dynamic_cast<const FuncCallStmt*>(&stmt);
            if (fcStmtPtr) {
                auto callClone = cloneFuncCall(*fcStmtPtr->call);
                return std::make_unique<FuncCallStmt>(std::move(callClone));
            }
            break;
        }
        // If you add more statement types (e.g., DECL), add their cloning logic here.
        default:
            break;
    }
    return nullptr;
}

} // namespace cloneHelper

#endif // CLONEHELPER_HPP
