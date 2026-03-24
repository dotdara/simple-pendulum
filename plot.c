#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <plplot/plplot.h>

// #include <raylib.h>

// #define HEIGHT 600
// #define WIDTH 800

const double g = 9.81;
const double l = 1.0;

typedef struct {
    double *items;
    size_t count;
    size_t capacity;
} Series;

typedef struct
{
    double angle;
    double angular_velocity;
    // double angular_acceleration;
} StateVec;

StateVec state_create(double initial_angle)
{
    StateVec state = (StateVec) 
    {
        .angle = initial_angle,
        .angular_velocity = 0,
        // .angular_acceleration = 0
    };
    return state;
}

void state_step(StateVec *state, double dt)
{
    double angle = state->angle; 
    state->angle += dt*state->angular_velocity;
    state->angular_velocity -= dt*(g/l)*angle;
}

StateVec derivatives(StateVec s)
{
    return (StateVec) {
        .angle = s.angular_velocity,
        .angular_velocity = -(g/l) * s.angle
    };
}

StateVec vec_add(StateVec a, StateVec b) 
{
    return (StateVec){ a.angle + b.angle, a.angular_velocity + b.angular_velocity };
}

StateVec vec_scale(StateVec a, double s)
{
    return (StateVec){ a.angle * s, a.angular_velocity * s };
}

void state_step_rk4(StateVec *s, double dt)
{
    StateVec k1 = derivatives(*s);
    StateVec k2 = derivatives(vec_add(*s, vec_scale(k1, dt/2)));
    StateVec k3 = derivatives(vec_add(*s, vec_scale(k2, dt/2)));
    StateVec k4 = derivatives(vec_add(*s, vec_scale(k3, dt)));

    // 1/6 * (k1 + 2k2 + 2k3 + k4)
    s->angle += (dt / 6.0) * (k1.angle + 2*k2.angle + 2*k3.angle + k4.angle);
    s->angular_velocity += (dt / 6.0) * (k1.angular_velocity + 2*k2.angular_velocity + 2*k3.angular_velocity + k4.angular_velocity);
}

void state_display(StateVec *state)
{
    printf("angle = %lf\n", state->angle);
}

double series_min(const Series *ser)
{
    if (ser->count == 0) return 0.0;

    double minimum = ser->items[0];
    for (size_t i = 1; i < ser->count; i++) {
        if (ser->items[i] < minimum) {
            minimum = ser->items[i];
        }
    }

    return minimum;
}

double series_max(const Series *ser)
{
    if (ser->count == 0) return 0.0;

    double maximum = ser->items[0];
    for (size_t i = 1; i < ser->count; i++) {
        if (ser->items[i] > maximum) {
            maximum = ser->items[i];
        }
    }

    return maximum;
}

void plot(Series *x, Series *y)
{
    if (x->count == 0 || y->count == 0) return;

    assert(x->count == y->count && "X and Y series must have the same length");

    double xmin = series_min(x);
    double xmax = series_max(x);
    double ymin = series_min(y);
    double ymax = series_max(y);

    double x_pad = (xmax - xmin) * 0.1;
    double y_pad = (ymax - ymin) * 0.1;
    
    plsetopt("dev", "xcairo");
    plstar(1, 1);
    plenv(xmin - x_pad, xmax + x_pad, ymin - y_pad, ymax + y_pad, 0, 0);
    pllab("t(s)", "angle (deg)", "Pendulum");

    plline(x->count, x->items, y->items);

    plend();   
}

int main(void)
{
    // StateVec euler = state_create(0.1);
    StateVec state = state_create(0.1);
    double dt = 0.0001;

    size_t num_steps = 50/dt;

    double *t = malloc(num_steps * sizeof(double));
    double *angles = malloc(num_steps * sizeof(double));

    for (size_t i = 0; i < num_steps; i++)
    {  
       state_step_rk4(&state, dt); 

       t[i] = i*dt;
       angles[i] = state.angle;       
    }

    for (size_t i = 0; i < num_steps; i++)
    {  
       angles[i] = (180/3.14159)*angles[i];
    }

    Series x = {0};
    Series y = {0};

    for (size_t i = 0; i < num_steps; i++) {
        nob_da_append(&x, t[i]);
    }
    for (size_t i = 0; i < num_steps; i++) {
        nob_da_append(&y, angles[i]);
    }

    plot(&x, &y);
    return 0;
}
