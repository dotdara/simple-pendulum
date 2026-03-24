#include "pendulum.h"
#include <math.h>

static StateVec derivatives(StateVec s, double length, double g) {
    return (StateVec){
        .angle            = s.angular_velocity,
        .angular_velocity = -(g / length) * sin(s.angle)
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

void pendulum_step (Pendulum *pendulum, double g, double dt) {
    StateVec k1 = derivatives(pendulum->state, pendulum->length, g);
    StateVec k2 = derivatives(vec_add(pendulum->state, vec_scale(k1, dt / 2)), pendulum->length, g);
    StateVec k3 = derivatives(vec_add(pendulum->state, vec_scale(k2, dt / 2)), pendulum->length, g);
    StateVec k4 = derivatives(vec_add(pendulum->state, vec_scale(k3, dt)), pendulum->length, g);

    StateVec delta = vec_scale(
        vec_add(k1, vec_add(vec_scale(k2, 2), vec_add(vec_scale(k3, 2), k4))),
        dt / 6.0
    );
    pendulum->state = vec_add(pendulum->state, delta);
}