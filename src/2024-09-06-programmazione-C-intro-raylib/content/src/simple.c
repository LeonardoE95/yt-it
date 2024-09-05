#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose()) {      
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("HELLO WORLD!", 0, 0, 50, BLACK);
        EndDrawing();	
    }

    CloseWindow();

    return 0;
}
