#include "game.hpp"

void Engine::init_game() {
    Particle particle;
    particle.curr_pos = vec2(500.f, 500.f);
    particle.prev_pos = particle.curr_pos;
    particle.accel = vec2(0.0f);
    particle.radius = 10.f;

    particles.push_back(particle);

    InitializeParticles(100000, 1.f, m_window, particles);
}

void Engine::update() {
    const float sub_dt = m_engineData.timeStep / float(m_engineData.maxSteps);
    for(int i = 0; i < m_engineData.maxSteps; ++i) {
        m_physics->update(particles, sub_dt, i == 0);
    }
}

void Engine::render() {
    m_renderer->renderClear();
    m_renderer->renderCircles(particles);
    m_renderer->renderScreen();
}

void Engine::input() {
    m_input.update(m_event, m_quit);
    if (m_input.isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_quit = true;
    }
}
