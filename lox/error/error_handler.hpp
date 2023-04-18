#pragma once

#include <bits/stdc++.h>

struct ErrorHandler {
    bool hadError;

    ErrorHandler() : hadError(false) {}

    void error(int line, std::string message);

    void report(int line, std::string where, std::string message);
};

