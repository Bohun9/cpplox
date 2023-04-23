#include "interpreter.hpp"
#include "objects.hpp"

Interpreter::Interpreter(ErrorHandler &errorHandler) : errorHandler(errorHandler) {
    globals = std::make_shared<Environment>(nullptr);
    environment = globals;

    struct NativeClock : LoxCallable {
        std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override {
            return double(time(0));
        }
        int arity() override {
            return 0;
        }
        std::string toString() override {
            return "<native clock fn>";
        }
    };
    std::shared_ptr<LoxCallable> clock(std::make_shared<NativeClock>());
    globals->define("clock", clock);

}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements) {
    try {
        for (auto statement : statements) {
            execute(statement);
        }
    } catch (RunTimeError &e) {
        errorHandler.error(e);
    }
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> e) {
    e->accept(*this);
    assert(stack.size() > 0);
    auto res = stack.back();
    stack.pop_back();
    return res;
}

void Interpreter::execute(std::shared_ptr<Stmt> s) {
    s->accept(*this);
}

void Interpreter::Return(std::any v) {
    stack.push_back(v);
}

std::string Interpreter::stringify(std::any v) {
    if (v.type() == typeid(double)) {
        auto d = std::any_cast<double>(v);
        if (int(d) == d) {
            return std::to_string(int(d));
        } else {
            return std::to_string(d);
        }
    } else if (v.type() == typeid(bool)) {
        return std::to_string(std::any_cast<bool>(v));
    } else if (v.type() == typeid(std::string)) {
        return std::any_cast<std::string>(v);
    } else if (v.type() == typeid(std::nullptr_t)) {
        return "nil";
    } else if (v.type() == typeid(std::shared_ptr<LoxCallable>)) {
        return std::any_cast<std::shared_ptr<LoxCallable>>(v)->toString();
    }
    assert(0);
}

void Interpreter::checkNumberOperand(Token op, std::any v) {
    if (v.type() == typeid(double)) {
        return;
    }
    throw RunTimeError(op, "Operand must be number.");
}

void Interpreter::checkNumberOperands(Token op, std::any lhs, std::any rhs) {
    if (lhs.type() == typeid(double) && rhs.type() == typeid(double)) {
        return;
    }
    throw RunTimeError(op, "Operands must be numbers.");
}

void Interpreter::checkBooleanOperands(Token op, std::any lhs, std::any rhs) {
    if (lhs.type() == typeid(bool) && rhs.type() == typeid(bool)) {
        return;
    }
    throw RunTimeError(op, "Operands must be booleans.");
}

void Interpreter::visitLiteralExpr(std::shared_ptr<LiteralExpr> e) {
    Return(e->value);
}

void Interpreter::visitGroupingExpr(std::shared_ptr<GroupingExpr> e) {
    Return(evaluate(e->expr));
}

void Interpreter::visitBinaryExpr(std::shared_ptr<BinaryExpr> e) {
    auto lhs = evaluate(e->lhs);
    auto rhs = evaluate(e->rhs);

    switch (e->op.type) {
        case TokenType::BANG_EQUAL:
            Return(!isEqual(lhs, rhs));
            break;
        case TokenType::EQUAL_EQUAL:
            Return(isEqual(lhs, rhs));
            break;

        case TokenType::GREATER:
            checkNumberOperands(e->op, lhs, rhs);
            Return(std::any_cast<double>(lhs) > std::any_cast<double>(rhs));
            break;
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(e->op, lhs, rhs);
            Return(std::any_cast<double>(lhs) >= std::any_cast<double>(rhs));
            break;
        case TokenType::LESS:
            checkNumberOperands(e->op, lhs, rhs);
            Return(std::any_cast<double>(lhs) < std::any_cast<double>(rhs));
            break;
        case TokenType::LESS_EQUAL:
            checkNumberOperands(e->op, lhs, rhs);
            Return(std::any_cast<double>(lhs) <= std::any_cast<double>(rhs));
            break;

        case TokenType::PLUS:
            if (lhs.type() == typeid(double) && rhs.type() == typeid(double)) {
                Return(std::any_cast<double>(lhs) + std::any_cast<double>(rhs));
            } else if (lhs.type() == typeid(std::string) && rhs.type() == typeid(std::string)) {
                Return(std::any_cast<std::string>(lhs) + std::any_cast<std::string>(rhs));
            } else {
                throw RunTimeError(e->op, "Operands must be two numbers or two strings.");
            }
            break;
        case TokenType::MINUS:
            checkNumberOperands(e->op, lhs, rhs);
            Return(std::any_cast<double>(lhs) - std::any_cast<double>(rhs));
            break;

        case TokenType::STAR:
            checkNumberOperands(e->op, lhs, rhs);
            Return(std::any_cast<double>(lhs) * std::any_cast<double>(rhs));
            break;
        case TokenType::SLASH:
            checkNumberOperands(e->op, lhs, rhs);
            if (std::any_cast<double>(rhs) == 0.0) {
                throw RunTimeError(e->op, "Division by zero.");
            }
            Return(std::any_cast<double>(lhs) / std::any_cast<double>(rhs));
            break;

        default:
            std::cerr << "Interpreter internal error: unkownn binary operator\n";
            exit(1);
    };
}

