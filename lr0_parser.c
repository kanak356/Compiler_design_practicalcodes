
#include <stdio.h>
#include <string.h>

#define ACCEPT 999

int action[7][3] = {
    { 0,  0,  0},       // State 0 (filled below)
    { 0,  0, ACCEPT},   // State 1
    { 0,  0,  0},       // State 2
    { 0,  0,  0},       // State 3
    {-3, -3, -3},       // State 4: C -> d
    {-1, -1, -1},       // State 5: S -> CC
    {-2, -2, -2}        // State 6: C -> cC
};

int goTo[7][2] = {
    {1, 2},    // State 0: S, C
    {-1, -1},
    {-1, 5},   // State 2: C
    {-1, 6},   // State 3: C
    {-1, -1},
    {-1, -1},
    {-1, -1}
};

int productionLength[] = {0, 2, 2, 1};
char productionLeft[] = {' ', 'S', 'C', 'C'};

int getColumn(char symbol) {
    if (symbol == 'c') return 0;
    if (symbol == 'd') return 1;
    if (symbol == '$') return 2;
    return -1;
}

int getGotoColumn(char symbol) {
    if (symbol == 'S') return 0;
    if (symbol == 'C') return 1;
    return -1;
}

int main() {
    char input[100];
    int stack[100];
    int top = 0;
    int pos = 0;

    // State 0: shift c to state 3, d to state 4
    action[0][0] = 4;
    action[0][1] = 5;

    // State 2: shift c to state 3, d to state 4
    action[2][0] = 4;
    action[2][1] = 5;

    // State 3: shift c to state 3, d to state 4
    action[3][0] = 4;
    action[3][1] = 5;

    printf("Enter input string (only c and d): ");
    scanf("%99s", input);

    int len = strlen(input);

    if (len == 0 || len >= 98) {
        printf("Invalid input length.\n");
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (input[i] != 'c' && input[i] != 'd') {
            printf("Invalid character in input.\n");
            return 0;
        }
    }

    input[len] = '$';
    input[len + 1] = '\0';

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
            printf("%-16s %-16s Accept\n", "LR(0) Stack", input + pos);
            printf("\nString Accepted\n");
            break;
        }

        if (act > 0) {
            int nextState = act - 1;

            printf("State %d\t\t%s\t\tShift %c\n",
                   state, input + pos, input[pos]);

            stack[++top] = nextState;
            pos++;
        }
        else if (act < 0) {
            int rule = -act;
            int count = productionLength[rule];

            printf("State %d\t\t%s\t\tReduce by %c -> ",
                   state, input + pos, productionLeft[rule]);

            if (rule == 1) printf("CC\n");
            else if (rule == 2) printf("cC\n");
            else printf("d\n");

            top -= count;

            int currentState = stack[top];
            int lhs = getGotoColumn(productionLeft[rule]);

            if (lhs == -1 || goTo[currentState][lhs] == -1) {
                printf("String Rejected\n");
                break;
            }

            stack[++top] = goTo[currentState][lhs];
        }
        else {
            printf("\nString Rejected\n");
            break;
        }

        if (top >= 98) {
            printf("Stack limit exceeded.\n");
            break;
        }
    }

    return 0;
}
