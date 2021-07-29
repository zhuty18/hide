#include <stdio.h>
#include <unistd.h>

int main() {
    int a = access("/proc/58", F_OK);
    printf("%d\n", a);
    return 0;
}