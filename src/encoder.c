#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
const int line_width = 10;  // TEMP

char *remove_extension(char *file_name) {
    if (file_name == NULL) return NULL;
    char *file_base = malloc(strlen(file_name) + 1);
    if (file_base == NULL) return NULL;
    strcpy(file_base, file_name);

    char *dot_position = strrchr(file_base, '.');
    if (dot_position != NULL) *dot_position = '\0';
    return file_base;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(1);
    }
    char *input_file = argv[1];
    printf("Input:  %s\n", input_file);
    char *output_file = strcat(remove_extension(input_file), ".hep");
    printf("Output: %s\n", output_file);

    Image img = LoadImage(input_file);
    if (img.data == NULL) {
        exit(1);
    }
    FILE *write_ptr = fopen(output_file, "wb");

    // TODO! Write metadata

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < (img.width - line_width); x++) {
            unsigned int total_r = 0, total_g = 0, total_b = 0, total_a = 0;
            for (int i = 0; i < line_width; i++) {
                Color c = GetImageColor(img, x, y);
                total_r += c.r;
                total_g += c.g;
                total_b += c.b;
                total_a += c.a;
                x++;
            }
            unsigned char new_color[4] = {
                total_r / line_width,
                total_g / line_width,
                total_b / line_width,
                total_a / line_width,
            };
            fwrite(new_color, sizeof(new_color), 1, write_ptr);
        }
    }

    fclose(write_ptr);
    return 0;
}
