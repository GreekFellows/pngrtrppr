# 3

On operator precedence.

## The table

Mostly ripped off from [this C++ operator precedence table](http://en.cppreference.com/w/cpp/language/operator_precedence).

Precedence 1 is the highest.

Precedence	| Operator	| Description	| Association
---			| ---		| ---			| ---
1	| `a()`	| Function call	| Left-to-right
2	| `-a`	| Unary minus	| Right-to-left
2	| `!a`	| Logical NOT	| Right-to-left
3	| `a*b`	| Multiplication	| Left-to-right
3	| `a/b`	| Division	| Left-to-right
3	| `a%b`	| Remainder	| Left-to-right
4	| `a+b`	| Addition	| Left-to-right
4	| `a-b`	| Subtraction	| Left-to-right
5	| `<`	| Less than	| Left-to-right
5	| `>`	| Greater than	| Left-to-right
5	| `<=`	| Less than or equal to	| Left-to-right
5	| `>=`	| Greater than or equal to	| Left-to-right
6	| `==`	| Equal to	| Left-to-right
6	| `!=`	| Not equal to	| Left-to-right
7	| `&&`	| Logical AND	| Left-to-right
8	| `||`	| Logical OR	| Left-to-right
9	| `=`	| Direct assignment	| Right-to-left
9	| `+=`	| Compound assignment by sum	| Right-to-left
9	| `-=`	| Compound assignment by difference	| Right-to-left
9	| `*=`	| Compound assignment by product	| Right-to-left
9	| `/=`	| Compound assignment by quotient	| Right-to-left
9	| `%=`	| Compound assignment by remainder	| Right-to-left