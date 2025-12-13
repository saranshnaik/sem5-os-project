#ifndef CONTAINER_H
#define CONTAINER_H

#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define STACK_SIZE (1024 * 1024)

// Container config
typedef struct {
	char *hostname;
	char *rootfs;
	char **command;
	int command_count;
	int interactive;
	char *image_name;
} container_config;

// Functions
int run_container(container_config *config);
int setup_container(void *arg);
void setup_filesystem(container_config *config);
void setup_network(void);
void setup_hostname(container_config *config);
int run_from_image(const char *image_name, container_config *config);

#endif
