#include "physics.hpp"

PhysicsEngine::PhysicsEngine() {

}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::update(std::vector<Particle>& particles, float dt, bool isFirstStep) {
    const vec2 gravity{0.f, 400.f};

    for(auto& particle : particles) {
        if (isFirstStep) {
            particle.prev_pos = particle.curr_pos - vec2(0.0f, 0.0f);
        }

        vec2 vel = particle.curr_pos - particle.prev_pos;
        
        vec2 temp = particle.curr_pos;
        particle.accel += gravity;
        particle.curr_pos = particle.curr_pos + vel + particle.accel * (dt * dt);
        particle.prev_pos = temp;
    }
}