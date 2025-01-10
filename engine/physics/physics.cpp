#include "physics.hpp"

PhysicsEngine::PhysicsEngine() {
    collShader.init("C:/Users/leon/Documents/dev/2dPhysicsSim/bin/shaders/collision.glsl");
    collShader.use();
}

PhysicsEngine::~PhysicsEngine() {
    glDeleteBuffers(1, &inputBuffer);
    glDeleteBuffers(1, &outputBuffer);
}

void PhysicsEngine::init(size_t maxParticles) {
    int work_grp_cnt[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    if(work_grp_cnt[0] < 1) {
        throw std::runtime_error("Compute shader not supported!");
    }

    glGenBuffers(1, &inputBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, maxParticles * sizeof(GpuCollisionData), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer);  

    glGenBuffers(1, &outputBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, maxParticles * sizeof(GpuCollisionData), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBuffer);  

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); 
}

void PhysicsEngine::update(std::vector<Particle>& particles, float dt) {
    const vec2 gravity{0.f, 1500.f};

    for(auto& particle : particles) {
        if(particle.isPlayer) {
            particle.curr_pos = particle.targetPos;
        } else {
            vec2 vel = particle.curr_pos - particle.prev_pos;
            
            vec2 temp = particle.curr_pos;
            particle.accel += gravity;
            particle.curr_pos = particle.curr_pos + vel + particle.accel * (dt * dt);
            particle.prev_pos = temp;
            particle.accel = vec2(0.f, 0.f);
        }
    }
    updateCollisions(particles);
}

void PhysicsEngine::updateCollisions(std::vector<Particle>& particles) {
    std::vector<GpuCollisionData> collisionData;
    collisionData.reserve(particles.size());
    
    for(const auto& particle : particles) {
        collisionData.emplace_back(GpuCollisionData{
            particle.curr_pos.x,
            particle.curr_pos.y,
            particle.radius
        });
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, collisionData.size() * sizeof(GpuCollisionData), collisionData.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, collisionData.size() * sizeof(GpuCollisionData), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBuffer);

    collShader.use();
    collShader.setFloat("responseCoef", RESPONSE_COEF);
    collShader.setInt("particleCount", collisionData.size());
    collShader.setVec2("world_size", worldSize);

    const uint32_t WORK_GROUP_SIZE = 1024;
    uint32_t numWorkGroups = (collisionData.size() + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
    glDispatchCompute(numWorkGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    std::vector<GpuCollisionData> updatedCollisionData(collisionData.size());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, updatedCollisionData.size() * sizeof(GpuCollisionData), updatedCollisionData.data());

    for(size_t i = 0; i < particles.size(); ++i) {
        particles[i].curr_pos.x = updatedCollisionData[i].x;
        particles[i].curr_pos.y = updatedCollisionData[i].y;
    }
}