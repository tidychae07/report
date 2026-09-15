#include <stdio.h>

int main(void) {
    int n;
    scanf("%d", &n);

    int mid = n / 2;

    for (int i = 0; i <= mid; i++) {
        for (int j = 0; j < i; j++) {
            printf(" ");
        }
        for (int j = 0; j < n - 2 * i; j++) {
            printf("*");
        }
        printf("\n");
    }

    for (int i = mid - 1; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            printf(" ");
        }
        for (int j = 0; j < n - 2 * i; j++) {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
