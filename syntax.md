# Small syntax

	stmt:
		add_expr ;

	add_expr:
		add_expr + exp_expr
		exp_expr

	exp_expr:
		num ^ exp_expr
		num

Left-associativity result in left recursion, which can be eliminated by:

*	[a simple trick](http://www.csd.uwo.ca/~moreno/CS447/Lectures/Syntax.html/node8.html); or
*	using a right-associative rule but keep reading the same operator and operands to simulate left-associativity (terrible way to phrase this)


# Big syntax

	stmts:
		stmt stmts
		(nothing)

	stmt:
		while ( expr_9 ) { stmts }
		def id = ??? ;
		def id ( id ) = ??? ;
		def id ( id , ... ) = ??? ;
		expr_9 ;
		if ( expr_9 ) { stmts } elif ( expr_9 ) { stmts } ... else { stmts }
		for ( stmt expr_9 ; stmt ) { stmts }

	expr_9:
		id = expr_8
		id += expr_8
		id -= expr_8
		id *= expr_8
		id /= expr_8
		id %= expr_8
		expr_8

	expr_8:
		expr_8 || expr_7
		expr_7

	expr_7:
		expr_7 && expr_6
		expr_6

	expr_6:
		expr_6 == expr_5
		expr_6 != expr_5
		expr_5

	expr_5:
		expr_5 < expr_4
		expr_5 > expr_4
		expr_5 <= expr_4
		expr_5 >= expr_4
		expr_4

	expr_4:
		expr_4 + expr_3
		expr_4 - expr_3
		expr_3

	expr_3:
		expr_3 * expr_2
		expr_3 / expr_2
		expr_3 % expr_2
		expr_2

	expr_2:
		- expr_2
		! expr_2
		expr_1

	expr_1
		expr_1 ( )
		expr_1 ( expr_9 )
		expr_1 ( expr_9 , ... )
		expr_0

	expr_0:
		num
		num unit
		str
		id
		"target" expr_9
		"target" expr_9 "from" expr_9
		( expr_9 )

	id:
		word

To implement `def` as part of the language syntax not just as a preprocessor, something like an `istream` reference switcher will be needed. A token checker must also be implemented to complement this. The effects of a `def` should be limited to its scope. `def` in the global scope will only be effective in the same file.