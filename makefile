# sources
LEXER_SRC = ./src/Lexer.cpp ./src/main_lexer.cpp
PARSER_SRC = ./src/Lexer.cpp ./src/Parser.cpp ./src/main_parser.cpp

PARSER_SRC_TEST = ./src/Lexer.cpp ./src/Parser.cpp ./tests/test_parser.cpp
LEXER_SRC_TEST = ./src/Lexer.cpp ./tests/test_lexer.cpp

# headers
INCLUDE = ./include

GTEST_LIBS = -lgtest -lgtest_main -pthread

# binaries
BIN = ./bin
TEST_BIN = ./tests/bin

# compilation targets
LEXER_TARGET = lexer
PARSER_TARGET = parser

LEXER_TARGET_TEST = test_lexer
PARSER_TARGET_TEST = test_parser

# compiler
G++ = g++

$(LEXER_TARGET): $(LEXER_SRC)
	$(G++) $(LEXER_SRC) -I$(INCLUDE) -o $(BIN)/$(LEXER_TARGET)

$(PARSER_TARGET): $(PARSER_SRC)
	$(G++) $(PARSER_SRC) -I$(INCLUDE) -o $(BIN)/$(PARSER_TARGET)

$(LEXER_TARGET_TEST): $(LEXER_SRC_TEST)
	$(G++) $(LEXER_SRC_TEST) -I$(INCLUDE) $(GTEST_LIBS) -o $(TEST_BIN)/$(LEXER_TARGET_TEST)

$(PARSER_TARGET_TEST): $(PARSER_SRC_TEST)
	$(G++) $(PARSER_SRC_TEST) -I$(INCLUDE) $(GTEST_LIBS) -o $(TEST_BIN)/$(PARSER_TARGET_TEST)

.PHONY: clean
clean:
	rm -rf $(BIN)/*
	rm -rf $(TEST_BIN)/*