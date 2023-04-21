// Metaprogramming
#include <bits/stdc++.h>

std::string trim(std::string s) {
    while (s.size() && *s.begin() == ' ') {
        s.erase(s.begin());
    }
    while (s.size() && *s.rbegin() == ' ') {
        s.erase(--s.end());
    }
    return s;
}

std::vector<std::string> split(std::string s, char delimiter) {
    std::vector<std::string> words;
    for (auto it = s.begin();;) {
        auto breakPoint = std::find(it, s.end(), delimiter);

        auto word = trim(std::string(it, breakPoint));
        if (word.size()) {
            words.push_back(word);
        }

        if (breakPoint == s.end()) {
            break;
        }
        it = breakPoint + 1;
    }
    return words;
}

void defineType(std::string baseName, std::string className, std::string fields) {
    className = className + baseName;
    std::cout << "struct " << className << " : public std::enable_shared_from_this<" << className << ">, " << baseName << " {\n";

    // Fields
    auto fieldList = split(fields, ',');
    for (auto field : fieldList) {
        std::cout << "    " << field << ";\n";
    }
    std::cout << "\n";

    // Constructor
    std::cout << "    " << className << "(" << fields << ") : ";
    for (int i = 0; i < fieldList.size(); i++) {
        auto parts = split(fieldList[i], ' ');
        assert(parts.size() == 2);
        auto name = trim(parts[1]);
        std::cout << name << "(" << name << ")";
        if (i + 1 < fieldList.size()) {
            std::cout << ", ";
        }
    }
    std::cout << " {}\n\n";

    // Override of accept
    std::cout << "    virtual void accept(Visitor" << baseName << " &visitor) override {\n";
    std::cout << "        visitor.visit" << className << "(shared_from_this());\n";
    std::cout << "    };\n";

    std::cout << "};\n\n";
}

void defineVisitor(std::string baseName, std::vector<std::string> types) {
    std::cout << "// C++ does not support virtual template functions :)\n";
    std::cout << "struct Visitor" << baseName << " {\n";

    for (auto type : types) {
        auto typeName = trim(split(type, ':')[0]);
        std::cout << "    virtual void visit" << typeName << baseName << "(std::shared_ptr<" << typeName << baseName << ">) = 0;\n";
    }

    std::cout << "};\n\n";
}

void defineAst(std::string baseName, std::vector<std::string> types) {
    // Signatures of subclasses
    for (auto type : types) {
        auto colon = std::find(type.begin(), type.end(), ':');
        std::string className = trim(std::string(type.begin(), colon));
        std::cout << "struct " << className + baseName  << ";\n";
    }
    std::cout << "\n";

    defineVisitor(baseName, types);

    std::cout << "struct " << baseName << " {\n";
    std::cout << "    virtual void accept(Visitor" << baseName << "&) = 0;\n";
    std::cout << "};\n\n";

    for (auto type : types) {
        auto colon = std::find(type.begin(), type.end(), ':');
        std::string className = trim(std::string(type.begin(), colon));
        std::string fields = trim(std::string(colon + 1, type.end()));
        defineType(baseName, className, fields);
    }
}

int main() {
    std::cout << "#pragma once\n";
    std::cout << "#include <bits/stdc++.h>\n";
    std::cout << "#include \"../lexer/token.hpp\"\n\n";

    defineAst("Expr", {
        "Binary     : std::shared_ptr<Expr> lhs, Token op, std::shared_ptr<Expr> rhs",
        "Unary      : Token op, std::shared_ptr<Expr> expr",
        "Literal    : std::any value",
        "Grouping   : std::shared_ptr<Expr> expr",
        "Variable   : Token name",
        "Assignment : Token name, std::shared_ptr<Expr> expr"
    });

    defineAst("Stmt", {
        "Expression : std::shared_ptr<Expr> expr",
        "Print      : std::shared_ptr<Expr> expr",
        "Var        : Token name, std::shared_ptr<Expr> initializer",
        "Block      : std::vector<Stmt> statements"
    });

    return 0;
}
