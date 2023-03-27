#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* args[]) {
    srand(5);
    for (int i = 0; i < 10; i++) {

        int random = rand() % 10 + 124;
        printf("%i\n",random);
    }

    return 0;
}