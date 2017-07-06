# tech

## sample code

```
def line(lw, style, p0, p1) = \psline*[linewidth=`lw`]{`style`} `p0` `p1`;

for i in (0, 10, 2) {
	if (i % 2 == 0) {
		line(2, <->, (i * 2, 0), (i, 50));
		// equivalent to \psline*[linewidth=`2`]{`<->`}`(i * 2, 0)` `(i, 50)`;
	}
	else {
		line(2, <->, (0, i), (50, i * 2));
		// equivalent to \psline*[linewidth=`2`]{`<->`}`(0, i)` `(50, i * 2)`;
	}
}
```

## lexer

### tokens

*	operators: `+`, `-`, `*`, `/`, `%`, `==`, `>`, `<`, `>=`, `<=`, `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `{`, `}`, `(`, `)`, `` ` ``, `,`, `;`
*	keywords: `def`, `for`, `in`, `if`, `else`
*	words: identifiers defined by the user, any pstricks macro (e.g. `\psline*`), option names and values (e.g. both `linecolor` and `green` in `{linecolor=green}`)

## parser

### value types

*	integers, with or without units
*	doubles, with or without units
*	texts
*	coords

## problems

*	are there better alternatives for the parse-as-expr symbol `:`?

## grammar

a piece of valid pngrtrppr code must meet the definition of `stmts`.

*	`stmts`:
	*	nothing
	*	`stmt stmts`
*	`stmt`:
	*	`;`
	*	`write <expr> ... ;`
*	`expr`:
	*	`factor`
	*	`expr + factor`
	*	`expr - factor`
*	`factor`:
	*	`term`
	*	`factor * term`
	*	`factor / term`
	*	`factor % term`
*	`term`:
	*	`num`
	*	`num unit`
	*	`str`
	*	`( expr )`
	*	`( expr, expr )` which gives coords