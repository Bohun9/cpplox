#pragma once

#include <bits/stdc++.h>
#include "../lexer/token.hpp"

struct ParseError {};

struct RunTimeError {
    Token token;
    std::string message;

    RunTimeError(Token token, std::string message) : token(token), message(message) {}
};

struct ReturnValue {
    Token keyword;
    std::any value;

    ReturnValue(Token keyword, std::any value) : keyword(keyword), value(value) {}
};

struct BreakLoop {
    Token keyword;

    BreakLoop(Token keyword) : keyword(keyword) {}
};

struct ContinueLoop {
    Token keyword;

    ContinueLoop(Token keyword) : keyword(keyword) {}
};

