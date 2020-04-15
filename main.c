#include "lexical_analyser.h"

int main(){
    char fname[100];
    printf("Filename: ");
    scanf("%s", fname);
    tokenise(fname);
    return 0;
}
