
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char stack[100];
char input[100];

int top = 0;
int pos = 0;

char precedence(char a, char b) {
    char table[6][6] = {
        // i    +    *    (    )    $
        {' ', '>', '>', ' ', '>', '>'}, // i
        {'<', '>', '<', '<', '>', '>'}, // +
        {'<', '>', '>', '<', '>', '>'}, // *
        {'<', '<', '<', '<', '=', ' '}, // (
        {' ', '>', '>', ' ', '>', '>'}, // )
        {'<', '<', '<', '<', ' ', '='}  // $
    };

    char symbols[] = {'i', '+', '*', '(', ')', '$'};

    int row = -1, col = -1;

    for (int i = 0; i < 6; i++) {
        if (symbols[i] == a) row = i;
        if (symbols[i] == b) col = i;
    }

    if (row == -1 || col == -1)
        return ' ';

    return table[row][col];
}

int isOperator(char c) {
    return c == '+' || c == '*' ||
           c == '(' || c == ')' || c == 'i' || c == '$';
}

void printStack() {
    for (int i = 0; i <= top; i++)
        printf("%c", stack[i]);
}

int reduce() {
    // E -> i
    if (stack[top] == 'i') {
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

    return 0;
}

int main() {
    printf("Enter an expression: ");
    scanf("%99s", input);

    int n = strlen(input);

    if (n == 0 || n >= 98) {
        printf("Invalid input length.\n");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        if (!isOperator(input[i]) || input[i] == '$') {
            printf("Invalid character in expression.\n");
            return 0;
        }
    }

    input[n] = '$';
    input[n + 1] = '\0';

    stack[0] = '$';

    printf("\nStack\t\tInput\t\tAction\n");
    printf("---------------------------------------------\n");

    while (1) {
        int i = top;

        // Find the topmost terminal
        while (i >= 0 &&
               stack[i] != 'i' &&
               stack[i] != '+' &&
               stack[i] != '*' &&
               stack[i] != '(' &&
               stack[i] != ')' &&
               stack[i] != '$') {
            i--;
        }

        if (i < 0) {
            printf("String Rejected\n");
            break;
        }

        char relation = precedence(stack[i], input[pos]);

        if (relation == '<' || relation == '=') {
            if (input[pos] == '$' && stack[top] == 'E' &&
                top == 1) {
                printf("\nString Accepted\n");
                break;
            }

            printf("%-16.*s %-16s Shift %c\n",
                   top + 1, stack, input + pos, input[pos]);

            if (top >= 98) {
                printf("Stack limit exceeded.\n");
                break;
            }

            stack[++top] = input[pos++];
        }
        else if (relation == '>') {
            if (reduce()) {
                printf("%-16.*s %-16s Reduce\n",
                       top + 1, stack, input + pos);
            } else {
                printf("\nString Rejected\n");
                break;
            }
        }
        else {
            printf("\nString Rejected\n");
            break;
        }
    }

    return 0;
}
