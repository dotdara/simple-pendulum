#include "pendulum.h"
#include <math.h>

#define G 9.81
#define L 1.0

static StateVec derivatives(StateVec s) {
    return (StateVec){
        .angle            = s.angular_velocity,
        .angular_velocity = -(G / L) * sin(s.angle)
    };
}

static StateVec vec_add(StateVec a, StateVec b) {
    return (StateVec){ a.angle + b.angle, a.angular_velocity + b.angular_velocity };
}

static StateVec vec_scale(StateVec a, double s) {
    return (StateVec){ a.angle * s, a.angular_velocity * s };
}

StateVec state_create(double initial_angle) {
    return (StateVec){ .angle = initial_angle, .angular_velocity = 0 };
}

void state_step_rk4(StateVec *s, double dt) {
    StateVec k1 = derivatives(*s);
    StateVec k2 = derivatives(vec_add(*s, vec_scale(k1, dt / 2)));
    StateVec k3 = derivatives(vec_add(*s, vec_scale(k2, dt / 2)));
    StateVec k4 = derivatives(vec_add(*s, vec_scale(k3, dt)));

    StateVec delta = vec_scale(
        vec_add(k1, vec_add(vec_scale(k2, 2), vec_add(vec_scale(k3, 2), k4))),
        dt / 6.0
    );
    *s = vec_add(*s, delta);
}