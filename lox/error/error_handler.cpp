#include <bits/stdc++.h>

#include "error_handler.hpp"

void ErrorHandler::error(Token token, std::string message) {
    report(token.line, token.lexeme, message);
}

void ErrorHandler::error(int line, std::string message) {
    report(line, "", message);
}

void ErrorHandler::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error " + where + ": " << message << "\n";
    hadError = true;
}

