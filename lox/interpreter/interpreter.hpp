#pragma once

#include <bits/stdc++.h>

#include "../ast/ast.hpp"
#include "../error/error_handler.hpp"

struct ErrorHandler;

struct RunTimeError {
    Token token;
    std::string message;

    RunTimeError(Token token, std::string message) : token(token), message(message) {}
};

struct Interpreter : VisitorExpr {
    ErrorHandler &errorHandler;
    std::vector<std::any> stack;

    Interpreter(ErrorHandler &errorHandler) : errorHandler(errorHandler), stack({}) {}

    void interpret(std::shared_ptr<Expr> expr);
    std::any evaluate(std::shared_ptr<Expr> expr);
    void Return(std::any v);
    std::string stringify(std::any v);

    void visitLiteralExpr(std::shared_ptr<LiteralExpr> expr);
    void visitGroupingExpr(std::shared_ptr<GroupingExpr> expr);
    void visitBinaryExpr(std::shared_ptr<BinaryExpr> expr);
    void visitUnaryExpr(std::shared_ptr<UnaryExpr> expr);

    void checkNumberOperand(Token token, std::any v);
    void checkNumberOperands(Token token, std::any lhs, std::any rhs);
    void checkBooleanOperands(Token op, std::any lhs, std::any rhs);

    bool isEqual(std::any lhs, std::any rhs);
    bool isTruthy(std::any v);
};

