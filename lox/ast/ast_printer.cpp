#include <bits/stdc++.h>

#include "expr.hpp"
#include "../lexer/token.hpp"

struct AstPrinter : VisitorExpr {
    std::vector<std::string> stack;

    AstPrinter() { stack = {}; }

    std::string print(std::shared_ptr<Expr> e) {
        e->accept(*this);
        assert(stack.size() > 0);
        auto res = stack.back();
        stack.pop_back();
        return res;
    }

    void Return(std::string s) {
        stack.push_back(s);
    }

    std::string parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs) {
        std::string s;
        s += "(" + name;
        for (auto e : exprs) {
            s += " " + print(e);
        }
        s += ")";
        return s;
    }

    virtual void visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override {
        Return(parenthesize(expr->op.lexeme, {expr->lhs, expr->rhs}));
    };

    virtual void visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override {
        Return(parenthesize(expr->op.lexeme, {expr->expr}));
    };

    virtual void visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override {
        Return(std::to_string(std::any_cast<int>(expr->value)));
    };

    virtual void visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override {
        Return(parenthesize("grouping", {expr->expr}));
    };
};

int main() {
    std::shared_ptr<Expr> e =
        std::make_shared<BinaryExpr> (
            std::make_shared<BinaryExpr> (
                std::make_shared<LiteralExpr>(1),
                Token(TokenType::PLUS, "+", "", 1),
                std::make_shared<LiteralExpr>(2)
            ),
            Token(TokenType::PLUS, "+", "", 1),
            std::make_shared<LiteralExpr>(2)
        );

    AstPrinter printer;
    std::cout << printer.print(e) << "\n";

    return 0;
}

