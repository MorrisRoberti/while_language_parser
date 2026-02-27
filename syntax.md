# WHILE Language Syntax

```
Statement ::= x := Expr
| skip
| Statement1; Statement2
| if Pred then Statement1 else Statement2 endif
| while Pred do Statement endwhile

Expr ::= x
| n
| Expr1 MathOp Expr2

Pred ::= true
| false
| not Pred
| Pred1 BinaryOp Pred1
| Expr1 RelationalOp Expr2

MathOp ::= + | − | ∗ | /

BooleanOp ::= and | or

RelationalOp ::= < | ≤ | = | > | ≥

Parenthesis ::= ( | )
```
