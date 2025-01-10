#pragma once
#include "core/common.hpp"
#include "glad/glad.h"

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
    void init(size_t maxParticles);
    void update(std::vector<Particle>& particles, EnginePerformanceData& perf, float dt);
    void setWorldSize(vec2 size) { worldSize = size; }
private:
    ComputeShader collShader;
     
    struct GpuCollisionData {
        float x;
        float y;
        float r;
    };
    
    unsigned int inputBuffer;
    unsigned int outputBuffer;
    size_t currentBufferSize = 0;

    std::vector<GpuCollisionData> collisionData;
    std::vector<GpuCollisionData> updatedCollisionData;
    
    void updateCollisions(std::vector<Particle>& particles);

    const float RESPONSE_COEF = 1.0f;
    vec2 worldSize;
};