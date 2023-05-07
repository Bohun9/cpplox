#include "../ast/ast.hpp"
#include "../interpreter/interpreter.hpp"
#include "../error/error_handler.hpp"

struct Resolver : VisitorExpr, VisitorStmt {
    Interpreter &interpreter;
    ErrorHandler &errorHandler;
    std::vector<std::shared_ptr<std::map<std::string, bool>>> scopes;

    Resolver(Interpreter &interpreter, ErrorHandler &errorHandler);

    void resolve(std::shared_ptr<Expr> expr);
    void resolve(std::shared_ptr<Stmt> stmt);
    void resolve(std::vector<std::shared_ptr<Stmt>> stmts);
    void resolveLocal(std::shared_ptr<Expr> expr, Token name);

    void declare(Token token);
    void define(Token token);

    void beginScope();
    void endScope();

    void visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
    void visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
    void visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
    void visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) override;
    void visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
    void visitVariableExpr(std::shared_ptr<VariableExpr> expr) override;
    void visitAssignmentExpr(std::shared_ptr<AssignmentExpr> expr) override;
    void visitCallExpr(std::shared_ptr<CallExpr> expr) override;
    void visitGetExpr(std::shared_ptr<GetExpr> expr) override;
    void visitSetExpr(std::shared_ptr<SetExpr> expr) override;
    void visitThisExpr(std::shared_ptr<ThisExpr> expr) override;

    void visitBreakStmt(std::shared_ptr<BreakStmt> expr) override;
    void visitContinueStmt(std::shared_ptr<ContinueStmt> expr) override;
    void visitExpressionStmt(std::shared_ptr<ExpressionStmt> expr) override;
    void visitPrintStmt(std::shared_ptr<PrintStmt> expr) override;
    void visitVarStmt(std::shared_ptr<VarStmt> expr) override;
    void visitBlockStmt(std::shared_ptr<BlockStmt>) override;
    void visitIfStmt(std::shared_ptr<IfStmt>) override;
    void visitWhileStmt(std::shared_ptr<WhileStmt>) override;
    void visitFunctionStmt(std::shared_ptr<FunctionStmt>) override;
    void visitClassStmt(std::shared_ptr<ClassStmt>) override;
    void visitReturnStmt(std::shared_ptr<ReturnStmt>) override;
};
