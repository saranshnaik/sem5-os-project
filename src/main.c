#include "container.h"
#include "filesystem.h"
#include "image.h"
#include <getopt.h>

void print_usage(const char *program_name) {
	printf("Usage: %s [OPTIONS] COMMAND [ARGS...]\n", program_name);
	printf("	   %s run IMAGE_NAME\n", program_name);
	printf("	   %s build DOCKERFILE_PATH IMAGE_NAME\n", program_name);
	printf("	   %s images\n", program_name);
	// printf("\nOptions:\n");
	// printf("  -h, --hostname HOSTNAME  Set container hostname\n");
	//printf("  -r, --rootfs PATH		Set root filesystem path\n");
	//printf("  -i, --interactive		Run in interactive mode\n");
	//printf("  --init-rootfs PATH	   Initialize root filesystem\n");
	//printf("  --help				   Show this help message\n");
	//printf("\nCommands:\n");
	//printf("  run IMAGE_NAME		   Run a container from image\n");
	//printf("  build DOCKERFILE IMAGE   Build image from Dockerfile\n");
	//printf("  images				   List available images\n");
}

int main(int argc, char *argv[]) {
	container_config config = {
		.hostname = "minidocker-container",
		.rootfs = NULL,
		.command = NULL,
		.command_count = 0,
		.interactive = 0,
		.image_name = NULL
	};
	if (argc > 1) {
		if (strcmp(argv[1], "run") == 0) {
			if (argc < 3) {
				fprintf(stderr, "Error: Image name required\n");
				print_usage(argv[0]);
				return 1;
			}
			return run_from_image(argv[2], &config);
		}
		else if (strcmp(argv[1], "build") == 0) {
			if (argc < 4) {
				fprintf(stderr, "Error: Dockerfile path and image name required\n");
				print_usage(argv[0]);
				return 1;
			}
			return build_image(argv[2], argv[3]);
		}
		else if (strcmp(argv[1], "images") == 0) {
			return list_images();
		}
	}
	
	static struct option long_options[] = {
		{"hostname", required_argument, 0, 'h'},
		{"rootfs", required_argument, 0, 'r'},
		{"interactive", no_argument, 0, 'i'},
		{"init-rootfs", required_argument, 0, 1},
		{"help", no_argument, 0, 2},
		{0, 0, 0, 0}
	};
	
	int opt;
	int option_index = 0;
	
	while ((opt = getopt_long(argc, argv, "h:r:i", long_options, &option_index)) != -1) {
		switch (opt) {
			case 'h':
				config.hostname = optarg;
				break;
			case 'r':
				config.rootfs = optarg;
				break;
			case 'i':
				config.interactive = 1;
				break;
			case 1: // --init-rootfs
				if (create_rootfs(optarg) == 0) {
					printf("Successfully initialized rootfs at: %s\n", optarg);
				}
				return 0;
			case 2: // --help
				print_usage(argv[0]);
				return 0;
			default:
				print_usage(argv[0]);
				return 1;
		}
	}
	
	// remainig arguments are the command to run
	if (optind >= argc) {
		fprintf(stderr, "Error: No command specified\n");
		print_usage(argv[0]);
		return 1;
	}
	
	config.command = &argv[optind];
	config.command_count = argc - optind;
	
	return run_container(&config);
}
