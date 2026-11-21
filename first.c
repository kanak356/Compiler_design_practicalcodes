#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char production[MAX][MAX];
char first[MAX][MAX];
int n;

int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

int contains(char set[], char c) {
    for (int i = 0; set[i] != '\0'; i++) {
        if (set[i] == c)
            return 1;
    }
    return 0;
}

void add(char set[], char c) {
    if (!contains(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

int findProduction(char symbol) {
    for (int i = 0; i < n; i++) {
        if (production[i][0] == symbol)
            return i;
    }
    return -1;
}

void findFirst(char symbol, char result[]) {

    if (!isNonTerminal(symbol)) {
        add(result, symbol);
        return;
    }

    for (int i = 0; i < n; i++) {

        if (production[i][0] == symbol) {

            char *rhs = strchr(production[i], '>');

            if (rhs == NULL)
                continue;

            rhs++;

            if (*rhs == '#') {
                add(result, '#');
                continue;
            }

            int nullable = 1;

            for (int j = 0; rhs[j] != '\0'; j++) {

                char temp[MAX] = "";

                findFirst(rhs[j], temp);

                for (int k = 0; temp[k] != '\0'; k++) {
                    if (temp[k] != '#')
                        add(result, temp[k]);
                }

                if (!contains(temp, '#')) {
                    nullable = 0;
                    break;
                }
            }

            if (nullable)
                add(result, '#');
        }
    }
}

int main() {

    printf("Enter number of productions: ");
    scanf("%d", &n);

    if (n <= 0 || n >= MAX) {
        printf("Invalid number of productions.\n");
        return 1;
    }

    printf("\nEnter productions (example: E->TR):\n");

    for (int i = 0; i < n; i++) {
        scanf("%19s", production[i]);
    }

    printf("\nFIRST SETS:\n");

    for (int i = 0; i < n; i++) {

        char symbol = production[i][0];

        if (findProduction(symbol) == i) {

            char result[MAX] = "";

            findFirst(symbol, result);

            printf("FIRST(%c) = { ", symbol);

            for (int j = 0; result[j] != '\0'; j++) {
                printf("%c ", result[j]);
            }

            printf("}\n");
        }
    }

    return 0;
}
