# sources
LEXER_SRC = ./src/Lexer.cpp ./src/main.cpp
PARSER_SRC = ./src/Lexer.cpp ./src/Parser.cpp ./src/main.cpp

# headers
INCLUDE = ./include

# binaries
BIN = ./bin

# compilation targets
LEXER_TARGET = lexer
PARSER_TARGET = parser

# compiler
G++ = g++

$(LEXER_TARGET): $(LEXER_SRC)
	$(G++) $(LEXER_SRC) -I$(INCLUDE) -o $(BIN)/$(LEXER_TARGET)

$(PARSER_TARGET): $(PARSER_SRC)
	$(G++) $(PARSER_SRC) -I$(INCLUDE) -o $(BIN)/$(PARSER_TARGET)

.PHONY: clean
clean:
	rm -rf $(BIN)/*