CXX := g++
CXXFLAGS := -std=c++17 -O2
BUILD_DIR := build

BUILD_OBJ := $(BUILD_DIR)/lox.o \
     	     $(BUILD_DIR)/error_handler.o \
     	     $(BUILD_DIR)/scanner.o \
     	     $(BUILD_DIR)/token.o \
     	     $(BUILD_DIR)/parser.o \
			 $(BUILD_DIR)/ast_printer.o \
			 $(BUILD_DIR)/interpreter.o \

SOURCE := lox/lox.cpp \
		  lox/error/error_handler.cpp \
		  lox/error/error_handler.hpp \
		  lox/lexer/scanner.cpp \
		  lox/lexer/scanner.hpp \
		  lox/lexer/token.cpp \
		  lox/lexer/token.hpp \
		  lox/parser/parser.cpp \
		  lox/parser/parser.hpp \
		  lox/ast/ast_printer.cpp \
		  lox/ast/ast_printer.hpp \
		  lox/interpreter/interpreter.cpp \
		  lox/interpreter/interpreter.hpp \

$(BUILD_DIR)/lox: $(BUILD_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/ast_printer: $(BUILD_DIR)/ast_printer.o $(BUILD_DIR)/token.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/lox.o: $(SOURCE)
	$(CXX) $(CXXFLAGS) -c -o $@ lox/lox.cpp

$(BUILD_DIR)/error_handler.o: $(SOURCE)
	$(CXX) $(CXXFLAGS) -c -o $@ lox/error/error_handler.cpp

$(BUILD_DIR)/scanner.o: $(SOURCE)
	$(CXX) $(CXXFLAGS) -c -o $@ lox/lexer/scanner.cpp

$(BUILD_DIR)/token.o: $(SOURCE)
	$(CXX) $(CXXFLAGS) -c -o $@ lox/lexer/token.cpp

$(BUILD_DIR)/parser.o: $(SOURCE)
	$(CXX) $(CXXFLAGS) -c -o $@ lox/parser/parser.cpp

$(BUILD_DIR)/ast_printer.o: $(SOURCE)
	$(CXX) $(CXXFLAGS) -c -o $@ lox/ast/ast_printer.cpp

$(BUILD_DIR)/interpreter.o: $(SOURCE)
	$(CXX) $(CXXFLAGS) -c -o $@ lox/interpreter/interpreter.cpp

$(BUILD_DIR)/generate_ast: tools/generate_ast.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

PHONY: tools print
tools: $(BUILD_DIR)/generate_ast

print: $(BUILD_DIR)/ast_printer 

