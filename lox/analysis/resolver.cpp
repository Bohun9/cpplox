#include "resolver.hpp"

Resolver::Resolver(Interpreter &Interpreter, ErrorHandler &errorHandler) : interpreter(Interpreter), errorHandler(errorHandler) {}

void Resolver::resolve(std::shared_ptr<Expr> expr) {
    expr->accept(*this);
}

void Resolver::resolve(std::shared_ptr<Stmt> stmt) {
    stmt->accept(*this);
}

void Resolver::resolve(std::vector<std::shared_ptr<Stmt>> stmts) {
    for (auto statement : stmts) {
        resolve(statement);
    }
}

void Resolver::beginScope() {
    scopes.push_back(std::make_shared<std::map<std::string, bool>>());
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(Token name) {
    if (!scopes.empty()) {
        if (scopes.back()->count(name.lexeme)) {
            errorHandler.error(name, "Variable redefined in local scope.");
        }
        (*scopes.back())[name.lexeme] = false;
    }
}

void Resolver::define(Token name) {
    if (!scopes.empty()) {
        (*scopes.back())[name.lexeme] = true;
    }
}

void Resolver::resolveLocal(std::shared_ptr<Expr> expr, Token name) {
    for (int i = 0; i < int(scopes.size()); i++) {
        if (scopes[scopes.size() - 1 - i]->count(name.lexeme)) {
            interpreter.resolve(expr, i);
        }
    }
}

void Resolver::visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) {}
void Resolver::visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) { resolve(expr->expr); }
void Resolver::visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) { resolve(expr->lhs); resolve(expr->rhs); }
void Resolver::visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) { resolve(expr->lhs); resolve(expr->rhs); }
void Resolver::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) { resolve(expr->expr); }

void Resolver::visitVariableExpr(std::shared_ptr<VariableExpr> expr) {
    if (!scopes.empty() && scopes.back()->count(expr->name.lexeme) && (*(scopes.back()))[expr->name.lexeme] == false) {
        errorHandler.error(expr->name, "Can't read local variable in its own initializer.");
    }
    resolveLocal(expr, expr->name);
}

void Resolver::visitAssignmentExpr(std::shared_ptr<AssignmentExpr> expr) {
    resolveLocal(expr, expr->name);
    resolve(expr->expr);
}

void Resolver::visitCallExpr(std::shared_ptr<CallExpr> expr) {
    resolve(expr->callee);
    for (auto argument : expr->arguments) {
        resolve(argument);
    }
}

void Resolver::visitGetExpr(std::shared_ptr<GetExpr> expr) {
    resolve(expr->object);
}

void Resolver::visitSetExpr(std::shared_ptr<SetExpr> expr) {
    resolve(expr->object);
    resolve(expr->value);
}

void Resolver::visitThisExpr(std::shared_ptr<ThisExpr> expr) {
    resolveLocal(expr, expr->keyword);
}

void Resolver::visitSuperExpr(std::shared_ptr<SuperExpr> expr) {
    resolveLocal(expr, expr->keyword);
}

void Resolver::visitBreakStmt(std::shared_ptr<BreakStmt> stmt) {}
void Resolver::visitContinueStmt(std::shared_ptr<ContinueStmt> stmt) {}
void Resolver::visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) { resolve(stmt->expr); }
void Resolver::visitPrintStmt(std::shared_ptr<PrintStmt> stmt) { resolve(stmt->expr); }

void Resolver::visitVarStmt(std::shared_ptr<VarStmt> expr) {
    declare(expr->name);
    if (expr->initializer != nullptr) {
        resolve(expr->initializer);
    }
    define(expr->name);
}

void Resolver::visitBlockStmt(std::shared_ptr<BlockStmt> stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
}

void Resolver::visitIfStmt(std::shared_ptr<IfStmt> stmt) {
    resolve(stmt->guard);
    resolve(stmt->then);
    if (stmt->elsee != nullptr) {
        resolve(stmt->elsee);
    }
}

void Resolver::visitWhileStmt(std::shared_ptr<WhileStmt> stmt) {
    resolve(stmt->cond);
    resolve(stmt->body);
}

void Resolver::visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) {
    declare(stmt->name);
    define(stmt->name);
    beginScope();
    for (auto parameter : stmt->parameters) {
        declare(parameter);
        define(parameter);
    }
    resolve(stmt->body);
    endScope();
}

void Resolver::visitClassStmt(std::shared_ptr<ClassStmt> stmt) {
    declare(stmt->name);
    define(stmt->name);

    if (stmt->superclass && stmt->name.lexeme == stmt->superclass->name.lexeme) {
        errorHandler.error(stmt->superclass->name, "A class can't inherit from itself.");
    }

    if (stmt->superclass) {
        resolve(stmt->superclass);
    }

    if (stmt->superclass) {
        beginScope();
        (*scopes.back())["super"] = true;
    }

    beginScope();
    (*scopes.back())["this"] = true;
    for (auto method : stmt->methods) {
        resolve(method);
    }
    endScope();

    if (stmt->superclass) {
        endScope();
    }
}

void Resolver::visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) {
    if (stmt->expr != nullptr) {
        resolve(stmt->expr);
    }
}

