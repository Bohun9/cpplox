#include "parser.hpp"

struct ParseError {};

ParseError Parser::error(Token token, std::string message) {
    errorHandler.error(token, message);
    return ParseError();
}

Parser::Parser(std::vector<Token> &tokens, ErrorHandler &errorHandler) : current(0), tokens(tokens), errorHandler(errorHandler) {
    assert(tokens.size() > 0);
    assert(tokens.back().type == TokenType::END_OF_FILE);
}

std::shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (ParseError &e) {
        return nullptr;
    }
}

Token Parser::advance() {
    return tokens[current++];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::peek() {
    return tokens[current];
}

bool Parser::isAtEnd() {
    return tokens[current].type == TokenType::END_OF_FILE;
}

bool Parser::match(std::vector<TokenType> types) {
    for (auto type : types) {
        if (tokens[current].type == type) {
            advance();
            return true;
        }
    }
    return false;
}

void Parser::consume(TokenType expected, std::string message) {
    if (match({expected})) {
        return; 
    }

    throw error(peek(), message);
}

std::shared_ptr<Expr> Parser::expression() {
    return equality();
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> left = comparison();
    while (match({TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        left = std::make_shared<BinaryExpr>(left, op, right);
    }
    return left;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> left = addition();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expr> right = addition();
        left = std::make_shared<BinaryExpr>(left, op, right);
    }
    return left;
}

std::shared_ptr<Expr> Parser::addition() {
    std::shared_ptr<Expr> left = multiplication();
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        std::shared_ptr<Expr> right = multiplication();
        left = std::make_shared<BinaryExpr>(left, op, right);
    }
    return left;
}

std::shared_ptr<Expr> Parser::multiplication() {
    std::shared_ptr<Expr> left = unary();
    while (match({TokenType::STAR, TokenType::SLASH})) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        left = std::make_shared<BinaryExpr>(left, op, right);
    }
    return left;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        return std::make_shared<UnaryExpr>(op, unary());
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::TRUE}))  return std::make_shared<LiteralExpr>(true);
    if (match({TokenType::FALSE})) return std::make_shared<LiteralExpr>(false);
    if (match({TokenType::NIL}))   return std::make_shared<LiteralExpr>(nullptr);

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_shared<LiteralExpr>(previous().literal); 
    }

    if (match({TokenType::LEFT_PAREN})) {
        std::shared_ptr<Expr> e = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ) after expression.");
        return e;
    }

    throw error(peek(), "Expected expression.");
}

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) {
            return;
        }

        if (peek().type == TokenType::CLASS  ||
            peek().type == TokenType::FOR    ||
            peek().type == TokenType::FUN    ||
            peek().type == TokenType::IF     ||
            peek().type == TokenType::PRINT  ||
            peek().type == TokenType::RETURN ||
            peek().type == TokenType::VAR    ||
            peek().type == TokenType::WHILE) {
            return;
        }
        advance();
    }
}

