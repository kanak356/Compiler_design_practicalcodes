
#include <stdio.h>
#include <string.h>

#define ACCEPT 999

/*
   CLR(1) ACTION TABLE
   Columns: c, d, $
   Positive value = Shift (state = value - 1)
   Negative value = Reduce (production = -value)
   999 = Accept
*/

int action[10][3] = {
    {4, 5, 0},       // State 0
    {0, 0, ACCEPT},  // State 1
    {7, 8, 0},       // State 2
    {4, 5, 0},       // State 3
    {-3, -3, 0},     // State 4
    {0, 0, -1},      // State 5
    {7, 8, 0},       // State 6
    {0, 0, -3},      // State 7
    {-2, -2, 0},     // State 8
    {0, 0, -2}       // State 9
};

/*
   GOTO TABLE
   Columns: S, C
*/

int goTo[10][2] = {
    {1, 2},
    {-1, -1},
    {-1, 5},
    {-1, 8},
    {-1, -1},
    {-1, -1},
    {-1, 9},
    {-1, -1},
    {-1, -1},
    {-1, -1}
};

/*
   Productions:
   1. S -> CC
   2. C -> cC
   3. C -> d
*/

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
    int i;

    printf("Enter input string (c and d only): ");
    scanf("%99s", input);

    int n = strlen(input);

    if (n == 0 || n >= 98) {
        printf("Invalid input length.\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        if (input[i] != 'c' && input[i] != 'd') {
            printf("Invalid input character.\n");
            return 0;
        }
    }

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
            printf("State %d\t\t%s\t\tAccept\n",
                   state, input + pos);
            printf("\nString Accepted\n");
            break;
        }

        if (act > 0) {
            int nextState = act - 1;

            if (top >= 98) {
                printf("Stack limit exceeded.\n");
                break;
            }

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

            if (rule == 1)
                printf("CC\n");
            else if (rule == 2)
                printf("cC\n");
            else
                printf("d\n");

            top -= count;

            int currentState = stack[top];
            int gotoCol = getGotoColumn(productionLeft[rule]);

            if (gotoCol == -1 ||
                goTo[currentState][gotoCol] == -1) {
                printf("String Rejected\n");
                break;
            }

            stack[++top] = goTo[currentState][gotoCol];
        }
        else {
            printf("State %d\t\t%s\t\tError\n",
                   state, input + pos);
            printf("\nString Rejected\n");
            break;
        }
    }

    return 0;
}
