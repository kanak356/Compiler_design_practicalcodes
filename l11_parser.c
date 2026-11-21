#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char stack[100];
int top = -1;

void push(char c) {
    stack[++top] = c;
}

char pop() {
    return stack[top--];
}

int main() {
    char input[100];
    int i = 0;

    printf("Grammar:\n");
    printf("E -> iA\n");
    printf("A -> +iA | #\n\n");

    printf("Enter input string (use i for identifier): ");
    scanf("%99s", input);

    strcat(input, "$");

    push('$');
    push('E');

    printf("\nStack\t\tInput\t\tAction\n");

    while (top >= 0) {
        char X = stack[top];
        char a = input[i];

        printf("%.*s\t\t%s\t\t", top + 1, stack, input + i);

        if (X == '$' && a == '$') {
            printf("ACCEPT\n");
            return 0;
        }

        if (X == a) {
            pop();
            i++;
            printf("Match %c\n", a);
        }
        else if (X == 'E' && a == 'i') {
            pop();
            push('A');
            push('i');
            printf("E -> iA\n");
        }
        else if (X == 'A' && a == '+') {
            pop();
            push('A');
            push('i');
            push('+');
            printf("A -> +iA\n");
        }
        else if (X == 'A' && a == '$') {
            pop();
            printf("A -> epsilon\n");
        }
        else {
            printf("REJECT\n");
            return 0;
        }
    }

    printf("REJECT\n");
    return 0;
}
