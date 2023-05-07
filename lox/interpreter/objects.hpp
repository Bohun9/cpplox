#include "interpreter.hpp"

struct LoxInstance;

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

    std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> instance) {
        auto environment = std::make_shared<Environment>(closure);
        environment->define("this", instance);
        return std::make_shared<LoxFunction>(declaration, environment);
    }
};

struct LoxClass : LoxCallable, public std::enable_shared_from_this<LoxClass> {
    std::string name;
    std::shared_ptr<LoxClass> superclass;
    std::map<std::string, std::shared_ptr<LoxFunction>> methods;

    LoxClass(std::string name, std::shared_ptr<LoxClass> superclass, std::map<std::string, std::shared_ptr<LoxFunction>> methods) : name(name), superclass(superclass), methods(methods) {}

    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override {
        auto instance = std::make_shared<LoxInstance>(shared_from_this());
        if (auto init = findMethod("init"); init) {
            init->bind(instance)->call(interpreter, arguments);
        }
        return instance;
    }

    int arity() override {
        if (auto init = findMethod("init"); init) {
            return init->arity();
        }
        return 0;
    }

    std::string toString() override {
        return "<class " + name + ">";
    }

    std::shared_ptr<LoxFunction> findMethod(std::string name) {
        if (methods.count(name)) {
            return methods[name];
        }
        if (superclass) {
            return superclass->findMethod(name);
        }
        return nullptr;
    }
};

struct LoxInstance : public std::enable_shared_from_this<LoxInstance> {
    std::shared_ptr<LoxClass> klass;
    std::map<std::string, std::any> fields;

    LoxInstance(std::shared_ptr<LoxClass> klass) : klass(klass) {}

    std::any get(Token name) {
        if (fields.count(name.lexeme)) {
            return fields[name.lexeme];
        }
        if (auto method = klass->findMethod(name.lexeme); method) {
            return std::static_pointer_cast<LoxCallable>(method->bind(shared_from_this()));
        }
        throw RunTimeError(name, "Undefined property '" + name.lexeme + "'.");
    }

    void update(Token name, std::any value) {
        fields[name.lexeme] = value;
    }

    std::string toString() {
        return "<" + klass->name + " object>";
    }
};

