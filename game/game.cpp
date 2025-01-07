#include "game.hpp"

void Engine::init_game() {

}

void Engine::update() {
    
}

void Engine::render() {
    
}

void Engine::input() {
    m_input.update(m_event, m_quit);

    if (m_input.isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_quit = true;
    }
}
