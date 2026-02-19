# sources
SRC = ./src

# headers
INCLUDE = ./include

# binaries
BIN = ./bin

# compilation targets
LEXER_TARGET = lex

# compiler
G++ = g++

$(LEXER_TARGET): $(SRC)
	$(G++) $(SRC)/*.cpp -I$(INCLUDE) -o $(BIN)/$(LEXER_TARGET)