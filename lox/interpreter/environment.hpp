#pragma once

#include <bits/stdc++.h>

#include "../lexer/token.hpp"

struct Environment {
    std::map<std::string, std::any> bindings;
    std::shared_ptr<Environment> enclosing;

    Environment();
    Environment(std::shared_ptr<Environment> enclosing);

    void define(std::string name, std::any value);
    void update(Token name, std::any value);
    std::any get(Token name);
};
