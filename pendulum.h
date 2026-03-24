#pragma once

typedef struct {
    double angle;
    double angular_velocity;
} StateVec;

StateVec state_create(double initial_angle);
void     state_step_rk4(StateVec *s, double dt);