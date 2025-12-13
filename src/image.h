#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

#define MAX_PATH_LENGTH 512
#define IMAGE_DIR "images"

typedef struct {
	char name[64];
	char path[MAX_PATH_LENGTH];
	char entrypoint[MAX_PATH_LENGTH];
} docker_image;

int build_image(const char *dockerfile_path, const char *image_name);
int create_image_directory(const char *image_name);
int copy_file_to_image(const char *src_path, const char *image_name, const char *dest_filename);
int compile_c_program(const char *src_path, const char *output_path);
docker_image* load_image(const char *image_name);
int list_images(void);

#endif
