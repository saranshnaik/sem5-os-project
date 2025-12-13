#include "image.h"
#include <errno.h>
#include <limits.h>

int create_image_directory(const char *image_name) {
	char image_path[MAX_PATH_LENGTH];
	int written = snprintf(image_path, sizeof(image_path), "%s/%s", IMAGE_DIR, image_name);
	if (written < 0 || (size_t)written >= sizeof(image_path)) {
		fprintf(stderr, "Image path too long\n");
		return -1;
	}
	if (mkdir(image_path, 0755) != 0 && errno != EEXIST) {
		perror("mkdir");
		return -1;
	}
	char bin_path[MAX_PATH_LENGTH]; // bin == binary (holds "binaries" of an app)
	written = snprintf(bin_path, sizeof(bin_path), "%s/bin", image_path);
	if (written < 0 || (size_t)written >= sizeof(bin_path)) {
		fprintf(stderr, "Bin path too long\n");
		return -1;
	}
	if (mkdir(bin_path, 0755) != 0 && errno != EEXIST) {
		perror("mkdir");
		return -1;
	}
	return 0;
}

int copy_file_to_image(const char *src_path, const char *image_name, const char *dest_filename) {
	char dest_path[MAX_PATH_LENGTH];
	int written = snprintf(dest_path, sizeof(dest_path), "%s/%s/bin/%s", IMAGE_DIR, image_name, dest_filename);
	if (written < 0 || (size_t)written >= sizeof(dest_path)) {
		fprintf(stderr, "Destination path too long\n");
		return -1;
	}
	FILE *src = fopen(src_path, "rb");
	FILE *dest = fopen(dest_path, "wb");
	
	if (!src || !dest) {
		if (src) fclose(src);
		if (dest) fclose(dest);
		return -1;
	}
	char buffer[4096];
	size_t bytes;
	while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
		fwrite(buffer, 1, bytes, dest);
	}
	fclose(src);
	fclose(dest);
	chmod(dest_path, 0755);	
	return 0;
}

int compile_c_program(const char *src_path, const char *output_path) {
	char output_dir[MAX_PATH_LENGTH];
	strncpy(output_dir, output_path, sizeof(output_dir) - 1);
	output_dir[sizeof(output_dir) - 1] = '\0';
	char *last_slash = strrchr(output_dir, '/');
	if (last_slash) {
		*last_slash = '\0'; // stop at last slash to get directory
		if (strlen(output_dir) > 0) {
			mkdir(output_dir, 0755);
		}
	}
	char command[1024];
	int written = snprintf(command, sizeof(command), "gcc -static -o %s %s", output_path, src_path);
	if (written < 0 || (size_t)written >= sizeof(command)) {
		fprintf(stderr, "Compile command too long\n");
		return -1;
	}
	printf("Compiling: %s\n", command);
	int result = system(command); // to execute command
	if (result != 0) {
		fprintf(stderr, "Compilation failed for %s\n", src_path);
		return -1;
	}
	return 0;
}

int build_image(const char *dockerfile_path, const char *image_name) {
	printf("Building image '%s' from %s\n", image_name, dockerfile_path);
	if (create_image_directory(image_name) != 0) {
		return -1;
	}
	FILE *dockerfile = fopen(dockerfile_path, "r");
	if (!dockerfile) {
		perror("fopen");
		return -1;
	}
	char line[256];
	char source_file[MAX_PATH_LENGTH] = "";
	char output_name[MAX_PATH_LENGTH] = "app";
	while (fgets(line, sizeof(line), dockerfile)) {
		line[strcspn(line, "\n")] = 0;
		if (strncmp(line, "COPY", 4) == 0) {
			sscanf(line, "COPY %255s", source_file);
		} 
		else if (strncmp(line, "RUN", 3) == 0) {
			if (strstr(line, "gcc")) {
				char *output_flag = strstr(line, "-o");
				if (output_flag) {
					sscanf(output_flag + 2, "%255s", output_name);
				}
			}
		}
	}
	fclose(dockerfile);
	if (strlen(source_file) == 0) {
		fprintf(stderr, "No COPY instruction found in Dockerfile\n");
		return -1;
	}
	char dockerfile_dir[MAX_PATH_LENGTH];
	strncpy(dockerfile_dir, dockerfile_path, sizeof(dockerfile_dir) - 1);
	dockerfile_dir[sizeof(dockerfile_dir) - 1] = '\0';
	char *dir = dirname(dockerfile_dir);
	// char full_path[MAX_PATH_LENGTH];
	char full_source_path[MAX_PATH_LENGTH];
	int written = snprintf(full_source_path, sizeof(full_source_path), "%s/%s", dir, source_file);
	if (written < 0 || (size_t)written >= sizeof(full_source_path)) {
		fprintf(stderr, "Source path too long\n");
		return -1;
	}
	char temp_output[MAX_PATH_LENGTH];
	written = snprintf(temp_output, sizeof(temp_output), "/tmp/minidocker_%s_%s", image_name, output_name);
	if (written < 0 || (size_t)written >= sizeof(temp_output)) {
		fprintf(stderr, "Temp output path too long\n");
		return -1;
	}
	if (compile_c_program(full_source_path, temp_output) != 0) {
		return -1;
	}
	if (copy_file_to_image(temp_output, image_name, output_name) != 0) {
		fprintf(stderr, "Failed to copy binary to image\n");
		return -1;
	}
	
	printf("Successfully built image '%s'\n", image_name);
	printf("Binary '%s' available in image\n", output_name);
	unlink(temp_output);
	return 0;
}

docker_image* load_image(const char *image_name) {
	static docker_image image;
	strncpy(image.name, image_name, sizeof(image.name) - 1);
	image.name[sizeof(image.name) - 1] = '\0';
	int written = snprintf(image.path, sizeof(image.path), "%s/%s", IMAGE_DIR, image_name);
	if (written < 0 || (size_t)written >= sizeof(image.path)) {
		fprintf(stderr, "Image path too long\n");
		return NULL;
	}
	strncpy(image.entrypoint, "/bin/app", sizeof(image.entrypoint) - 1);
	image.entrypoint[sizeof(image.entrypoint) - 1] = '\0';
	struct stat st;
	if (stat(image.path, &st) == -1) {
		fprintf(stderr, "Image '%s' not found\n", image_name);
		return NULL;
	}
	return &image;
}

int list_images(void) {
	printf("Available images:\n");
	struct stat st;
	if (stat(IMAGE_DIR, &st) == -1) {
		printf("No images directory found\n");
		return 0;
	}
	char command[512];
	snprintf(command, sizeof(command), "find %s -maxdepth 1 -type d | tail -n +2", IMAGE_DIR);
	printf("Images:\n");
	system(command);
	
	return 0;
}
