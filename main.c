#include <raylib.h>
#include "pendulum.h"
#include "render.h"

#define G 9.81
#define L 1.0

#define WIDTH  800
#define HEIGHT 600

int main() {
    StateVec state = state_create(0.2);
    Pendulum pendulum = (Pendulum) { .state = state, .length = L};

    InitWindow(WIDTH, HEIGHT, "Simple Pendulum");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        pendulum_step(&pendulum, G, GetFrameTime());
        render_frame(state);
    }

    CloseWindow();
    return 0;
}