#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20
#define SIZE 50

char lhs[MAX];
char rhs[MAX][SIZE];
int n;

int first[26][128];
int follow[26][128];

int isNT(char c) {
    return c >= 'A' && c <= 'Z';
}

void calculateFirst() {
    int changed = 1;

    while (changed) {
        changed = 0;

        for (int i = 0; i < n; i++) {
            int a = lhs[i] - 'A';
            char *s = rhs[i];
            int nullable = 1;

            for (int j = 0; s[j] != '\0'; j++) {
                char c = s[j];

                if (c == '#') {
                    if (!first[a]['#']) {
                        first[a]['#'] = 1;
                        changed = 1;
                    }
                    break;
                }

                if (!isNT(c)) {
                    if (!first[a][(unsigned char)c]) {
                        first[a][(unsigned char)c] = 1;
                        changed = 1;
                    }
                    nullable = 0;
                    break;
                }

                int b = c - 'A';

                for (int k = 0; k < 128; k++) {
                    if (k != '#' && first[b][k] &&
                        !first[a][k]) {
                        first[a][k] = 1;
                        changed = 1;
                    }
                }

                if (!first[b]['#']) {
                    nullable = 0;
                    break;
                }
            }

            if (nullable && !first[a]['#']) {
                first[a]['#'] = 1;
                changed = 1;
            }
        }
    }
}

void firstOfSuffix(char s[], int pos, int result[]) {
    int nullable = 1;

    for (int i = pos; s[i] != '\0'; i++) {
        char c = s[i];

        if (c == '#') {
            result['#'] = 1;
            return;
        }

        if (!isNT(c)) {
            result[(unsigned char)c] = 1;
            nullable = 0;
            break;
        }

        int b = c - 'A';

        for (int k = 0; k < 128; k++) {
            if (k != '#' && first[b][k])
                result[k] = 1;
        }

        if (!first[b]['#']) {
            nullable = 0;
            break;
        }
    }

    if (nullable)
        result['#'] = 1;
}

void calculateFollow() {
    follow[lhs[0] - 'A']['$'] = 1;

    int changed = 1;

    while (changed) {
        changed = 0;

        for (int i = 0; i < n; i++) {
            int a = lhs[i] - 'A';

            for (int j = 0; rhs[i][j] != '\0'; j++) {

                if (isNT(rhs[i][j])) {

                    int b = rhs[i][j] - 'A';
                    int temp[128] = {0};

                    firstOfSuffix(rhs[i], j + 1, temp);

                    for (int k = 0; k < 128; k++) {

                        if (k != '#' && temp[k] &&
                            !follow[b][k]) {
                            follow[b][k] = 1;
                            changed = 1;
                        }
                    }

                    if (temp['#']) {
                        for (int k = 0; k < 128; k++) {

                            if (follow[a][k] &&
                                !follow[b][k]) {
                                follow[b][k] = 1;
                                changed = 1;
                            }
                        }
                    }
                }
            }
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
        char p[SIZE];

        scanf("%49s", p);

        char *arrow = strstr(p, "->");

        if (arrow == NULL || arrow != p + 1) {
            printf("Invalid production format.\n");
            return 1;
        }

        lhs[i] = p[0];
        strcpy(rhs[i], arrow + 2);

        if (!isNT(lhs[i]) || rhs[i][0] == '\0') {
            printf("Invalid production.\n");
            return 1;
        }
    }

    calculateFirst();
    calculateFollow();

    printf("\nFOLLOW SETS:\n");

    for (int i = 0; i < n; i++) {

        int a = lhs[i] - 'A';
        int alreadyPrinted = 0;

        for (int j = 0; j < i; j++) {
            if (lhs[j] == lhs[i])
                alreadyPrinted = 1;
        }

        if (!alreadyPrinted) {
            printf("FOLLOW(%c) = { ", lhs[i]);

            for (int k = 0; k < 128; k++) {
                if (follow[a][k])
                    printf("%c ", k);
            }

            printf("}\n");
        }
    }

    return 0;
}
