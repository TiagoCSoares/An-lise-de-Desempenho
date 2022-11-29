#include <stdio.h>

double x = 0.0;
int aux = 0;

int main() {
    for (int i = 0; i < 100000; i++) {
        x += 0.027;
        int aux = (int)x;
        if (x > 99.90 && x<= 100.9)
            printf("X = %d\n",aux);
    }
}

