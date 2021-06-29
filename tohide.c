#include <stdio.h>

int main() {
    int pid = getpid();
    printf("%d\n", pid);
    while (1) {
        sleep(1);
    }
    return 0;
}