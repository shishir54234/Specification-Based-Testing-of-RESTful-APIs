#ifndef IMA_HPP
#define IMA_HPP

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <memory>
#include "ast.hpp"       
#include "PrintVisitor.hpp"   

using namespace std;

/* 
 * A simple environment (tau) mapping formal parameter names (from the spec)
 * to actual variable names (from the client code).
 */
using Env = map<string, string>;

class SymbolTable {
public:
    vector<SymbolTable*> children;
    SymbolTable* par = nullptr;
    set<Var> symtable;
    bool exists(Var v) {
        return (symtable.find(v) != symtable.end());
    }
    string to_string() {
        string s;
        for (auto &var : symtable) {
            s += var.name + " ";
        }
        return s;
    }
};

/**
 * convert1:
 * Traverses an expression and, if a variable exists in the given symbol table,
 * appends a suffix (provided in 'add') to its name.
 */
unique_ptr<Expr> convert1(unique_ptr<Expr> &expr, SymbolTable *symtable, const string &add) {
    if (!expr) return nullptr;
    if (auto var = dynamic_cast<Var*>(expr.get())) {
        if (symtable->exists(*var))
            return make_unique<Var>(var->name + add);
        return make_unique<Var>(var->name);
    }
    if (auto func = dynamic_cast<FuncCall*>(expr.get())) {
        vector<unique_ptr<Expr>> args;
        for (auto &arg : func->args) {
            args.push_back(convert1(arg, symtable, add));
        }
        return make_unique<FuncCall>(func->name, move(args));
    }
    if (auto num = dynamic_cast<Num*>(expr.get())) {
        return make_unique<Num>(num->value);
    }
    if (auto set = dynamic_cast<Set*>(expr.get())) {
        vector<unique_ptr<Expr>> elements;
        for (auto &element : set->elements) {
            elements.push_back(convert1(element, symtable, add));
        }
        return make_unique<Set>(move(elements));
    }
    if (auto mapExpr = dynamic_cast<Map*>(expr.get())) {
        vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> ret;
        for (size_t i = 0; i < mapExpr->value.size(); i++) {
            auto key = convert1(reinterpret_cast<unique_ptr<Expr>&>(mapExpr->value[i].first), symtable, add);
            auto value = convert1(mapExpr->value[i].second, symtable, add);
            ret.push_back(make_pair(move(reinterpret_cast<unique_ptr<Var>&>(key)), move(value)));
        }
        return make_unique<Map>(move(ret));
    }
    if (auto tuple = dynamic_cast<Tuple*>(expr.get())) {
        vector<unique_ptr<Expr>> exprs;
        for (auto &exp : tuple->expr) {
            exprs.push_back(convert1(exp, symtable, add));
        }
        return make_unique<Tuple>(move(exprs));
    }
    throw runtime_error("Unknown expression type in convert function");
}

/**
 * makeStmt:
 * Generates a function call statement (here, for an input() call) based on the given expression.
 */
unique_ptr<FuncCallStmt> makeStmt(unique_ptr<Expr> expr) {
    auto call = dynamic_cast<Var*>(expr.get());
    if (call) {
        vector<unique_ptr<Expr>> args;
        args.push_back(make_unique<Var>(call->name));
        auto h = make_unique<FuncCall>("input", move(args));
        return make_unique<FuncCallStmt>(move(h));
    }
    return nullptr;
}

/**
 * getInputVars:
 * Traverses an expression to extract all variables (via convert1) that are inputs.
 */
void getInputVars(unique_ptr<Expr> &expr, vector<unique_ptr<Expr>> &InputVariables, string toadd, SymbolTable *symtable) {
    if (auto var = dynamic_cast<Var*>(expr.get())) {
        InputVariables.push_back(convert1(expr, symtable, toadd));
        return;
    }
    if (auto func = dynamic_cast<FuncCall*>(expr.get())) {
        for (auto &arg : func->args) {
            getInputVars(arg, InputVariables, toadd, symtable);
        }
    }
    if (auto set = dynamic_cast<Set*>(expr.get())) {
        for (auto &element : set->elements) {
            getInputVars(element, InputVariables, toadd, symtable);
        }
    }
    if (auto tuple = dynamic_cast<Tuple*>(expr.get())) {
        for (auto &exp : tuple->expr) {
            getInputVars(exp, InputVariables, toadd, symtable);
        }
    }
    if (auto mapExpr = dynamic_cast<Map*>(expr.get())) {
        for (auto &element : mapExpr->value) {
            getInputVars(reinterpret_cast<unique_ptr<Expr>&>(element.first), InputVariables, toadd, symtable);
            getInputVars(element.second, InputVariables, toadd, symtable);
        }
    }
    if (dynamic_cast<Num*>(expr.get())) {
        return;
    }
}

/**
 * --- Environment for renaming pre/post conditions ---
 * renameExprWithMap recursively traverses an expression and renames variables according to the environment.
 */
