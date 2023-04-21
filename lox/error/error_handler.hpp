#pragma once

#include <bits/stdc++.h>

#include "../lexer/token.hpp"

struct ErrorHandler {
    bool hadError;

    ErrorHandler() : hadError(false) {}

    void error(Token token, std::string message);

    void error(int line, std::string message);

    void report(int line, std::string where, std::string message);
};

