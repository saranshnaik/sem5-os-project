#include "container.h"
#include "image.h"

int run_container(container_config *config) {
	char *stack = malloc(STACK_SIZE);
	if (!stack) {
		perror("malloc");
		return -1;
	}
	printf("Starting container with hostname: %s\n", config->hostname);
	int flags = SIGCHLD | CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWPID;

	// THIS LINE CREATES A NEW PROCESS TO RUN A CONTAINER
	pid_t pid = clone(setup_container, stack + STACK_SIZE, flags, config);
	if (pid == -1) {
		perror("clone");
		free(stack);
		return -1;
	}

	waitpid(pid, NULL, 0);
	free(stack);
	return 0;
}

int setup_container(void *arg) {
	container_config *config = (container_config *)arg;
	printf("Setting up container environment...\n");
	setup_hostname(config);
	setup_filesystem(config);
	setup_network(); //dummy
	printf("Executing command in container...\n");
	execvp(config->command[0], config->command);
	perror("execvp");
	exit(EXIT_FAILURE);
}

void setup_hostname(container_config *config) {
	if (sethostname(config->hostname, strlen(config->hostname)) == -1) {
		perror("sethostname");
	}
}

void setup_filesystem(container_config *config) {
	if (config->rootfs) {
		if (chroot(config->rootfs) != 0) {
			perror("chroot");
		}
	}
	if (chdir("/") != 0) { // root directory
		perror("chdir");
	}
}

void setup_network(void) {
	printf("Network namespace setup (basic implementation)\n");
}

int run_from_image(const char *image_name, container_config *config) {
	docker_image *image = load_image(image_name);
	if (!image) {
		return -1;
	}
	printf("Running image: %s\n", image->name);
	config->rootfs = image->path;
	char *entrypoint_cmd[] = {image->entrypoint, NULL};
	config->command = entrypoint_cmd;
	config->command_count = 1;
	
	return run_container(config);
}
