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
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    /* 
     * If primitive types are instances of class in host lang then literal should have type Object.
     * In C++ it's too much trouble to create abstract class and casting to subclasses.
     */
    std::string literal;
    int line;

    Token(TokenType type, std::string lexeme, std::string literal, int line);

    std::string toString();
};

