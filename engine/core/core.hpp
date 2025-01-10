#pragma once
#include "common.hpp"
#include "input/input.hpp"

typedef struct EngineData_t {
    uint64_t previousTicks = 0;
    uint64_t currentTicks = 0;
    uint64_t elapsedTicks= 0;
    double accumulator = 0.f;

    const float timeStep = 1.f / 200.f;
    const double maxFrameTime = 0.25f;
    const int maxSteps = 7;

    const double targetFrameTime = 1 / 60.f;

    const size_t MaxParticles = 100000;
    const size_t spawnParticles = 1 - 1;
    const float particleRadius = 3.f;
} EngineData;

typedef struct EnginePerformanceData_t {
    double frameTime = 0;
    double inputTime = 0;
    double updateTime = 0;
    double renderTime = 0;
} EnginePerformanceData;

class Engine {
    public:
        void init_game();

        void init_engine() {
            memset(&m_event, 0, sizeof(SDL_Event));
            
            m_window = std::make_shared<Window>();
            m_renderer = std::make_unique<RendererGL>(m_window);
            m_physics = std::make_unique<PhysicsEngine>();
            m_physics->init(m_engineData.MaxParticles);
        }

        ~Engine();

        void run();
    private:
        void update();
        void input();
        void render();

        bool m_quit = false;
        SDL_Event m_event;
        Input m_input;
        EngineData m_engineData;
        EnginePerformanceData m_enginePerformance;

        std::shared_ptr<Window> m_window;
        std::unique_ptr<RendererGL> m_renderer;
        std::unique_ptr<PhysicsEngine> m_physics;
        Particle playerParticle;
        std::vector<Particle> particles;
};