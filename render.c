#include "render.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>

#define WIDTH  800
#define HEIGHT 600
#define METRE  100
#define L      1.0

#define PIVOT_RADIUS 2.0f
#define BOB_RADIUS   9.0f

static Vector2 to_world(Vector2 v) {
    Vector2 scaled    = Vector2Scale(v, METRE);
    Vector2 translate = { WIDTH / 2.0f, HEIGHT / 2.0f - (L / 2.0 * METRE) };
    return Vector2Add(scaled, translate);
}

void render_frame(StateVec state) {
    Vector2 pivot = to_world((Vector2){ 0, 0 });
    Vector2 bob   = to_world((Vector2){
        .x = L * sin(state.angle),
        .y = L * cos(state.angle)
    });

    BeginDrawing();
        ClearBackground(BLUE);
        DrawCircleV(pivot, PIVOT_RADIUS, GREEN);
        DrawLineV(pivot, bob, GREEN);
        DrawCircleV(bob, BOB_RADIUS, RED);
    EndDrawing();
}