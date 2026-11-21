#include <stdio.h>
#include <string.h>

char input[100];
char stack[100];
int top = -1;

void shift(char symbol) {
    stack[++top] = symbol;
    stack[top + 1] = '\0';
}

int reduce() {
    // E -> i
    if (stack[top] == 'i') {
        stack[top] = 'E';
        return 1;
    }

    // E -> E+E
    if (top >= 2 &&
        stack[top] == 'E' &&
        stack[top - 1] == '+' &&
        stack[top - 2] == 'E') {
        top -= 2;
        stack[top] = 'E';
        return 1;
    }

    // E -> E*E
    if (top >= 2 &&
        stack[top] == 'E' &&
        stack[top - 1] == '*' &&
        stack[top - 2] == 'E') {
        top -= 2;
        stack[top] = 'E';
        return 1;
    }

    // E -> (E)
    if (top >= 2 &&
        stack[top] == ')' &&
        stack[top - 1] == 'E' &&
        stack[top - 2] == '(') {
        top -= 2;
        stack[top] = 'E';
        return 1;
    }

    return 0;
}

int main() {
    int i = 0;

    printf("Enter input string: ");
    scanf("%99s", input);

    printf("\nStack\t\tInput\t\tAction\n");
    printf("---------------------------------------------\n");

    while (input[i] != '\0') {
        shift(input[i]);

        printf("%-16s %-16s Shift %c\n",
               stack, input + i + 1, input[i]);

        i++;

        while (reduce()) {
            printf("%-16s %-16s Reduce\n",
                   stack, input + i);
        }
    }

    if (top == 0 && stack[0] == 'E') {
        printf("\nString Accepted\n");
    } else {
        printf("\nString Rejected\n");
    }

    return 0;
}
