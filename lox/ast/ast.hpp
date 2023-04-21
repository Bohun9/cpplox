#pragma once
#include <bits/stdc++.h>
#include "../lexer/token.hpp"

struct BinaryExpr;
struct UnaryExpr;
struct LiteralExpr;
struct GroupingExpr;

// C++ does not support virtual template functions :)
struct VisitorExpr {
    virtual void visitBinaryExpr(std::shared_ptr<BinaryExpr>) = 0;
    virtual void visitUnaryExpr(std::shared_ptr<UnaryExpr>) = 0;
    virtual void visitLiteralExpr(std::shared_ptr<LiteralExpr>) = 0;
    virtual void visitGroupingExpr(std::shared_ptr<GroupingExpr>) = 0;
};

struct Expr {
    virtual void accept(VisitorExpr&) = 0;
};

struct BinaryExpr : public std::enable_shared_from_this<BinaryExpr>, Expr {
    std::shared_ptr<Expr> lhs;
    Token op;
    std::shared_ptr<Expr> rhs;

    BinaryExpr(std::shared_ptr<Expr> lhs, Token op, std::shared_ptr<Expr> rhs) : lhs(lhs), op(op), rhs(rhs) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitBinaryExpr(shared_from_this());
    };
};

struct UnaryExpr : public std::enable_shared_from_this<UnaryExpr>, Expr {
    Token op;
    std::shared_ptr<Expr> expr;

    UnaryExpr(Token op, std::shared_ptr<Expr> expr) : op(op), expr(expr) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitUnaryExpr(shared_from_this());
    };
};

struct LiteralExpr : public std::enable_shared_from_this<LiteralExpr>, Expr {
    std::any value;

    LiteralExpr(std::any value) : value(value) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitLiteralExpr(shared_from_this());
    };
};

struct GroupingExpr : public std::enable_shared_from_this<GroupingExpr>, Expr {
    std::shared_ptr<Expr> expr;

    GroupingExpr(std::shared_ptr<Expr> expr) : expr(expr) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitGroupingExpr(shared_from_this());
    };
};

