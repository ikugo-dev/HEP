#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
const int line_width = 10;  // TEMP
const int img_width = 320;
const int img_height = 240;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(1);
    }
    char *input_file = argv[1];
    printf("Input:  %s\n", input_file);

    FILE *read_ptr = fopen(input_file, "rb");

    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "WINDOW");

    BeginDrawing();  // dont need??
    ClearBackground(RAYWHITE);
    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x += line_width) {
            unsigned char read_buffer[4];
            fread(read_buffer, sizeof(read_buffer), 1, read_ptr);
            Color c = {
                .r = read_buffer[0],
                .g = read_buffer[1],
                .b = read_buffer[2],
                .a = read_buffer[3],
            };
            DrawLine(x, y, x + line_width, y, c);
        }
    }
    EndDrawing();  // dont need??

    while (!WindowShouldClose()) {
        WaitTime(1);
    }
    CloseWindow();

    fclose(read_ptr);
    return 0;
}
