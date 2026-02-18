# WHILE Language Syntax

```
S ::= x := a
| skip
| S1; S2
| if P then S1 else S2 endif
| while P do S endwhile

a ::= x
| n
| a1 opa a2


P ::= true
| false
| not P
| P1 opb P2
| a1 opr a2

opa ::= + | − | ∗ | /

opb ::= and | or

opr ::= < | ≤ | = | > | ≥
```
