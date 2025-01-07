#include "core.hpp"

Engine::~Engine() {
    SDL_Quit();
}

void Engine::run() {
    init_engine();
    init_game();

    m_engineData.previousTicks = SDL_GetPerformanceCounter();
    double performanceFrequency = static_cast<double>(SDL_GetPerformanceFrequency());

    while(!m_quit) {
        m_engineData.currentTicks = SDL_GetPerformanceCounter();
        m_engineData.elapsedTicks = m_engineData.currentTicks - m_engineData.previousTicks;
        double frameTime = m_engineData.elapsedTicks / performanceFrequency;
        m_engineData.previousTicks = m_engineData.currentTicks;

        if(frameTime > m_engineData.maxFrameTime) {
           frameTime = m_engineData.maxFrameTime;
        }

        m_engineData.accumulator += frameTime;

        input();

        int steps = 0;
        while (m_engineData.accumulator >= m_engineData.timeStep && steps < m_engineData.maxSteps) {
            update();
            m_engineData.accumulator -= m_engineData.timeStep;
            steps++;
        }

        if(steps == m_engineData.maxSteps && m_engineData.accumulator >= m_engineData.timeStep) {
            m_engineData.accumulator = 0.0f;
        }

        render();

        double currentFrameTime = (SDL_GetPerformanceCounter() - m_engineData.currentTicks) / performanceFrequency;
        while(currentFrameTime < m_engineData.targetFrameTime) {
            currentFrameTime = (SDL_GetPerformanceCounter() - m_engineData.currentTicks) / performanceFrequency;
        }
    }
}