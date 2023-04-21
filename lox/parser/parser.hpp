#include <bits/stdc++.h>

#include "../lexer/token.hpp"
#include "../ast/expr.hpp"
#include "../error/error_handler.hpp"

struct ParseError;

struct Parser {
private:
    int current;
    std::vector<Token> &tokens;
    ErrorHandler &errorHandler;

public:
    Parser(std::vector<Token> &tokens, ErrorHandler &errorHandler);
    std::shared_ptr<Expr> parse();

private:
    // grammar functions
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> addition();
    std::shared_ptr<Expr> multiplication();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    Token advance();
    Token previous();
    Token peek();
    bool isAtEnd();
    bool match(std::vector<TokenType>);
    void consume(TokenType, std::string);

    ParseError error(Token token, std::string message);
    void synchronize();
};

