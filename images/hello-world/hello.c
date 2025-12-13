#include <stdio.h>
#include <unistd.h>

int main() {
	printf("Hello World from Minidocker Container!\n");
	printf("This is running from a custom container image\n");
	printf("Container PID: %d\n", getpid());
	
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == 0) {
		printf("Container Hostname: %s\n", hostname);
	}
	
	printf("Demonstration successful!\n");
	return 0;
}
