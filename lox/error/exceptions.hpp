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
    std::any value;

    ReturnValue(std::any value) : value(value) {}
};

