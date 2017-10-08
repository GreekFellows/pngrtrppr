stmt
	=	;
	|	expr_9, ';'

expr_9
	=	expr_8 '=' expr_9
	|	expr_8 '+=' expr_9
	|	expr_8 '-=' expr_9
	|	expr_8 '*=' expr_9
	|	expr_8 '/=' expr_9
	|	expr_8 '%=' expr_9
	|	expr_8

expr_8
	=	expr_8 '||' expr_7
	|	expr_7

expr_7
	=	expr_7 '&&' expr_6
	|	expr_6

expr_6
	=	expr_6 '==' expr_5
	|	expr_6 '!=' expr_5
	|	expr_5

expr_5
	=	expr_5 '<' expr_4
	|	expr_5 '>' expr_4
	|	expr_5 '<=' expr_4
	|	expr_5 '>=' expr_4
	|	expr_4

expr_4
	=	expr_4 '+' expr_3
	|	expr_4 '-' expr_3
	|	expr_3

expr_3
	=	expr_3 '*' expr_2
	|	expr_3 '/' expr_2
	|	expr_3 '%' expr_2
	|	expr_2

expr_2
	=	'-' expr_2
	|	'!' expr_2
	|	expr_1

expr_1
	=	expr_1 '(' expr_9... ')''
	=	expr_0

expr_0
	=	acc
	=	lit
	=	'(' expr_9 ')'