#pragma once
#include "core/common.hpp"

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    void applyAccel(Particle& particle, vec2 accel) {
        particle.accel = accel;
    }

    void setPos(Particle& particle, vec2 pos) {
        particle.curr_pos = pos;
    }

    void update(std::vector<Particle>& particles, float dt, bool isFirstStep);
private:

};