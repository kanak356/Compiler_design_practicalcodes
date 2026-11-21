#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char input[100];
int pos = 0;

void E();
void Eprime();
void T();
void Tprime();
void F();

void match(char expected) {
    if (input[pos] == expected) {
        pos++;
    } else {
        printf("Syntax Error: Expected '%c'\n", expected);
        printf("Found '%c'\n", input[pos]);
        exit(0);
    }
}

// E -> TE'
void E() {
    T();
    Eprime();
}

// E' -> +TE' | epsilon
void Eprime() {
    if (input[pos] == '+') {
        match('+');
        T();
        Eprime();
    }
}

// T -> FT'
void T() {
    F();
    Tprime();
}

// T' -> *FT' | epsilon
void Tprime() {
    if (input[pos] == '*') {
        match('*');
        F();
        Tprime();
    }
}

// F -> (E) | i
void F() {
    if (input[pos] == 'i') {
        match('i');
    }
    else if (input[pos] == '(') {
        match('(');
        E();
        match(')');
    }
    else {
        printf("Syntax Error at position %d\n", pos);
        exit(0);
    }
}

int main() {

    printf("RECURSIVE DESCENT PARSER\n");
    printf("------------------------\n");

    printf("Enter an expression (use i for identifier): ");
    scanf("%99s", input);

    E();

    if (input[pos] == '\0') {
        printf("\nString Accepted!\n");
    }
    else {
        printf("\nString Rejected!\n");
    }

    return 0;
}
