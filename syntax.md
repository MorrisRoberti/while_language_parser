# WHILE Language Syntax

```
Program      ::= Statement*

Statement    ::= Assignment | Skip | IfStmt | WhileStmt

Assignment   ::= identifier ':=' Expr ';'
Skip         ::= 'skip'
IfStmt       ::= 'if' Pred 'then' Statement 'else' Statement 'endif'
WhileStmt    ::= 'while' Pred 'do' Statement 'endwhile'

Pred         ::= AndPred ( 'or' AndPred )*
AndPred      ::= UnaryPred ( 'and' UnaryPred )*
UnaryPred    ::= 'not' UnaryPred 
               | PrimaryPred

PrimaryPred  ::= 'true' 
               | 'false' 
               | '(' Pred ')' 
               | Relational

Relational   ::= Expr RelOp Expr
RelOp        ::= '<' | '<=' | '=' | '>' | '>='

Expr         ::= Term ( AddOp Term )*
Term         ::= Factor ( MulOp Factor )*
Factor       ::= '(' Expr ')' 
               | identifier 
               | number

AddOp        ::= '+' | '-'
MulOp        ::= '*' | '/'
```