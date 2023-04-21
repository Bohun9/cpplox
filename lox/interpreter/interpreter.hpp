#pragma once

#include <bits/stdc++.h>

#include "environment.hpp"
#include "../ast/ast.hpp"
#include "../error/error_handler.hpp"

struct Interpreter : VisitorExpr, VisitorStmt {
    std::shared_ptr<Environment> environment;
    ErrorHandler &errorHandler;
    std::vector<std::any> stack;

    Interpreter(ErrorHandler &errorHandler) : errorHandler(errorHandler), stack({}), environment(std::make_shared<Environment>(nullptr)) {}

    void interpret(std::vector<std::shared_ptr<Stmt>> statements);
    std::any evaluate(std::shared_ptr<Expr> expr);
    void execute(std::shared_ptr<Stmt> expr);

    void Return(std::any v);
    std::string stringify(std::any v);

    void visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
    void visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
    void visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
    void visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
    void visitVariableExpr(std::shared_ptr<VariableExpr> expr) override;
    void visitAssignmentExpr(std::shared_ptr<AssignmentExpr> expr) override;

    void visitExpressionStmt(std::shared_ptr<ExpressionStmt> expr) override;
    void visitPrintStmt(std::shared_ptr<PrintStmt> expr) override;
    void visitVarStmt(std::shared_ptr<VarStmt> expr) override;
    void visitBlockStmt(std::shared_ptr<BlockStmt>) override;

    void checkNumberOperand(Token token, std::any v);
    void checkNumberOperands(Token token, std::any lhs, std::any rhs);
    void checkBooleanOperands(Token op, std::any lhs, std::any rhs);

    bool isEqual(std::any lhs, std::any rhs);
    bool isTruthy(std::any v);
};

