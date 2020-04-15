/**This is the BETA version of a C program that does the lexical analysis of a C source file. Note the following things:
As the lexical analysis happens after preprocessing, I assumed that preprocessing had happened, so it does not recognize any macros.
It currently does not support comments of any kind, so leaving comments in the source file will cause LEXICAL ERROR.
The trinary operator is not yet supported, sizeof() is, but it will recognize sizeof as a keyword, not sizeof() as an operator.
Integer and real constants are currently only supported in decimal format, and it does not recognize modifiers like 'l' 'f' or 'd',
so 3.14 and 8567 are valid, but 3.14f, 3.14d, or 8567l will all cause LEXICAL ERRORs.
Escaping in character constants is currently not supported, so 'a' is valid, but special characters like '\n' are not recognized.
Escaping in string constants works though.**/

#ifndef LEXICAL_ANALYSER_H
#define LEXICAL_ANALYSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define NUM_OF_KEYWORDS 32 ///Adding more is simple, write it in the corresponding array, then change the macro accordingly
#define NUM_OF_OPERATORS 33
#define MAX_TOKEN_LENGTH 33

extern const char c_keywords[NUM_OF_KEYWORDS][9];
extern const char operators[NUM_OF_OPERATORS][4];
extern const char delimiters[];
extern const char spec_symbols[];

/**Function to check if a string is a valid C keyword. Returns 1 if it is and 0 if it's not.**/
int is_c_keyword(const char* s);

/**Function to check if a string is a valid C operator. Returns 1 if it is and 0 if it's not.**/
int is_operator(const char* s);

/**Function to check if a string is valid C identifier. Returns some none-zero value if it is, and 0 if it's not.**/
int is_identifier(const char* s);

/**Function to check if a string is an integer constant. Returns some none-zero value if it is, and 0 if it's not.**/
int is_integer_const(const char* s);

/**Function to check if a character is a delimiter. Returns some none-zero value if it is, and 0 if it's not.**/
int is_delimiter(char ch);

/**Function to check if a string is a real constant. Returns some none-zero value if it is, and 0 if it's not.
It currently does not recognize f and d at the end of the number. So it matches with 3.14 but not with 3.14f or 3.14d**/
int is_real_constant(const char* s);

/**This function checks if string s is a valid string constant, however dumb that sounds.
Returns some none-zero value if it is, and 0 if it's not.**/
int is_string_constant(const char* s);

/**Function to check if a string is a character constant. Returns some none-zero value if it is, and 0 if it's not.
This currently does not support escaping.**/
int is_char_constant(const char* s);

/**Function to check if a character is whitespace. Returns some none-zero value if it is, and 0 if it's not.**/
int is_whitespace(char c);

/**Function to check if a string is a special symbol. Returns some none-zero value if it is, and 0 if it's not.**/
int is_spec_symbol(const char* s);

/**A procedure to split the input file into token candidates, then analyze those tokens using analyse()**/
void tokenise(const char* f_name);

/**A procedure to try and recognize a given token "candidate".
It prints an error message if no match is found, and terminates the process with status 1**/
void analyse(const char* s);

#endif
