#pragma once

#include <bits/stdc++.h>

#include "../lexer/token.hpp"
#include "../interpreter/interpreter.hpp"

struct RunTimeError;

struct ErrorHandler {
    bool hadError;

    ErrorHandler() : hadError(false) {}

    void error(RunTimeError &e);
    void error(Token token, std::string message);
    void error(int line, std::string message);

    void report(int line, std::string where, std::string message);
};