unique_ptr<Expr> renameExprWithMap(const Expr* expr, const Env &env) {
    if (!expr) return nullptr;
    if (auto v = dynamic_cast<const Var*>(expr)) {
        auto it = env.find(v->name);
        string newName = (it != env.end()) ? it->second : v->name;
        return make_unique<Var>(newName);
    }
    else if (auto num = dynamic_cast<const Num*>(expr)) {
        return make_unique<Num>(num->value);
    }
    else if (auto fc = dynamic_cast<const FuncCall*>(expr)) {
        vector<unique_ptr<Expr>> newArgs;
        newArgs.reserve(fc->args.size());
        for (auto &argPtr : fc->args) {
            newArgs.push_back(renameExprWithMap(argPtr.get(), env));
        }
        return make_unique<FuncCall>(fc->name, move(newArgs));
    }
    // else if (auto poly = dynamic_cast<const PolymorphicFuncCall*>(expr)) {
    //     vector<unique_ptr<Expr>> newArgs;
    //     newArgs.reserve(poly->args.size());
    //     for (auto &argPtr : poly->args) {
    //         newArgs.push_back(renameExprWithMap(argPtr.get(), env));
    //     }
    //     // Deep-copy typeArgs using clone()
    //     vector<unique_ptr<TypeExpr>> newTypeArgs;
    //     newTypeArgs.reserve(poly->typeArgs.size());
    //     for (auto &typeArg : poly->typeArgs) {
    //         newTypeArgs.push_back(typeArg->clone());
    //     }
    //     return make_unique<PolymorphicFuncCall>(poly->name, move(newTypeArgs), move(newArgs));
    // }
    else if (auto s = dynamic_cast<const Set*>(expr)) {
        vector<unique_ptr<Expr>> newElems;
        newElems.reserve(s->elements.size());
        for (auto &e : s->elements) {
            newElems.push_back(renameExprWithMap(e.get(), env));
        }
        return make_unique<Set>(move(newElems));
    }
    else if (auto m = dynamic_cast<const Map*>(expr)) {
        vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> newPairs;
        newPairs.reserve(m->value.size());
        for (auto &pair : m->value) {
            auto newKeyExpr = renameExprWithMap(pair.first.get(), env);
            auto newValExpr = renameExprWithMap(pair.second.get(), env);
            auto newKeyVar = dynamic_cast<Var*>(newKeyExpr.release());
            newPairs.push_back({make_unique<Var>(newKeyVar->name), move(newValExpr)});
        }
        return make_unique<Map>(move(newPairs));
    }
    else if (auto t = dynamic_cast<const Tuple*>(expr)) {
        vector<unique_ptr<Expr>> newExprs;
        newExprs.reserve(t->expr.size());
        for (auto &sub : t->expr) {
            newExprs.push_back(renameExprWithMap(sub.get(), env));
        }
        return make_unique<Tuple>(move(newExprs));
    }
    else if (auto string = dynamic_cast<const String*>(expr)) {
        return make_unique<String>(string->value);
    }
    throw runtime_error("renameExprWithMap: Unhandled expression type");
}

/**
 * cloneStmt:
 * Makes a copy of a statement. (Here we handle only Assign and FuncCallStmt.)
 */
unique_ptr<Stmt> cloneStmt(const Stmt* s) {
    if (!s) return nullptr;
    if (auto a = dynamic_cast<const Assign*>(s)) {
        auto leftVar = make_unique<Var>(a->left->name);
        auto rightExpr = renameExprWithMap(a->right.get(), {});
        return make_unique<Assign>(move(leftVar), move(rightExpr));
    }
    else if (auto fc = dynamic_cast<const FuncCallStmt*>(s)) {
        auto oldCall = fc->call.get();
        vector<unique_ptr<Expr>> newArgs;
        newArgs.reserve(oldCall->args.size());
        for (auto &arg : oldCall->args) {
            newArgs.push_back(renameExprWithMap(arg.get(), {}));
        }
        auto newCall = make_unique<FuncCall>(oldCall->name, move(newArgs));
        return make_unique<FuncCallStmt>(move(newCall));
    }
    throw runtime_error("cloneStmt: Unhandled statement type");
}

/**
 * getMutatedVars:
 * Placeholder for detecting system variables mutated by an API call.
 * In this demo, we pretend that variables "session", "token", and "a" are candidates,
 * but formal parameters (e.g. "a" if it is in the formal list) are skipped.
 */
vector<string> getMutatedVars(const API &block,
                              const vector<string> &formalParams,
                              const Env &tau) {
    vector<string> mutated;
    vector<string> foundInPost = {};
    set<string> paramSet(formalParams.begin(), formalParams.end());
    for (auto &name : foundInPost) {
        if (paramSet.find(name) != paramSet.end())
            continue;
        mutated.push_back(name);
    }
    for (auto &m : mutated) {
        auto it = tau.find(m);
        if (it != tau.end())
            m = it->second;
    }
    return mutated;
}

