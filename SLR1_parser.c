
#include <stdio.h>
#include <string.h>

#define ACCEPT 999

// Columns: i, +, *, (, ), $
int action[12][6] = {
    { 6,  0,  0,  5,  0,   0},       // State 0
    { 0,  7,  0,  0,  0, ACCEPT},   // State 1
    { 0, -2,  8,  0, -2,  -2},      // State 2
    { 0, -4, -4,  0, -4,  -4},      // State 3
    { 6,  0,  0,  5,  0,   0},      // State 4
    { 0, -6, -6,  0, -6,  -6},      // State 5
    { 6,  0,  0,  5,  0,   0},      // State 6
    { 6,  0,  0,  5,  0,   0},      // State 7
    { 0,  7,  0,  0, 12,   0},      // State 8
    { 0, -1,  8,  0, -1,  -1},      // State 9
    { 0, -3, -3,  0, -3,  -3},      // State 10
    { 0, -5, -5,  0, -5,  -5}       // State 11
};

// GOTO columns: E, T, F
int goTo[12][3] = {
    {1, 2, 3},
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1},
    {8, 2, 3},
    {-1, -1, -1},
    {-1, 9, 3},
    {-1, -1, 10},
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1}
};

// Production lengths
int length[] = {0, 3, 1, 3, 1, 3, 1};

// Production left-hand sides
char lhs[] = {' ', 'E', 'E', 'T', 'T', 'F', 'F'};

int getColumn(char symbol) {
    switch (symbol) {
        case 'i': return 0;
        case '+': return 1;
        case '*': return 2;
        case '(': return 3;
        case ')': return 4;
        case '$': return 5;
        default: return -1;
    }
}

int getGotoColumn(char symbol) {
    switch (symbol) {
        case 'E': return 0;
        case 'T': return 1;
        case 'F': return 2;
        default: return -1;
    }
}

void displayStack(int stack[], int top) {
    int i;
    for (i = 0; i <= top; i++) {
        printf("%d ", stack[i]);
    }
}

int main() {
    char input[100];
    int stack[100];
    int top = 0;
    int pos = 0;
    int i;

    printf("Enter input string: ");
    scanf("%99s", input);

    int n = strlen(input);

    if (n == 0 || n >= 98) {
        printf("Invalid input length.\n");
        return 0;
    }

    // Validate input characters
    for (i = 0; i < n; i++) {
        if (getColumn(input[i]) < 0 ||
            input[i] == '$') {
            printf("Invalid input character.\n");
            return 0;
        }
    }

    // Append end marker
    input[n] = '$';
    input[n + 1] = '\0';

    stack[0] = 0;

    printf("\nStack\t\tInput\t\tAction\n");
    printf("---------------------------------------------\n");

    while (1) {
        int state = stack[top];
        int col = getColumn(input[pos]);

        if (col == -1) {
            printf("String Rejected\n");
            break;
        }

        int act = action[state][col];

        if (act == ACCEPT) {
            displayStack(stack, top);
            printf("\t%s\tAccept\n", input + pos);
            printf("\nString Accepted\n");
            break;
        }

        if (act > 0) {
            int nextState = act - 1;

            if (top >= 98) {
                printf("Stack limit exceeded.\n");
                break;
            }

            displayStack(stack, top);
            printf("\t%s\tShift %c\n", input + pos, input[pos]);

            stack[++top] = nextState;
            pos++;
        }
        else if (act < 0) {
            int rule = -act;

            displayStack(stack, top);
            printf("\t%s\tReduce by %c\n",
                   input + pos, lhs[rule]);

            top -= length[rule];

            int currentState = stack[top];
            int gotoCol = getGotoColumn(lhs[rule]);

            if (gotoCol == -1 ||
                goTo[currentState][gotoCol] == -1) {
                printf("String Rejected\n");
                break;
            }

            stack[++top] = goTo[currentState][gotoCol];
        }
        else {
            displayStack(stack, top);
            printf("\t%s\tError\n", input + pos);
            printf("\nString Rejected\n");
            break;
        }
    }

    return 0;
}
