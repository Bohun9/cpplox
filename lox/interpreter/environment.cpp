#include "environment.hpp"
#include "../error/exceptions.hpp"

Environment::Environment() : enclosing(nullptr) {}
Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

void Environment::define(std::string name, std::any value) {
    bindings[name] = value;
}

std::any Environment::get(Token name) {
    if (bindings.count(name.lexeme)) {
        return bindings[name.lexeme];
    }

    if (enclosing.get() != nullptr) {
        return enclosing->get(name);
    }

    throw RunTimeError(name, "Undefined variable '" + name.lexeme + "'");
}

void Environment::update(Token name, std::any value) {
    if (bindings.count(name.lexeme)) {
        bindings[name.lexeme] = value;
        return;
    }

    if (enclosing.get() != nullptr) {
        enclosing->update(name, value);
        return;
    }

    throw RunTimeError(name, "Undefined variable '" + name.lexeme + "'");
}

