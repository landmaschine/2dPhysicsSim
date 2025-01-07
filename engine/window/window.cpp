#include "window.hpp"

Window::Window() {
    if(!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        ERRLOG(SDL_GetError());
        assert(false && "Failed to init Renderer");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_window = SDL_CreateWindow(m_windowMetaData.name.c_str(), m_screenData.width, m_screenData.height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if(!m_window) {
        ERRLOG(SDL_GetError());
        assert(false && "Failed to create SDL window");
    }
}

Window::~Window() {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
}

//writes screen Width and height into Window struct -> screenData
void Window::pullSize() {
    SDL_GetWindowSize(m_window, &m_screenData.width, &m_screenData.height);
}