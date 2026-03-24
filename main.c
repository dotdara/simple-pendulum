#include <raylib.h>
#include "pendulum.h"
#include "render.h"

#define WIDTH  800
#define HEIGHT 600

int main() {
    StateVec state = state_create(0.2);

    InitWindow(WIDTH, HEIGHT, "Simple Pendulum");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        state_step_rk4(&state, GetFrameTime());
        render_frame(state);
    }

    CloseWindow();
    return 0;
}