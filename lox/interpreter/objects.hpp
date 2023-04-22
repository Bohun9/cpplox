#include "interpreter.hpp"

struct LoxCallable {
    virtual std::any call(Interpreter &Interpreter, std::vector<std::any> arguments) = 0;
    virtual int arity() = 0;
    virtual std::string toString() = 0;
};

struct LoxFunction : LoxCallable {
    std::shared_ptr<FunctionStmt> declaration;
    std::shared_ptr<Environment> closure;

    LoxFunction(std::shared_ptr<FunctionStmt> declration, std::shared_ptr<Environment> closure) : declaration(declration), closure(closure) {}

    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) {
        auto environment = std::make_shared<Environment>(closure);
        assert(declaration->parameters.size() == arguments.size());
        for (int i = 0; i < arguments.size(); i++) {
            environment->define(declaration->parameters[i].lexeme, arguments[i]);
        }
        interpreter.executeBlock(declaration->body, environment);
        return nullptr;
    }

    int arity() {
        return declaration->parameters.size();
    }

    std::string toString() {
        return "<fn " + declaration->name.lexeme + ">";
    }
};

