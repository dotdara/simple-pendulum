#pragma once

typedef struct {
    double angle;
    double angular_velocity;
} StateVec;

typedef struct {
    StateVec state;
    double length;
} Pendulum;

StateVec state_create(double initial_angle);
void pendulum_step(Pendulum *pendulum, double g, double dt);