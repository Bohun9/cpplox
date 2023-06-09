#pragma once

#include <bits/stdc++.h>

enum class TokenType {
    // single character
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // double character class
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // literals
    STRING, NUMBER, IDENTIFIER,

    // keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, BREAK, CONTINUE,

    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token(TokenType type, std::string lexeme, std::any literal, int line);

    std::string toString();
};

