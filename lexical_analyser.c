/**This is the BETA version of a C program that does the lexical analysis of a C source file. Note the following things:
As the lexical analysis happens after preprocessing, I assumed that preprocessing had happened, so it does not recognize any macros.
It currently does not support comments of any kind, so leaving comments in the source file will cause LEXICAL ERROR.
The trinary operator is not yet supported, sizeof() is, but it will recognize sizeof as a keyword, not sizeof() as an operator.
Integer and real constants are currently only supported in decimal format, and it does not recognize modifiers like 'l' 'f' or 'd',
so 3.14 and 8567 are valid, but 3.14f, 3.14d, or 8567l will all cause LEXICAL ERRORs.
Escaping in character constants is currently not supported, so 'a' is valid, but special characters like '\n' are not recognized.
Escaping in string constants works though.**/

#include "lexical_analyser.h"

/**List of all C keywords. There has to be a more elegant way of doing this...**/
const char c_keywords[NUM_OF_KEYWORDS][9] = {"auto", "break", "case", "char", "const", "continue", "default", "do",
                                            "double", "else", "enum", "extern", "float", "for", "goto", "if",
                                            "int", "long", "register", "return", "short", "signed", "sizeof", "static",
                                            "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

/**And the list of the operators. It currently does not include sizeof() and the trinary operator**/
const char operators[NUM_OF_OPERATORS][4] = {"+", "-", "*", "/", "%", "++", "--", "==", "!=", "<", ">",
                                            "<=", ">=", "&&", "||", "!", "&", "|", "^", "~", ">>", "<<",
                                            "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|="};

/**String to store the delimiter characters including whitespace**/
const char delimiters[] = " \n\t+-*/,;><=()[]{}%!&|^~";

/**The special symbols in C. These are valid tokens on their own.**/
const char spec_symbols[] = "[]{}(),;";

/**Function to check if a string is a valid C keyword. Returns 1 if it is and 0 if it's not.**/
int is_c_keyword(const char* s){
    int b = -1;
    for (int i = 0; i < NUM_OF_KEYWORDS && (b != 0); i++){
        b = strcmp(s, c_keywords[i]);
    }
    return ((b == 0) ? 1 : 0);
}

/**Function to check if a string is a valid C operator. Returns 1 if it is and 0 if it's not.**/
int is_operator(const char* s){
    int b = -1;
    for (int i = 0; i < NUM_OF_OPERATORS && (b != 0); i++){
        b = strcmp(s, operators[i]);
    }
    return ((b == 0) ? 1 : 0);
}

/**Function to check if a character is a delimiter. Returns some none-zero value if it is, and 0 if it's not.**/
int is_delimiter(char ch){
    int b = 0;
    for (int i = 0; delimiters[i] != '\0' && !b; i++){
        b = (ch == delimiters[i]);
    }
    return b;
}

/**Function to check if a string is valid C identifier. Returns some none-zero value if it is, and 0 if it's not.**/
int is_identifier(const char* s){
    if (strlen(s) == 0){
        return 0;
    }
    int b = 1;
    for (int i = 0; s[i] != '\0' && b; i++){
        b = (s[i] == '_' || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9'));
    }
    return (b && !(s[0] >= 48 && s[0] <= 57) && !is_c_keyword(s) && strlen(s) <= 31);
}

/**Function to check if a string is an integer constant. Returns some none-zero value if it is, and 0 if it's not.**/
int is_integer_const(const char* s){
    if (strlen(s) == 0){
        return 0;
    }
    int b = 1;
    for (int i = 0; s[i] != '\0' && b; i++){
        b = (s[i] >= '0' && s[i] <= '9');
    }
    return b;
}

/**Function to check if a string is a real constant. Returns some none-zero value if it is, and 0 if it's not.
It currently does not recognize f and d at the end of the number. So it matches with 3.14 but not with 3.14f or 3.14d**/
int is_real_constant(const char* s){
    if (strlen(s) < 2){
        return 0;
    }
    int c = 0; ///counts the number of '.' characters
    int b = 1;
    for (int i = 0; s[i] != '\0' && b && (c < 2); i++){
        b = ((s[i] >= '0' && s[i] <= '9') || s[i] == '.');
        if (s[i] == '.'){
            c++;
        }
    }
    return (b && (c == 1));
}

/**This function checks if string s is a valid string constant, however dumb that sounds.
Returns some none-zero value if it is, and 0 if it's not.**/
int is_string_constant(const char* s){
    if (strlen(s) < 2){
        return 0;
    }
    int b = 1;
    for (unsigned int i = 1; (i < strlen(s) - 1) && b; i++){
        b = ((s[i] != '"') || s[i-1] == '\\');
    }
    return (s[0] == '"' && s[strlen(s)-1] == '"' && b);
}

/**Function to check if a string is a character constant. Returns some none-zero value if it is, and 0 if it's not.
This currently does not support escaping.**/
int is_char_constant(const char* s){
    return ((strlen(s) == 3) ? (s[0] == '\'' && s[2] == '\'') : 0);
}

/**Function to check if a string is a special symbol. Returns some none-zero value if it is, and 0 if it's not.**/
int is_spec_symbol(const char* s){
    if (strlen(s) != 1){
        return 0;
    }
    int b = 0;
    for (int i = 0; spec_symbols[i] != '\0' && !b ; i++){
        b = (s[0] == spec_symbols[i]);
    }
    return b;
}

/**Function to check if a character is whitespace. Returns some none-zero value if it is, and 0 if it's not.**/
int is_whitespace(char c){
    return (c == ' ' || c == '\n' || c == '\t');
}

/**A procedure to try and recognize a given token "candidate".
It prints an error message if no match is found, and terminates the process with status 1**/
void analyse(const char* s){
    if (is_c_keyword(s)) printf("%s ---> C keyword\n", s);
    else if (is_spec_symbol(s)) printf("%s ---> special symbol\n", s);
    else if (is_identifier(s)) printf("%s ---> valid identifier\n", s);
    else if (is_integer_const(s)) printf("%s ---> integer constant\n", s);
    else if (is_real_constant(s)) printf("%s ---> real constant\n", s);
    else if (is_string_constant(s)) printf("%s ---> string constant\n", s);
    else if (is_char_constant(s)) printf("%s ---> character constant\n", s);
    else if (is_operator(s)) printf("%s ---> operator\n", s);
    else{
        printf("LEXICAL ERROR: %s is not a valid token\n", s);
        exit(1);
    }
}

/**A procedure to split the input file into token candidates, then analyze those tokens using analyse()**/
void tokenise(const char* f_name){
    FILE* f = fopen(f_name, "r");
    if (f == NULL){
        perror("File open error");
    }
    int c, w_length = 0;
    char word[MAX_TOKEN_LENGTH];
    while ((c = fgetc(f)) != EOF){
        label:
        if (!is_delimiter((char) c) && w_length < MAX_TOKEN_LENGTH){
            word[w_length++] = (char) c;
        }
        else{
            if (w_length > 0){
                word[w_length] = '\0';
                analyse(word);
                w_length = 0;
            }
            word[0] = (char) c;
            word[1] = '\0';
            if (is_spec_symbol(word)){
                analyse(word);
            }
            else if (is_operator(word)){
                w_length = 1;
                do{
                    c = fgetc(f);
                    word[w_length++] = (char) c;
                    word[w_length] = '\0';
                }while(is_operator(word));
                word[w_length-1] = '\0';
                analyse(word);
                w_length = 0;
                goto label;
            }
        }
    }
    if (w_length > 0){
        word[w_length] = '\0';
        analyse(word);
    }
    fclose(f);
}
