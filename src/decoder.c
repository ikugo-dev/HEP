#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(1);
    }
    char *input_file = argv[1];
    printf("Input:  %s\n", input_file);

    FILE *read_ptr = fopen(input_file, "rb");

    unsigned short metadata[3];
    fread(metadata, sizeof(metadata), 1, read_ptr);
    unsigned short img_width = metadata[0];
    unsigned short img_height = metadata[1];
    unsigned short line_width = metadata[2];

    InitWindow(img_width, img_height, "WINDOW");
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x += line_width) {
            unsigned char read_buffer[3];
            fread(read_buffer, sizeof(read_buffer), 1, read_ptr);
            Color c = {
                .r = read_buffer[0],
                .g = read_buffer[1],
                .b = read_buffer[2],
                .a = 255,
            };
            DrawLine(x, y, x + line_width, y, c);
        }
    }
    EndDrawing();

    fclose(read_ptr);

    while (!WindowShouldClose()) {
        WaitTime(1);
    }
    CloseWindow();

    return 0;
}
