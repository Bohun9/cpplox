#include <bits/stdc++.h>

#include "../lexer/token.hpp"
#include "../ast/ast.hpp"
#include "../error/error_handler.hpp"
#include "../error/exceptions.hpp"

struct ParseError;

struct Parser {
private:
    int current;
    std::vector<Token> &tokens;
    ErrorHandler &errorHandler;

public:
    Parser(std::vector<Token> &tokens, ErrorHandler &errorHandler);
    std::vector<std::shared_ptr<Stmt>> parse();

private:
    // grammar functions
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> logic_or();
    std::shared_ptr<Expr> logic_and();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> addition();
    std::shared_ptr<Expr> multiplication();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> print();
    std::shared_ptr<Stmt> var();
    std::shared_ptr<Stmt> expressionStmt();
    std::shared_ptr<Stmt> ifStmt();
    std::shared_ptr<Stmt> whileStmt();
    std::shared_ptr<Stmt> forStmt();
    std::vector<std::shared_ptr<Stmt>> block();

    Token advance();
    Token previous();
    Token peek();
    bool isAtEnd();
    bool match(std::vector<TokenType>);
    bool check(std::vector<TokenType>);
    void consume(TokenType, std::string);

    ParseError error(Token token, std::string message);
    void synchronize();
};

