#include <bits/stdc++.h>

#include "error/error_handler.hpp"
#include "lexer/scanner.hpp"
#include "parser/parser.hpp"
#include "ast/ast_printer.hpp"
#include "interpreter/interpreter.hpp"

void run(std::string source, ErrorHandler errorHandler) {
    Scanner scanner(source, errorHandler);
    auto tokens = scanner.scanTokens();

    if (errorHandler.hadError) return;

    Parser parser(tokens, errorHandler);
    auto ast = parser.parse();

    if (errorHandler.hadError) return;

    Interpreter interpreter(errorHandler);
    interpreter.interpret(ast);
}

void runFile(char *filePath) {
    std::ifstream t(filePath);
    std::stringstream buffer;
    buffer << t.rdbuf();

    ErrorHandler errorHandler;
    run(buffer.str(), errorHandler);

    if (errorHandler.hadError) {
        exit(65);
    }
}

void runPrompt() {
    std::string line;
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        ErrorHandler errorHandler;
        run(line, errorHandler);
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

