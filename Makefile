CXX := g++
CXXFLAGS := -std=c++17 -O2
BUILD_DIR := build

OBJS := \
             $(BUILD_DIR)/lox.o \
             $(BUILD_DIR)/error_handler.o \
             $(BUILD_DIR)/scanner.o \
             $(BUILD_DIR)/token.o \
             $(BUILD_DIR)/parser.o \
             $(BUILD_DIR)/ast_printer.o \
             $(BUILD_DIR)/interpreter.o \
             $(BUILD_DIR)/environment.o \
             $(BUILD_DIR)/resolver.o \

# SOURCE := \
#              lox/lox.cpp \
#              lox/error/error_handler.cpp \
#              lox/lexer/scanner.cpp \
#              lox/lexer/token.cpp \
#              lox/parser/parser.cpp \
#              lox/ast/ast_printer.cpp \
#              lox/interpreter/interpreter.cpp \
#              lox/interpreter/environment.cpp \
#              lox/analysis/resolver.cpp \

HEADERS := \
             lox/error/error_handler.hpp \
             lox/lexer/scanner.hpp \
             lox/lexer/token.hpp \
             lox/parser/parser.hpp \
             lox/ast/ast_printer.hpp \
             lox/interpreter/interpreter.hpp \
             lox/interpreter/environment.hpp \
             lox/interpreter/objects.hpp \
             lox/analysis/resolver.hpp

$(BUILD_DIR)/lox: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/ast_printer: $(BUILD_DIR)/ast_printer.o $(BUILD_DIR)/token.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/lox.o: $(HEADERS) lox/lox.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/lox.cpp

$(BUILD_DIR)/error_handler.o: $(HEADERS) lox/error/error_handler.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/error/error_handler.cpp

$(BUILD_DIR)/scanner.o: $(HEADERS) lox/lexer/scanner.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/lexer/scanner.cpp

$(BUILD_DIR)/token.o: $(HEADERS) lox/lexer/token.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/lexer/token.cpp

$(BUILD_DIR)/parser.o: $(HEADERS) lox/parser/parser.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/parser/parser.cpp

$(BUILD_DIR)/ast_printer.o: $(HEADERS) lox/ast/ast_printer.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/ast/ast_printer.cpp

$(BUILD_DIR)/interpreter.o: $(HEADERS) lox/interpreter/interpreter.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/interpreter/interpreter.cpp

$(BUILD_DIR)/environment.o: $(HEADERS) lox/interpreter/environment.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/interpreter/environment.cpp

$(BUILD_DIR)/resolver.o: $(HEADERS) lox/analysis/resolver.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ lox/analysis/resolver.cpp

$(BUILD_DIR)/generate_ast: tools/generate_ast.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

PHONY: tools print
tools: $(BUILD_DIR)/generate_ast

print: $(BUILD_DIR)/ast_printer 

