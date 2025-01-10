#pragma once
#include "math/simMath.hpp"

struct Particle {
    vec2 curr_pos;
    vec2 prev_pos;
    vec2 accel;
    vec2 targetPos;
    float radius;
    bool isPlayer;
};