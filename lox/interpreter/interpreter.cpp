#include "interpreter.hpp"

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
    Return(environment->get(expr->name));
}

void Interpreter::visitAssignmentExpr(std::shared_ptr<AssignmentExpr> expr) {
    std::any v = evaluate(expr->expr);
    environment->update(expr->name, v);
    Return(v);
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
    auto new_scope = std::make_shared<Environment>(environment);
    environment = new_scope;

    struct RAII {
        std::shared_ptr<Environment> &e;
        RAII(std::shared_ptr<Environment> &e) : e(e) {}
        ~RAII() { e = e->enclosing; }
    } raii(environment);

    for (auto statement : stmt->statements) {
        execute(statement);
    }
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

