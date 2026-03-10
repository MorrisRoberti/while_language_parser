# WHILE Language Syntax

```
Statement ::=  TS Statement'
            | skip
            | if Pred then Statement1 else Statement2 endif 
            | while Pred do Statement endwhile
TS ::=  x := Expr;
Statement' ::= Statement* | eps

Expr ::= T (MathOp2 T)*
T ::= F (MathOp1 F)*
F ::= (Expr) | x | n

Pred ::= TP BooleanPred 
       | not Pred 
       | Expr1 RelationalOp Expr2
TP ::= true | false 
BooleanPred ::= BooleanOp Pred | eps 

MathOp2 ::= + | − 
MathOp1 ::= ∗ | /

BooleanOp ::= and | or

RelationalOp ::= < | ≤ | = | > | ≥
```
