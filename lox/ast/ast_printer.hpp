#pragma once

#include <bits/stdc++.h>

#include "expr.hpp"
#include "../lexer/token.hpp"

struct AstPrinter : VisitorExpr {
    std::vector<std::string> stack;

    AstPrinter();

    std::string print(std::shared_ptr<Expr> e);

    void Return(std::string s);

    std::string parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs);

    void visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;

    void visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;

    void visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;

    void visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
};

