#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char word[]) {
    char keywords[32][10] = {
        "int", "float", "char", "double",
        "if", "else", "while", "for",
        "do", "return", "void", "break",
        "continue", "switch", "case", "default",
        "long", "short", "signed", "unsigned",
        "const", "static", "struct", "union",
        "typedef", "enum", "sizeof", "goto",
        "auto", "register", "extern", "volatile"
    };

    for (int i = 0; i < 32; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isOperator(char ch) {
    return strchr("+-*/%=<>!&|^~", ch) != NULL;
}

int isSpecialSymbol(char ch) {
    return strchr("(){}[];,.:", ch) != NULL;
}

int main() {
    char input[1000];
    int i = 0;

    printf("Enter a C statement:\n");
    fgets(input, sizeof(input), stdin);

    printf("\nLEXICAL ANALYSIS RESULT\n");
    printf("------------------------------\n");
    printf("%-20s %s\n", "TOKEN TYPE", "LEXEME");
    printf("------------------------------\n");

    while (input[i] != '\0') {

        if (isspace((unsigned char)input[i])) {
            i++;
        }

        else if (isalpha((unsigned char)input[i]) ||
                 input[i] == '_') {

            char word[100];
            int j = 0;

            while (isalnum((unsigned char)input[i]) ||
                   input[i] == '_') {
                word[j++] = input[i++];
            }

            word[j] = '\0';

            if (isKeyword(word))
                printf("%-20s %s\n", "KEYWORD", word);
            else
                printf("%-20s %s\n", "IDENTIFIER", word);
        }

        else if (isdigit((unsigned char)input[i])) {

            char number[100];
            int j = 0;

            while (isdigit((unsigned char)input[i]) ||
                   input[i] == '.') {
                number[j++] = input[i++];
            }

            number[j] = '\0';

            printf("%-20s %s\n", "CONSTANT", number);
        }

        else if (isOperator(input[i])) {

            char op[3];
            op[0] = input[i++];
            op[1] = '\0';

            if ((op[0] == '=' || op[0] == '!' ||
                 op[0] == '<' || op[0] == '>') &&
                 input[i] == '=') {
                op[1] = input[i++];
                op[2] = '\0';
            }

            printf("%-20s %s\n", "OPERATOR", op);
        }

        else if (isSpecialSymbol(input[i])) {

            printf("%-20s %c\n",
                   "SPECIAL SYMBOL", input[i]);
            i++;
        }

        else {
            printf("%-20s %c\n", "UNKNOWN", input[i]);
            i++;
        }
    }

    printf("------------------------------\n");

    return 0;
}
