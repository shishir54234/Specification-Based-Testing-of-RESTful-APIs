#ifndef CLONEHELPER_HPP
#define CLONEHELPER_HPP

#include "ast.hpp"
#include <memory>
#include <vector>
#include <string>

namespace cloneHelper {

    // MAP,
    // VAR,
    // STRING,
    // NUM,
    // TUPLE,
    // SET,
    // FUNCTIONCALL_EXPR,
    // POLYMORPHIC_FUNCTIONCALL_EXPR
    // unique_ptr<Expr> &expr

inline std::unique_ptr<Expr> clone(unique_ptr<Expr> &expr) {
    switch(expr.expressionType) {
        case ExpressionType::VAR: {
            const Var* varPtr = dynamic_cast<const Var*>(expr.get());
            if (varPtr) {
                return std::make_unique<Var>(varPtr->name);
            }
            break;
        }
        case ExpressionType::NUM: {
            const Num* numPtr = dynamic_cast<const Num*>(expr.get());
            if (numPtr) {
                return std::make_unique<Num>(numPtr->value);
            }
            break;
        }
        case ExpressionType::STRING: {
            const String* strPtr = dynamic_cast<const String*>(expr.get());
            if (strPtr) {
                return std::make_unique<String>(strPtr->value);
            }
            break;
        }
        case ExpressionType::FUNCTIONCALL_EXPR: {
            const FuncCall* fcPtr = dynamic_cast<const FuncCall*>(expr.get());
            if (fcPtr) {
                std::vector<std::unique_ptr<Expr>> clonedArgs;
                for (const auto &arg : fcPtr->args) {
                    clonedArgs.push_back(clone(*arg));
                }
                return std::make_unique<FuncCall>(fcPtr->name, std::move(clonedArgs));
            }
            break;
        }
        case ExpressionType::MAP: {
            const Map* mpPtr = dynamic_cast<const Map*>(expr.get());
            if(mpPtr) {
                std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>> clonedvalue;
                for(const auto &values: mpPtr->value){
                    const Var* first = clone(values.first);
                    const Expr* second = clone(values.second);

                    clonedvalue.push_back({first,second});
                }

                return std::make_unique<Map>(std::move(clonedvalue));
            }
            break;
        }
        case ExpressionType::SET: {
            const Set* setPtr = dynamic_cast<const Set*>(expr.get());
            if(setPtr){
                std::vector<std::unique_ptr<Expr>> clonedelements;
                for(const auto &value:setPtr->elements){
                    clonedelements.push_back(clone(value));
                }
                return std::make_unique<Set>(std::move(clonedvalue));
            }
            break;
        }
        case ExpressionType::TUPLE: {
            const Tuple* tupplePtr = dynamic_cast<const Tuple*>(expr.get());
            if(tupplePtr){
                std::vector<std::unique_ptr<Expr>> clonedexpr;
                for(const auto &value:tupplePtr->expr){
                    clonedelements.push_back(clone(value));
                }
            }
            break;  
        }
        default:
            break;
    }
    return nullptr;
}


inline std::unique_ptr<FuncCall> clone(const FuncCall &funcCall) {
    std::vector<std::unique_ptr<Expr>> clonedArgs;
    for (const auto &arg : funcCall.args) {
        clonedArgs.push_back(cloneExpr(*arg));
    }
    return std::make_unique<FuncCall>(funcCall.name, std::move(clonedArgs));
}


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
       
        default:
            break;
    }
    return nullptr;
}

} // namespace cloneHelper

#endif // CLONEHELPER_HPP
