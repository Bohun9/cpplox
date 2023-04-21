#pragma once
#include <bits/stdc++.h>
#include "../lexer/token.hpp"

struct BinaryExpr;
struct UnaryExpr;
struct LiteralExpr;
struct GroupingExpr;
struct VariableExpr;
struct AssignmentExpr;

// C++ does not support virtual template functions :)
struct VisitorExpr {
    virtual void visitBinaryExpr(std::shared_ptr<BinaryExpr>) = 0;
    virtual void visitUnaryExpr(std::shared_ptr<UnaryExpr>) = 0;
    virtual void visitLiteralExpr(std::shared_ptr<LiteralExpr>) = 0;
    virtual void visitGroupingExpr(std::shared_ptr<GroupingExpr>) = 0;
    virtual void visitVariableExpr(std::shared_ptr<VariableExpr>) = 0;
    virtual void visitAssignmentExpr(std::shared_ptr<AssignmentExpr>) = 0;
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

struct VariableExpr : public std::enable_shared_from_this<VariableExpr>, Expr {
    Token name;

    VariableExpr(Token name) : name(name) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitVariableExpr(shared_from_this());
    };
};

struct AssignmentExpr : public std::enable_shared_from_this<AssignmentExpr>, Expr {
    Token name;
    std::shared_ptr<Expr> expr;

    AssignmentExpr(Token name, std::shared_ptr<Expr> expr) : name(name), expr(expr) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitAssignmentExpr(shared_from_this());
    };
};

struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;
struct BlockStmt;

// C++ does not support virtual template functions :)
struct VisitorStmt {
    virtual void visitExpressionStmt(std::shared_ptr<ExpressionStmt>) = 0;
    virtual void visitPrintStmt(std::shared_ptr<PrintStmt>) = 0;
    virtual void visitVarStmt(std::shared_ptr<VarStmt>) = 0;
    virtual void visitBlockStmt(std::shared_ptr<BlockStmt>) = 0;
};

struct Stmt {
    virtual void accept(VisitorStmt&) = 0;
};

struct ExpressionStmt : public std::enable_shared_from_this<ExpressionStmt>, Stmt {
    std::shared_ptr<Expr> expr;

    ExpressionStmt(std::shared_ptr<Expr> expr) : expr(expr) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitExpressionStmt(shared_from_this());
    };
};

struct PrintStmt : public std::enable_shared_from_this<PrintStmt>, Stmt {
    std::shared_ptr<Expr> expr;

    PrintStmt(std::shared_ptr<Expr> expr) : expr(expr) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitPrintStmt(shared_from_this());
    };
};

struct VarStmt : public std::enable_shared_from_this<VarStmt>, Stmt {
    Token name;
    std::shared_ptr<Expr> initializer;

    VarStmt(Token name, std::shared_ptr<Expr> initializer) : name(name), initializer(initializer) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitVarStmt(shared_from_this());
    };
};

struct BlockStmt : public std::enable_shared_from_this<BlockStmt>, Stmt {
    std::vector<std::shared_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::shared_ptr<Stmt>> statements) : statements(statements) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitBlockStmt(shared_from_this());
    };
};

