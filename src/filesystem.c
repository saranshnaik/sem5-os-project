#include "filesystem.h"

int create_rootfs(const char *path) {
	struct stat st = {0};
	if (stat(path, &st) == -1) {
		if (mkdir(path, 0755) != 0) {
			perror("mkdir");
			return -1;
		}
		printf("Created rootfs directory: %s\n", path);
	}
	char subdir[256];
	const char *dirs[] = {"bin", "lib", "lib64", "usr", "etc", "tmp", NULL};
	for (int i = 0; dirs[i] != NULL; i++) {
		snprintf(subdir, sizeof(subdir), "%s/%s", path, dirs[i]);
		if (stat(subdir, &st) == -1) {
			if (mkdir(subdir, 0755) != 0) {
				perror("mkdir");
			} else {
				printf("Created directory: %s\n", subdir);
			}
		}
	}
	return 0;
}

int copy_file(const char *src, const char *dest) {
	FILE *source = fopen(src, "rb");
	FILE *destination = fopen(dest, "wb");
	if (!source || !destination) {
		if (source) {
			fclose(source);
		}
		if (destination) {
			fclose(destination);
		}
		return -1;
	}
	char buffer[4096];
	size_t bytes;
	while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
		fwrite(buffer, 1, bytes, destination);
	}
	fclose(source);
	fclose(destination);
	chmod(dest, 0755); // read-write permissions
	return 0;
}
