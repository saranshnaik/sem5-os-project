#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Hello World from Minidocker Container!\n");
    printf("Container ID: %d\n", getpid());
    printf("This program is running in an isolated environment\n");
    
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("Hostname: %s\n", hostname);
    }
    
    return 0;
}
