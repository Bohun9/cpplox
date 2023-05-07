#pragma once
#include <bits/stdc++.h>
#include "../lexer/token.hpp"

struct BinaryExpr;
struct LogicalExpr;
struct UnaryExpr;
struct LiteralExpr;
struct GroupingExpr;
struct VariableExpr;
struct AssignmentExpr;
struct CallExpr;
struct GetExpr;
struct SetExpr;
struct ThisExpr;
struct SuperExpr;

// C++ does not support virtual template functions :)
struct VisitorExpr {
    virtual void visitBinaryExpr(std::shared_ptr<BinaryExpr>) = 0;
    virtual void visitLogicalExpr(std::shared_ptr<LogicalExpr>) = 0;
    virtual void visitUnaryExpr(std::shared_ptr<UnaryExpr>) = 0;
    virtual void visitLiteralExpr(std::shared_ptr<LiteralExpr>) = 0;
    virtual void visitGroupingExpr(std::shared_ptr<GroupingExpr>) = 0;
    virtual void visitVariableExpr(std::shared_ptr<VariableExpr>) = 0;
    virtual void visitAssignmentExpr(std::shared_ptr<AssignmentExpr>) = 0;
    virtual void visitCallExpr(std::shared_ptr<CallExpr>) = 0;
    virtual void visitGetExpr(std::shared_ptr<GetExpr>) = 0;
    virtual void visitSetExpr(std::shared_ptr<SetExpr>) = 0;
    virtual void visitThisExpr(std::shared_ptr<ThisExpr>) = 0;
    virtual void visitSuperExpr(std::shared_ptr<SuperExpr>) = 0;
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

struct LogicalExpr : public std::enable_shared_from_this<LogicalExpr>, Expr {
    std::shared_ptr<Expr> lhs;
    Token op;
    std::shared_ptr<Expr> rhs;

    LogicalExpr(std::shared_ptr<Expr> lhs, Token op, std::shared_ptr<Expr> rhs) : lhs(lhs), op(op), rhs(rhs) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitLogicalExpr(shared_from_this());
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

struct CallExpr : public std::enable_shared_from_this<CallExpr>, Expr {
    std::shared_ptr<Expr> callee;
    Token paren;
    std::vector<std::shared_ptr<Expr>> arguments;

    CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments) : callee(callee), paren(paren), arguments(arguments) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitCallExpr(shared_from_this());
    };
};

struct GetExpr : public std::enable_shared_from_this<GetExpr>, Expr {
    std::shared_ptr<Expr> object;
    Token name;

    GetExpr(std::shared_ptr<Expr> object, Token name) : object(object), name(name) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitGetExpr(shared_from_this());
    };
};

struct SetExpr : public std::enable_shared_from_this<SetExpr>, Expr {
    std::shared_ptr<Expr> object;
    Token name;
    std::shared_ptr<Expr> value;

    SetExpr(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value) : object(object), name(name), value(value) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitSetExpr(shared_from_this());
    };
};

struct ThisExpr : public std::enable_shared_from_this<ThisExpr>, Expr {
    Token keyword;

    ThisExpr(Token keyword) : keyword(keyword) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitThisExpr(shared_from_this());
    };
};

struct SuperExpr : public std::enable_shared_from_this<SuperExpr>, Expr {
    Token keyword;
    Token method;

    SuperExpr(Token keyword, Token method) : keyword(keyword), method(method) {}

    virtual void accept(VisitorExpr &visitor) override {
        visitor.visitSuperExpr(shared_from_this());
    };
};

struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;
struct BlockStmt;
struct IfStmt;
struct WhileStmt;
struct FunctionStmt;
struct ClassStmt;
struct ReturnStmt;
struct BreakStmt;
struct ContinueStmt;

// C++ does not support virtual template functions :)
struct VisitorStmt {
    virtual void visitExpressionStmt(std::shared_ptr<ExpressionStmt>) = 0;
    virtual void visitPrintStmt(std::shared_ptr<PrintStmt>) = 0;
    virtual void visitVarStmt(std::shared_ptr<VarStmt>) = 0;
    virtual void visitBlockStmt(std::shared_ptr<BlockStmt>) = 0;
    virtual void visitIfStmt(std::shared_ptr<IfStmt>) = 0;
    virtual void visitWhileStmt(std::shared_ptr<WhileStmt>) = 0;
    virtual void visitFunctionStmt(std::shared_ptr<FunctionStmt>) = 0;
    virtual void visitClassStmt(std::shared_ptr<ClassStmt>) = 0;
    virtual void visitReturnStmt(std::shared_ptr<ReturnStmt>) = 0;
    virtual void visitBreakStmt(std::shared_ptr<BreakStmt>) = 0;
    virtual void visitContinueStmt(std::shared_ptr<ContinueStmt>) = 0;
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

struct IfStmt : public std::enable_shared_from_this<IfStmt>, Stmt {
    std::shared_ptr<Expr> guard;
    std::shared_ptr<Stmt> then;
    std::shared_ptr<Stmt> elsee;

    IfStmt(std::shared_ptr<Expr> guard, std::shared_ptr<Stmt> then, std::shared_ptr<Stmt> elsee) : guard(guard), then(then), elsee(elsee) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitIfStmt(shared_from_this());
    };
};

struct WhileStmt : public std::enable_shared_from_this<WhileStmt>, Stmt {
    std::shared_ptr<Expr> cond;
    std::shared_ptr<Stmt> body;
    bool isDesugaredFor;

    WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body, bool isDesugaredFor) : cond(cond), body(body), isDesugaredFor(isDesugaredFor) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitWhileStmt(shared_from_this());
    };
};

struct FunctionStmt : public std::enable_shared_from_this<FunctionStmt>, Stmt {
    Token name;
    std::vector<Token> parameters;
    std::vector<std::shared_ptr<Stmt>> body;

    FunctionStmt(Token name, std::vector<Token> parameters, std::vector<std::shared_ptr<Stmt>> body) : name(name), parameters(parameters), body(body) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitFunctionStmt(shared_from_this());
    };
};

struct ClassStmt : public std::enable_shared_from_this<ClassStmt>, Stmt {
    Token name;
    std::shared_ptr<VariableExpr> superclass;
    std::vector<std::shared_ptr<FunctionStmt>> methods;

    ClassStmt(Token name, std::shared_ptr<VariableExpr> superclass, std::vector<std::shared_ptr<FunctionStmt>> methods) : name(name), superclass(superclass), methods(methods) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitClassStmt(shared_from_this());
    };
};

struct ReturnStmt : public std::enable_shared_from_this<ReturnStmt>, Stmt {
    Token keyword;
    std::shared_ptr<Expr> expr;

    ReturnStmt(Token keyword, std::shared_ptr<Expr> expr) : keyword(keyword), expr(expr) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitReturnStmt(shared_from_this());
    };
};

struct BreakStmt : public std::enable_shared_from_this<BreakStmt>, Stmt {
    Token keyword;

    BreakStmt(Token keyword) : keyword(keyword) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitBreakStmt(shared_from_this());
    };
};

struct ContinueStmt : public std::enable_shared_from_this<ContinueStmt>, Stmt {
    Token keyword;

    ContinueStmt(Token keyword) : keyword(keyword) {}

    virtual void accept(VisitorStmt &visitor) override {
        visitor.visitContinueStmt(shared_from_this());
    };
};

