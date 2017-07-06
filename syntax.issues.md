# Issue 1

On syntax.

## The ideal syntax

The ideal syntax should allow the user to do all of the followings with the language:

*	Differentiate pngrtrppr lines-of-code / blocks-of-code from LaTeX lines-of-code.
*	Embed pngrtrppr expressions within a LaTeX line-of-code.
*	Avoid having to escape characters as much as possible.
*	Avoid having to press the Shift key as much as possible.

## Solution 1: strings and the `write` statement

This solution requires the implementation of a string data type, which as literals can be represented by a sequence of characters enclosed by double quotes (`"`). Below are examples of valid strings:

	"hello"
	""
	"escaping\nis\n\"cool\""

And invalid strings:

	"
	"\"

This solution also requires that all other data types (boolean, integers, doubles, coordinates for now) are implicitly convertible into strings.

This solution also requires the implementation of a `write` statement, which can take the form of the keyword `write` followed by whitespace-delimited expressions, terminated by a semicolon (`;`):

	write <expr0> <expr1> <expr2> ... ;

Each expression in a `write` statement will have its string representation written to the output file specified in some way (exactly how is to be decided).

This allows regular programming techniques to be employed in pngrtrppr code, since `""`-enclosed strings have nothing to do with being LaTeX; they are nothing but simply strings.

Here is an example:

	for x in (0, 5) {
		write "\psline[linewidth=2pt]{<->}(" x ", " 5-x ")";
	}

Melborps:

*	Allows multiline LaTeX output.

Problems:

*	Isn't specific to LaTeX, so perhaps there can be a better way.

*	Escaped characters such as newline (`\n`) and tab (`\t`) cannot be represented as a backslash and a following character since backslashes are interpreted literally. The problem is how to differentiate backslash for escaping from backslash for macros.

## Solution 2: LaTeX lines starting with backslashes (`\`)

This solution requires differentiating lines-of-code starting with a backslash (`\`) from those that do not. This distinction is recognized at the lexer level; when parsing a new line, if the line begins with a backslash, the lexer puts the entire line into a single LaTeX-type token. There is no syntax that contains such a LaTeX-type token, and thus the parser (interpreter), upon seeing the token, will write the lexeme (the content of the token) into the output file specified by the user.

One issue is that this does not allow writing evaluated values of expressions to file, which is a crucial feature. One discussed workaround to this is enabling the inclusion of pngrtrppr expressions within LaTeX lines-of-code, most likely implemented by choosing specific characters that enclose such expressions, for example, within backticks (` ` `):

	\psline[linewidth=2pt]{<->}(`x`, `5-x`)

Melborps:

*	No need for extra syntax to distinguish LaTeX-writing and pngrtrppr code

Problems:

*	Since many common enclosing characters (e.g. `()`, `[]`, `{}`, `<>`) are reserved for LaTeX, the choice of pngrtrppr-enclosing characters will be limited and will require typing effort.

*	If simpler characters are to be chosen, they have to be backslash-escaped when used as LaTeX rather than for pngrtrppr-enclosing purpose. Below is an example where `()` are used to enclose pngrtrppr expressions:

		\psline[linewidth=2pt]{<->}\( (x), (5-x) \)

	which is not really readable.

*	Code should be lexed / parsed by blocks rather than by lines.

## Discussions

Perhaps Solution 1 is preferrable, but more syntactic sugar and features should be implemented to make coding in pngrtrppr simpler.

One example is implementing the `writeln` statement which is equivalent to the `write` statement but adds a newline character after writing all expressions to the output file.

	for x in (0, 5) {
		writeln "\psline[linewidth=2pt]{<->}(" x ", " 5-x ")";
	}
