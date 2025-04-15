#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

char *remove_extension(char *file_name);
void  readMetadata(FILE *read_ptr, unsigned short *img_width,
                   unsigned short *img_height, unsigned short *line_width);
void  renderImage(unsigned char *color_data, int img_width, int img_height,
                  int line_width);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(1);
    }
    char *input_file = argv[1];
    printf("Input:  %s\n", input_file);

    // LOAD
    FILE          *img_ptr = fopen(input_file, "rb");
    unsigned short img_width, img_height, line_width;
    unsigned short metadata[3];
    fread(metadata, sizeof(metadata), 1, img_ptr);
    img_width = metadata[0];
    img_height = metadata[1];
    line_width = metadata[2];

    unsigned int   color_data_size = img_width * img_height * 3;
    unsigned char *color_data = malloc(color_data_size);
    if (color_data == NULL) {
        perror("Cannot malloc color data");
        exit(1);
    }
    fread(color_data, color_data_size, 1, img_ptr);
    fclose(img_ptr);

    InitWindow(img_width, img_height, input_file);
    SetTargetFPS(1);
    SetWindowPosition(200, 300);

    // SetExitKey(KEY_ESCAPE);
    float zoom = 1;
    while (!WindowShouldClose()) {
        renderImage(color_data, img_width * zoom, img_height * zoom,
                    line_width * zoom);
        if (IsKeyPressed(KEY_LEFT_BRACKET)) {
            zoom /= 2;
        } else if (IsKeyPressed(KEY_RIGHT_BRACKET)) {
            zoom *= 2;
        }
        if (IsKeyPressed(KEY_S)) {
            TakeScreenshot(
                strcat("./screenshots/",
                       strcat(remove_extension(input_file), ".png")));
        }
    }
    CloseWindow();

    // UNLOAD
    return 0;
}

void renderImage(unsigned char *color_data, int img_width, int img_height,
                 int line_width) {
    // static int previous_width;
    // if (previous_width == img_width) {
    //     return;
    // }
    // previous_width = img_width;
    SetWindowSize(img_width, img_height);
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x += line_width) {
            Color c = {
                .r = color_data[(x + (y * img_width)) / line_width * 3 + 0],
                .g = color_data[(x + (y * img_width)) / line_width * 3 + 1],
                .b = color_data[(x + (y * img_width)) / line_width * 3 + 2],
                .a = 255,
            };
            DrawLine(x, y, x + line_width, y, c);
        }
    }
    EndDrawing();
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
