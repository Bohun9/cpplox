#pragma once

#include <bits/stdc++.h>

#include "token.hpp"
#include "../error/error_handler.hpp"

struct Scanner {
private:
    std::string source;
    ErrorHandler errorHandler;
    int start;
    int current;
    int line;
    std::vector<Token> tokens;
    std::map<std::string, TokenType> keywords;

public:
    Scanner(std::string &source, ErrorHandler &errorHandler);

    std::vector<Token> scanTokens();

private:
    bool isAtEnd();

    void scanToken();

    char advance();

    void addToken(TokenType type);

    void addToken(TokenType type, std::any literal);

    bool match(char expected);

    char peek();

    char peekNext();

    void string();

    void number();

    void identifier();

    bool isDigit(char c);

    bool isAlpha(char c);

    bool isAlphaNumeric(char c);
};
