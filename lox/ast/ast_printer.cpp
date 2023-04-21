#include <bits/stdc++.h>

#include "ast_printer.hpp"
#include "ast.hpp"
#include "../lexer/token.hpp"

AstPrinter::AstPrinter() { stack = {}; }

std::string AstPrinter::print(std::shared_ptr<Expr> e) {
    e->accept(*this);
    assert(stack.size() > 0);
    auto res = stack.back();
    stack.pop_back();
    return res;
}

void AstPrinter::Return(std::string s) {
    stack.push_back(s);
}

std::string AstPrinter::parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs) {
    std::string s;
    s += "(" + name;
    for (auto e : exprs) {
        s += " " + print(e);
    }
    s += ")";
    return s;
}

void AstPrinter::visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) {
    Return(parenthesize(expr->op.lexeme, {expr->lhs, expr->rhs}));
};

void AstPrinter::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) {
    Return(parenthesize(expr->op.lexeme, {expr->expr}));
};

void AstPrinter::visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) {
    auto value = std::any_cast<double>(expr->value);
    if (int(value) == value) {
        Return(std::to_string(int(value)));
    } else {
        Return(std::to_string(value));
    }
    // Return(std::to_string(std::any_cast<double>(expr->value)));
};

void AstPrinter::visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) {
    Return(parenthesize("grouping", {expr->expr}));
};

// int main() {
//     std::shared_ptr<Expr> e =
//         std::make_shared<BinaryExpr> (
//             std::make_shared<BinaryExpr> (
//                 std::make_shared<LiteralExpr>(1),
//                 Token(TokenType::PLUS, "+", "", 1),
//                 std::make_shared<LiteralExpr>(2)
//             ),
//             Token(TokenType::PLUS, "+", "", 1),
//             std::make_shared<LiteralExpr>(2)
//         );

//     AstPrinter printer;
//     std::cout << printer.print(e) << "\n";

//     return 0;
// }

