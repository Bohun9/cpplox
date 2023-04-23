#include <bits/stdc++.h>

#include "error/error_handler.hpp"
#include "lexer/scanner.hpp"
#include "parser/parser.hpp"
#include "ast/ast_printer.hpp"
#include "interpreter/interpreter.hpp"
#include "analysis/resolver.hpp"

void run(std::string source, ErrorHandler &errorHandler, Interpreter &interpreter) {
    Scanner scanner(source, errorHandler);
    auto tokens = scanner.scanTokens();

    if (errorHandler.hadError) return;

    Parser parser(tokens, errorHandler);
    std::vector<std::shared_ptr<Stmt>> ast = parser.parse();

    if (errorHandler.hadError) return;

    Resolver resolver(interpreter, errorHandler);
    resolver.resolve(ast);

    if (errorHandler.hadError) return;

    interpreter.interpret(ast);
}

void runFile(char *filePath) {
    std::ifstream t(filePath);
    std::stringstream buffer;
    buffer << t.rdbuf();

    ErrorHandler errorHandler;
    Interpreter interpreter(errorHandler);
    run(buffer.str(), errorHandler, interpreter);

    if (errorHandler.hadError) {
        exit(65);
    }
}

void runPrompt() {
    std::string line;
    ErrorHandler errorHandler;
    Interpreter interpreter(errorHandler);

    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        run(line, errorHandler, interpreter);
        errorHandler.hadError = false;
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        std::cerr << "usage: " << argv[0] << " [script]\n";
        exit(64);
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }

    return 0;
}

