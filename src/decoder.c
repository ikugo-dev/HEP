#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

void readMetadata(FILE *read_ptr, unsigned short *img_width,
                  unsigned short *img_height, unsigned short *line_width);
void drawPixels(FILE *img, int img_width, int img_height, int line_width);
void renderImage(FILE *img, const char *img_name);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(1);
    }
    char *input_file = argv[1];
    printf("Input:  %s\n", input_file);

    // LOAD
    FILE *img_ptr = fopen(input_file, "rb");

    renderImage(img_ptr, input_file);

    // UNLOAD
    fclose(img_ptr);

    SetExitKey(KEY_ESCAPE);
    while (!WindowShouldClose()) {  // dont know what the issue is...
        // WaitTime(1);             // it just doesnt work...
    }
    CloseWindow();

    return 0;
}

void readMetadata(FILE *read_ptr, unsigned short *img_width,
                  unsigned short *img_height, unsigned short *line_width) {
    unsigned short metadata[3];
    fread(metadata, sizeof(metadata), 1, read_ptr);
    *img_width = metadata[0];
    *img_height = metadata[1];
    *line_width = metadata[2];
}

void drawPixels(FILE *img, int img_width, int img_height, int line_width) {
    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x += line_width) {
            unsigned char read_buffer[3];
            fread(read_buffer, sizeof(read_buffer), 1, img);
            Color c = {
                .r = read_buffer[0],
                .g = read_buffer[1],
                .b = read_buffer[2],
                .a = 255,
            };
            DrawLine(x, y, x + line_width, y, c);
        }
    }
}
void renderImage(FILE *img, const char *img_name) {
    unsigned short img_width, img_height, line_width;
    readMetadata(img, &img_width, &img_height, &line_width);

    InitWindow(img_width, img_height, img_name);
    BeginDrawing();
    drawPixels(img, img_width, img_height, line_width);
    EndDrawing();
}