void Interpreter::visitLogicalExpr(std::shared_ptr<LogicalExpr> e) {
    auto lhs = evaluate(e->lhs);

    if (e->op.type == TokenType::OR) {
        if (isTruthy(lhs)) {
            Return(lhs);
            return;
        }
    } else {
        if (!isTruthy(lhs)) {
            Return(lhs);
            return;
        }
    }

    auto rhs = evaluate(e->rhs);
    Return(rhs);
}

void Interpreter::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) {
    auto v = evaluate(expr->expr);

    switch (expr->op.type) {
        case TokenType::MINUS:
            checkNumberOperand(expr->op, v);
            Return(-std::any_cast<double>(v)); 
            break;
        case TokenType::BANG:
            Return(!isTruthy(v));
            break;
        default:
            break;
    }
}

void Interpreter::visitVariableExpr(std::shared_ptr<VariableExpr> expr) {
    if (locals.count(expr)) {
        Return(environment->getAt(locals[expr], expr->name));
    } else {
        Return(globals->get(expr->name));
    }
}

void Interpreter::visitAssignmentExpr(std::shared_ptr<AssignmentExpr> expr) {
    std::any v = evaluate(expr->expr);
    if (locals.count(expr)) {
        environment->updateAt(locals[expr], expr->name, v);
    } else {
        globals->update(expr->name, v);
    }
    Return(v);
}

void Interpreter::visitCallExpr(std::shared_ptr<CallExpr> expr) {
    std::any callee = evaluate(expr->callee);

    std::vector<std::any> arguments;
    for (auto argument : expr->arguments) {
        arguments.push_back(evaluate(argument));
    }

    if (callee.type() == typeid(std::shared_ptr<LoxCallable>)) {
        auto callable = std::any_cast<std::shared_ptr<LoxCallable>>(callee);
        if (callable->arity() == arguments.size()) {
            try {
                Return(callable->call(*this, arguments));
            } catch (ReturnValue &returnvalue) {
                Return(returnvalue.value);
            }
        } else {
            throw RunTimeError(expr->paren, "Expected " + std::to_string(callable->arity()) + " parameters, but got " + std::to_string(arguments.size()) + "arguments.");
        }
    } else {
        throw RunTimeError(expr->paren, "Can only call functions and classes.");
    }
}

bool Interpreter::isEqual(std::any lhs, std::any rhs) {
    if (lhs.type() != rhs.type()) return false;
    if (lhs.type() == typeid(std::nullptr_t)) return true;

    if (lhs.type() == typeid(double)) {
        return std::any_cast<double>(lhs) == std::any_cast<double>(rhs);
    } else if (lhs.type() == typeid(std::string)) {
        return std::any_cast<std::string>(lhs) == std::any_cast<std::string>(rhs);
    } else if (lhs.type() == typeid(bool)) {
        return std::any_cast<bool>(lhs) == std::any_cast<bool>(rhs);
    }
    assert(0);
}

bool Interpreter::isTruthy(std::any v) {
    if (v.type() == typeid(std::nullptr_t)) return false;
    if (v.type() == typeid(bool)) return std::any_cast<bool>(v);
    return true;
}

void Interpreter::visitExpressionStmt(std::shared_ptr<ExpressionStmt> s) {
    evaluate(s->expr);
}

void Interpreter::visitPrintStmt(std::shared_ptr<PrintStmt> s) {
    auto v = evaluate(s->expr);
    std::cout << stringify(v) << "\n";
}

void Interpreter::visitVarStmt(std::shared_ptr<VarStmt> expr) {
    std::any value = nullptr;
    if (expr->initializer != nullptr) {
        value = evaluate(expr->initializer);
    }
    environment->define(expr->name.lexeme, value);
}

void Interpreter::visitBlockStmt(std::shared_ptr<BlockStmt> stmt) {
    executeBlock(stmt->statements, std::make_shared<Environment>(environment));
}

void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> newEnvironment) {
    auto previous = environment;
    environment = newEnvironment;

    struct RAII {
        std::shared_ptr<Environment> &e;
        std::shared_ptr<Environment> &p;
        RAII(std::shared_ptr<Environment> &e, std::shared_ptr<Environment> &p) : e(e), p(p) {}
        ~RAII() { e = p; }
    } raii(environment, previous);

    for (auto statement : statements) {
        execute(statement);
    }
}

void Interpreter::resolve(std::shared_ptr<Expr> expr, int numHops) {
    assert(!locals.count(expr));
    locals[expr] = numHops;
}

void Interpreter::visitIfStmt(std::shared_ptr<IfStmt> stmt) {
    if (isTruthy(evaluate(stmt->guard))) {
        execute(stmt->then);
    } else if (stmt->elsee != nullptr) {
        execute(stmt->elsee);
    }
}

void Interpreter::visitWhileStmt(std::shared_ptr<WhileStmt> stmt) {
    while (isTruthy(evaluate(stmt->cond))) {
        execute(stmt->body);
    }
}

void Interpreter::visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) {
    std::shared_ptr<LoxCallable> loxFunction = std::make_shared<LoxFunction>(stmt, environment);
    environment->define(stmt->name.lexeme, loxFunction);
}

void Interpreter::visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) {
    std::any value = (stmt->expr == nullptr ? nullptr : evaluate(stmt->expr));
    throw ReturnValue(value);
}


