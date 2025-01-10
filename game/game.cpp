#include "game.hpp"

void Engine::init_game() {
    playerParticle.curr_pos = vec2(500.f, 500.f);
    playerParticle.prev_pos = playerParticle.curr_pos;
    playerParticle.accel = vec2(0.0f);
    playerParticle.radius = 10.f;

    particles.push_back(playerParticle);

    InitializeParticles(m_engineData.spawnParticles, m_engineData.particleRadius, m_window, particles);
}

void Engine::update() {
    auto start = std::chrono::high_resolution_clock::now();
    
    const float sub_dt = m_engineData.timeStep / float(m_engineData.maxSteps);
    for(int i = 0; i < m_engineData.maxSteps; ++i) {
        m_physics->setWorldSize(vec2(m_window->getWindowSize().width, m_window->getWindowSize().height));
        m_physics->update(particles, sub_dt);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    m_enginePerformance.updateTime = duration.count() / 1000.0f;
}

void Engine::render() {
    auto start = std::chrono::high_resolution_clock::now();
    
    m_renderer->renderClear();
    m_renderer->renderCircles(particles);
    m_renderer->renderScreen();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    m_enginePerformance.renderTime = duration.count() / 1000.0f;

    m_enginePerformance.frameTime = m_enginePerformance.renderTime + m_enginePerformance.updateTime;
    std::cout << "Physics time: " << m_enginePerformance.updateTime << " ms | Render Time: " << m_enginePerformance.renderTime << 
    " ms | FPS: " << 1000.f / (m_enginePerformance.frameTime) << " | Particles: " << particles.size() <<"\n";

}

void Engine::input() {
    m_input.update(m_event, m_quit);
    if (m_input.isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_quit = true;
    }

vec2 mousePos = vec2(0.0f);
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    if(m_input.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
        playerParticle.radius = 10.f;
        playerParticle.targetPos = mousePos;
        playerParticle.isPlayer = true;
    } else {
        playerParticle.radius = 0.0f;
        playerParticle.targetPos = mousePos;
    }
    particles[0] = playerParticle;

    if (m_input.isMouseButtonPressed(SDL_BUTTON_RIGHT)) {
        constexpr float PI = 3.14159f;
        const float spawnRadius = 10.f;
        const int numParticlesToSpawn = 30;

        for (int i = 0; i < numParticlesToSpawn; i++) {
            float randomAngle = static_cast<float>(rand()) / RAND_MAX * 2.f * PI;
            float randomDistance = static_cast<float>(rand()) / RAND_MAX * spawnRadius;
            
            vec2 offset = {
                randomDistance * cos(randomAngle),
                randomDistance * sin(randomAngle)
            };
            
            vec2 spawnPos = mousePos + offset;

            Particle newParticle;
            newParticle.radius = 3.f;
            newParticle.curr_pos = spawnPos;
            newParticle.prev_pos = spawnPos;
            newParticle.isPlayer = false;
            
            particles.push_back(newParticle);
        }
    }
}
