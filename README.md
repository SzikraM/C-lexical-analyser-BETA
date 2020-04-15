# C-lexical-analyser-BETA

This is the BETA version of a C program that does the lexical analysis of a C source file. Note the following things:

As the lexical analysis happens after preprocessing, I assumed that preprocessing had happened, so it does not recognize any macros.

It currently does not support comments of any kind, so leaving comments in the source file will cause LEXICAL ERROR.

The trinary operator is not yet supported, sizeof() is, but it will recognize sizeof as a keyword, not sizeof() as an operator.

Integer and real constants are currently only supported in decimal format, and it does not recognize modifiers like 'l' 'f' or 'd',
so 3.14 and 8567 are valid, but 3.14f, 3.14d, or 8567l will all cause LEXICAL ERRORs.

Escaping in character constants is currently not supported, so 'a' is valid, but special characters like '\n' are not recognized.
Escaping in string constants works though.

The actual project consist of lexical_analyser.c and lexical_analyser.h, but I added some test files containing C source code,
and a sample main too.
