#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

int   findLineWidth(Image *img, int compression_rate);
char *remove_extension(char *file_name);
void  writePayload(FILE *write_ptr, Image *img, int line_width);
void  writeMetadata(FILE *write_ptr, Image *img, int line_width);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <file name> <compression rate>/n", argv[0]);
        exit(1);
    }
    char *input_file = argv[1];
    printf("Input:  %s\n", input_file);
    char *output_file = strcat(remove_extension(input_file), ".hep");
    printf("Output: %s\n", output_file);

    // LOAD
    Image img = LoadImage(input_file);
    if (img.data == NULL) {
        perror("Cannot load image");
        exit(1);
    }
    FILE *output_img_ptr = fopen(output_file, "wb");
    if (output_img_ptr == NULL) {
        perror("Cannot open file");
        exit(1);
    }

    int compression_rate = atoi(argv[2]);
    int line_width = findLineWidth(&img, compression_rate);

    writeMetadata(output_img_ptr, &img, line_width);
    writePayload(output_img_ptr, &img, line_width);

    // UNLOAD
    UnloadImage(img);
    fclose(output_img_ptr);
    return 0;
}

int findLineWidth(Image *img, int compression_rate) {
    if (compression_rate < 0) {
        return 1;
    } else if (compression_rate > img->width / 2) {
        return img->width;
    }
    for (int line_width = 1; line_width < img->width; line_width++) {
        if (img->width % line_width == 0) {
            compression_rate--;
            if (compression_rate == 0) {
                return line_width;
            }
        }
    }
    return img->width;
}

void writePayload(FILE *write_ptr, Image *img, int line_width) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x <= (img->width - line_width); x += line_width) {
            unsigned int total_r = 0, total_g = 0, total_b = 0;
            for (int i = 0; i < line_width; i++) {
                Color c = GetImageColor(*img, x + i, y);
                total_r += c.r;
                total_g += c.g;
                total_b += c.b;
            }
            unsigned char new_color[3] = {
                total_r / line_width,
                total_g / line_width,
                total_b / line_width,
            };
            fwrite(new_color, sizeof(new_color), 1, write_ptr);
        }
    }
}

void writeMetadata(FILE *write_ptr, Image *img, int line_width) {
    unsigned short metadata[3] = {
        img->width,
        img->height,
        line_width,
    };
    fwrite(metadata, sizeof(metadata), 1, write_ptr);
}

char *remove_extension(char *file_name) {
    if (file_name == NULL) return NULL;
    char *file_base = malloc(strlen(file_name) + 1);
    if (file_base == NULL) return NULL;
    strcpy(file_base, file_name);

    char *dot_position = strrchr(file_base, '.');
    if (dot_position != NULL) *dot_position = '\0';
    return file_base;
}
