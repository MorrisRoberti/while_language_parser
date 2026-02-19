# WHILE Language Parser

> This is a lexer + parser repo for the WHILE language, that is a learning-purpose language.

## Structure

It is made of two components:

- **Lexer** whose task is to lexically analize the program and _tokenize_ the input;
- **Parser** who follows the _syntax rules_ of the language to validate it and produce an _AST_ (_Abstract Syntax Tree_) of the program.

### Lexer

The **lexer** component performs lexical analysis of the input program; that means that it doesn't catch syntax rules like `a + b`, but blindly _tokenizes_ the input recognizing the different types of tokens.

It has a method `nextToken()` that returns a `Token` object composed of:

- `TokenType m_token_type` -> the type of the token (`NUMBER`, `IDENTIFIER`, `ASSIGN`, etc.);
- `std::string m_value` -> the actual string of the corresponding type;

#### Token Types

Here it is a table with an example of value per type

| Token Type      | Example       | Description                                         |
| :-------------- | :------------ | :-------------------------------------------------- |
| **UNKNOWN**     | `@`, `$`      | Any character not recognized by the grammar.        |
| **IDENTIFIER**  | `x`, `my_var` | Variable or function names defined by the user.     |
| **WHITESPACE**  | ` `           | Spaces.                                             |
| **NUMBER**      | `42`, `1024`  | Sequences of numeric digits.                        |
| **SKIP**        | `skip`        | A "no-op" or null instruction.                      |
| **IF**          | `if`          | Start of a conditional block.                       |
| **THEN**        | `then`        | Introduces the "true" branch of a conditional.      |
| **ELSE**        | `else`        | Introduces the alternative branch of a conditional. |
| **ENDIF**       | `endif`       | Termination of a conditional block.                 |
| **WHILE**       | `while`       | Start of an iterative loop.                         |
| **DO**          | `do`          | Introduces the body of a loop.                      |
| **ENDWHILE**    | `endwhile`    | Termination of a loop block.                        |
| **TRUE**        | `true`        | Boolean constant for **true**.                      |
| **FALSE**       | `false`       | Boolean constant for **false**.                     |
| **SEMICOLON**   | `;`           | Statement terminator.                               |
| **ASSIGN**      | `:=`          | Variable assignment operator.                       |
| **EQ**          | `=`           | Equality comparison operator.                       |
| **LT**          | `<`           | Less-than operator.                                 |
| **LTE**         | `<=`          | Less-than-or-equal-to operator.                     |
| **GT**          | `>`           | Greater-than operator.                              |
| **GTE**         | `>=`          | Greater-than-or-equal-to operator.                  |
| **PLUS**        | `+`           | Addition operator.                                  |
| **MINUS**       | `-`           | Subtraction operator.                               |
| **WILDCARD**    | `*`           | Multiplication operator or wildcard symbol.         |
| **SLASH**       | `/`           | Division operator.                                  |
| **AND**         | `and`         | Logical AND operator.                               |
| **OR**          | `or`          | Logical OR operator.                                |
| **NOT**         | `not`         | Logical negation operator.                          |
| **LPAREN**      | `(`           | Left (opening) parenthesis.                         |
| **RPAREN**      | `)`           | Right (closing) parenthesis.                        |
| **END_OF_FILE** | `EOF`         | End of the source file stream.                      |
| **END_OF_LINE** | `EOL`         | Newline character.                                  |

For the _identifier_ it's used the standard C convention.

**IMPORTANT**: the reserved keywords are case sensitive i.e. `skip` is of type `SKIP`, while `Skip`, `SKIP`, `sKip` etc. are of type `IDENTIFIER`.

## Parser
