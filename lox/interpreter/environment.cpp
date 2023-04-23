#include "environment.hpp"
#include "../error/exceptions.hpp"

Environment::Environment() : enclosing(nullptr) {}
Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

Environment *Environment::ancestor(int numHops) {
    Environment *env = this;
    while (numHops--) {
        env = (env->enclosing).get();
    }
    return env;
}

void Environment::define(std::string name, std::any value) {
    bindings[name] = value;
}

std::any Environment::get(Token name) {
    if (bindings.count(name.lexeme)) {
        return bindings[name.lexeme];
    }

    throw RunTimeError(name, "Undefined variable '" + name.lexeme + "'");
}

std::any Environment::getAt(int numHops, Token name) {
    return ancestor(numHops)->get(name);
}

void Environment::update(Token name, std::any value) {
    if (bindings.count(name.lexeme)) {
        bindings[name.lexeme] = value;
        return;
    }

    throw RunTimeError(name, "Undefined variable '" + name.lexeme + "'");
}

void Environment::updateAt(int numHops, Token name, std::any value) {
    ancestor(numHops)->update(name, value);
}