/**
 * createTau:
 * Given the spec's formal parameter names and the client’s actual arguments,
 * creates the mapping tau (formal -> actual).
 */
Env createTau(const vector<string> &formalParams, const vector<string> &actualParams) {
    Env env;
    for (size_t i = 0; i < formalParams.size() && i < actualParams.size(); i++) {
        env[formalParams[i]] = actualParams[i];
    }
    return env;
}


/**
 * IMA (Implicit Mocking Algorithm):
 * Transforms a client program by replacing API calls with a sequence:
 *   assume(pre[τ]), assignments for mutated variables (input()), and assert(post[τ]).
 */


Program IMA(const Program &p, const Spec &spec) {
    vector<unique_ptr<Stmt>> newStmts;
    vector<unique_ptr<Decl>> declarations;

    vector<unique_ptr<Program>> ProgramCodes;

    for (const auto &decl : spec.globals) {
        declarations.push_back(decl->clone());
    }
    for(const auto &decl: p.declarations){
        declarations.push_back(decl->clone());
    }
    for(const auto &inits: spec.init){ //here im assuming i wont be using init of specs again after. 
        std::unique_ptr<Var> var = std::make_unique<Var>(inits->varName);
        std::unique_ptr<Expr> expr = std::move(inits->expr);
        newStmts.push_back(std::make_unique<Assign>(std::move(var), std::move(expr)));

    }

    // Iterate over each statement in the program.
    for (auto &stmtPtr : p.statements) {
        auto fcStmt = dynamic_cast<FuncCallStmt*>(stmtPtr.get());
        if (!fcStmt) {
            newStmts.push_back(cloneStmt(stmtPtr.get()));
            continue;
        }
        const string &funcName = fcStmt->call->name;
        API *matchedBlock = nullptr;
        vector<string> formalParams;
        // Search for a matching API block in the spec.
        for (auto &apiBlock : spec.blocks) {
            if (apiBlock->call->call->name == funcName) {
                matchedBlock = apiBlock.get();
                // Extract formal parameter names from the API block's function call.
                for (auto &argExpr : matchedBlock->call->call->args) {
                    if (auto varArg = dynamic_cast<Var*>(argExpr.get()))
                        formalParams.push_back(varArg->name);
                    else
                        formalParams.push_back("TMP_expr");
                }
                break;
            }
        }
        if (!matchedBlock) {
            newStmts.push_back(cloneStmt(stmtPtr.get()));
            continue;
        }
        // Build the environment tau from formal to actual parameters.
        vector<string> actualParams;
        for (auto &argExpr : fcStmt->call->args) {
            if (auto varArg = dynamic_cast<Var*>(argExpr.get()))
                actualParams.push_back(varArg->name);
            else
                actualParams.push_back("TMP_expr");
        }
        Env tau = createTau(formalParams, actualParams);
        
        // Rename pre- and postconditions using tau.
        auto renamedPre  = renameExprWithMap(matchedBlock->pre.get(), tau);
        auto renamedCallResponse = renameExprWithMap(matchedBlock->call->response.expr.get(), tau);
        // auto renamedBlockResponse = renameExprWithMap(matchedBlock->response.expr.get(), tau);
        
        // Insert assume(pre[τ])
        if(renamedPre)
        {
            vector<unique_ptr<Expr>> assumeArgs;
            assumeArgs.push_back(move(renamedPre));
            auto assumeCall = make_unique<FuncCall>("assume", move(assumeArgs));
            newStmts.push_back(make_unique<FuncCallStmt>(move(assumeCall)));
        }
        
        //print api call as it is
        newStmts.push_back(std::make_unique<FuncCallStmt>(std::move(*fcStmt)));
        // im assuming mutated vars are variables, what if its set or smth
        auto mutatedVars = getMutatedVars(*matchedBlock, formalParams, tau);
        for (auto &mv : mutatedVars) {
            auto leftVar = make_unique<Var>(mv);
            vector<unique_ptr<Expr>> inputArgs;
            auto inputCall = make_unique<FuncCall>("input", move(inputArgs));
            auto assign = make_unique<Assign>(move(leftVar), move(inputCall));
            newStmts.push_back(move(assign));
        }
        
        // Insert assert(post[τ])
        if (renamedCallResponse) {
            vector<unique_ptr<Expr>> assertArgs;
            assertArgs.push_back(move(renamedCallResponse));
            auto assertCall = make_unique<FuncCall>("assert", move(assertArgs));
            newStmts.push_back(make_unique<FuncCallStmt>(move(assertCall)));
        }

        // if (renamedBlockResponse) {
        //     vector<unique_ptr<Expr>> assertArgs;
        //     assertArgs.push_back(move(renamedBlockResponse));
        //     auto assertCall = make_unique<FuncCall>("assert", move(assertArgs));
        //     newStmts.push_back(make_unique<FuncCallStmt>(move(assertCall)));
        // }

        
    }
    return Program(move(newStmts),move(declarations));
}

#endif
