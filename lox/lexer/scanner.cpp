#include "scanner.hpp"

Scanner::Scanner(std::string &source, ErrorHandler &errorHandler) :
    source(source), errorHandler(errorHandler), start(0), current(0), line(1), tokens({})
{
    keywords["and"]    = TokenType::AND;
    keywords["class"]  = TokenType::CLASS;
    keywords["else"]   = TokenType::ELSE;
    keywords["false"]  = TokenType::FALSE;
    keywords["fun"]    = TokenType::FUN;
    keywords["for"]    = TokenType::FOR;
    keywords["if"]     = TokenType::IF;
    keywords["nil"]    = TokenType::NIL;
    keywords["or"]     = TokenType::OR;
    keywords["print"]  = TokenType::PRINT;
    keywords["return"] = TokenType::RETURN;
    keywords["super"]  = TokenType::SUPER;
    keywords["this"]   = TokenType::THIS;
    keywords["true"]   = TokenType::TRUE;
    keywords["var"]    = TokenType::VAR;
    keywords["while"]  = TokenType::WHILE;
    keywords["break"]  = TokenType::BREAK;
    keywords["continue"]  = TokenType::CONTINUE;
}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", "", line));
    return tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN);  break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE);  break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA);       break;
        case '.': addToken(TokenType::DOT);         break;
        case '-': addToken(TokenType::MINUS);       break;
        case '+': addToken(TokenType::PLUS);        break;
        case ';': addToken(TokenType::SEMICOLON);   break;
        case '*': addToken(TokenType::STAR);        break;
        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);       break;
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);     break;
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);       break;
        case '/':
            if (match('/')) {
                while (!isAtEnd() && peek() != '\n') {
                    advance();
                }
            } else {
                addToken(TokenType::SLASH);
            }
            break;
        case '"': string(); break;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': number(); break;
        case ' ': case '\t': break;
        case '\n':
            line++;
            break;
        default:
            if (isAlpha(c)) {
                identifier();
            } else {
                errorHandler.error(line, "Unexpected character.");
            }
            break;
    }
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

void Scanner::addToken(TokenType type, std::any literal) {
    std::string lexeme = std::string(source.begin() + start, source.begin() + current);
    tokens.push_back(Token(type, lexeme, literal, line));
}

bool Scanner::isAtEnd() {
    return current == source.size();
}

char Scanner::advance() {
    return source[current++];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Scanner::peekNext() {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

void Scanner::string() {
    while (!isAtEnd() && peek() != '"') {
        if (advance() == '\n') {
            line++;
        }
    }

    if (isAtEnd()) {
        errorHandler.error(line, "Unterminated string.");
        return;
    }

    advance();

    std::string literal(source.begin() + start + 1, source.begin() + current - 1);
    addToken(TokenType::STRING, literal);
}

bool Scanner::isDigit(char c) {
    return '0' <= c && c <= '9';
}

bool Scanner::isAlpha(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Scanner::number() {
    while (isDigit(peek())) {
        advance();
    }

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) {
            advance();
        }
    }

    double literal = std::stod(std::string(source.begin() + start, source.begin() + current));
    addToken(TokenType::NUMBER, literal);
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    std::string word(source.begin() + start, source.begin() + current);
    TokenType type = (keywords.count(word) ? keywords[word] : TokenType::IDENTIFIER);
    addToken(type);
}

