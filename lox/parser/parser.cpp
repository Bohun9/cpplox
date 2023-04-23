#include "parser.hpp"

ParseError Parser::error(Token token, std::string message) {
    errorHandler.error(token, message);
    return ParseError();
}

Parser::Parser(std::vector<Token> &tokens, ErrorHandler &errorHandler) : current(0), tokens(tokens), errorHandler(errorHandler) {
    assert(tokens.size() > 0);
    assert(tokens.back().type == TokenType::END_OF_FILE);
}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
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

bool Parser::check(std::vector<TokenType> types) {
    for (auto type : types) {
        if (tokens[current].type == type) {
            return true;
        }
    }
    return false;
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

Token Parser::consume(TokenType expected, std::string message) {
    if (match({expected})) {
        return previous(); 
    }

    throw error(peek(), message);
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    auto lhs = logic_or();
    if (match({TokenType::EQUAL})) {
        auto op = previous();
        auto rhs = logic_or(); 
        if (dynamic_cast<VariableExpr*>(lhs.get()) != nullptr) {
            return std::make_shared<AssignmentExpr>(dynamic_cast<VariableExpr*>(lhs.get())->name, rhs);
        }
        throw error(op, "Invalid assignment target.");
    }
    return lhs;
}

std::shared_ptr<Expr> Parser::logic_or() {
    auto lhs = logic_and();
    if (match({TokenType::OR})) {
        auto op = previous();
        auto rhs = logic_and(); 
        lhs = std::make_shared<LogicalExpr>(lhs, op, rhs);
    }
    return lhs;
}

std::shared_ptr<Expr> Parser::logic_and() {
    auto lhs = equality();
    if (match({TokenType::AND})) {
        auto op = previous();
        auto rhs = equality(); 
        lhs = std::make_shared<LogicalExpr>(lhs, op, rhs);
    }
    return lhs;
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
    return call();
}

std::shared_ptr<Expr> Parser::call() {
    auto callee = primary();
    while (match({TokenType::LEFT_PAREN})) {
        auto paren = previous();
        std::vector<std::shared_ptr<Expr>> arguments;

        if (!check({TokenType::RIGHT_PAREN})) {
            do {
                arguments.push_back(expression());
            } while (match({TokenType::COMMA}));
        }
        consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");

        if (arguments.size() > MAX_ARGUMENTS) {
            error(previous(), "Can't have more that " + std::to_string(MAX_ARGUMENTS) + " arguments.");
        }

        callee = std::make_shared<CallExpr>(callee, paren, arguments);
    }
    return callee;
}

std::shared_ptr<Expr> Parser::primary() {
    if (match({TokenType::TRUE}))  return std::make_shared<LiteralExpr>(true);
    if (match({TokenType::FALSE})) return std::make_shared<LiteralExpr>(false);
    if (match({TokenType::NIL}))   return std::make_shared<LiteralExpr>(nullptr);

    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<VariableExpr>(previous());
    }

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
    if (isAtEnd()) return;
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

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::VAR})) return var();
        if (match({TokenType::FUN})) return functionStmt("function");
        return statement();
    } catch (ParseError &e) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::PRINT})) return print();
    if (match({TokenType::LEFT_BRACE})) return std::make_shared<BlockStmt>(block());
    if (match({TokenType::IF})) return ifStmt();
    if (match({TokenType::WHILE})) return whileStmt();
    if (match({TokenType::FOR})) return forStmt();
    if (match({TokenType::RETURN})) return returnStmt();
    if (match({TokenType::BREAK})) return breakStmt();
    if (match({TokenType::CONTINUE})) return continueStmt();
    return expressionStmt();
}

std::shared_ptr<Stmt> Parser::breakStmt() {
    auto keyword = previous();
    consume(TokenType::SEMICOLON, "Expected ';' after 'break' keyword.");
    return std::make_shared<BreakStmt>(keyword);
}

std::shared_ptr<Stmt> Parser::continueStmt() {
    auto keyword = previous();
    consume(TokenType::SEMICOLON, "Expected ';' after 'continue' keyword.");
    return std::make_shared<ContinueStmt>(keyword);
}

std::shared_ptr<Stmt> Parser::print() {
    auto e = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after value.");
    return std::make_shared<PrintStmt>(e);
}

std::shared_ptr<Stmt> Parser::expressionStmt() {
    auto e = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::make_shared<ExpressionStmt>(e);
}

std::shared_ptr<Stmt> Parser::ifStmt() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
    auto guard = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");

    auto then = statement();
    std::shared_ptr<Stmt> elsee = nullptr;
    if (match({TokenType::ELSE})) {
        elsee = statement();
    }

    return std::make_shared<IfStmt>(guard, then, elsee);
}

std::shared_ptr<Stmt> Parser::whileStmt() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
    auto cond = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");
    auto body = statement();

    return std::make_shared<WhileStmt>(cond, body, false);
}

std::shared_ptr<Stmt> Parser::forStmt() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");

    std::shared_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) {
        initializer = std::make_shared<ExpressionStmt>(std::make_shared<LiteralExpr>(1));
    } else if (match({TokenType::VAR})) {
        initializer = var();
    } else {
        initializer = expressionStmt();
    }

    std::shared_ptr<Expr> condition;
    if (check({TokenType::SEMICOLON})) {
        condition = std::make_shared<LiteralExpr>(true);
    } else {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after loop condition.");

    std::shared_ptr<Expr> increment;
    if (check({TokenType::RIGHT_PAREN})) {
        increment = std::make_shared<LiteralExpr>(1);
    } else {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after loop clauses.");

    std::shared_ptr<Stmt> body = statement();

    return
    std::make_shared<BlockStmt>(std::vector<std::shared_ptr<Stmt>>{
        initializer,
        std::make_shared<WhileStmt>(condition, 
            std::make_shared<BlockStmt>(std::vector<std::shared_ptr<Stmt>>{
                body,
                std::make_shared<ExpressionStmt>(increment),
            }), true)
    });
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd() && !check({TokenType::RIGHT_BRACE})) {
        statements.push_back(declaration());
    }
    consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
    return statements;
}

std::shared_ptr<Stmt> Parser::var() {
    consume(TokenType::IDENTIFIER, "Expected variable name.");
    auto name = previous();

    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after varaible declaration.");

    return std::make_shared<VarStmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::functionStmt(std::string kind) {
    Token name = consume(TokenType::IDENTIFIER, "Expected " + kind + " name.");
    consume(TokenType::LEFT_PAREN, "Expected '(' after " + kind + " name.");

    std::vector<Token> parameters;
    if (!check({TokenType::RIGHT_PAREN})) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name."));
        } while (match({TokenType::COMMA}));
    }
    if (parameters.size() > MAX_ARGUMENTS) {
        error(name, "Can't have more than " + std::to_string(MAX_ARGUMENTS) + " parameters.");
    }

    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");
    consume(TokenType::LEFT_BRACE, "Expected '{' before " + kind + " body.");
    std::vector<std::shared_ptr<Stmt>> body = block();

    return std::make_shared<FunctionStmt>(name, parameters, body);
}

std::shared_ptr<Stmt> Parser::returnStmt() {
    Token keyword = previous();

    std::shared_ptr<Expr> value = nullptr;
    if (!check({TokenType::SEMICOLON})) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after return value.");

    return std::make_shared<ReturnStmt>(keyword, value);
}

