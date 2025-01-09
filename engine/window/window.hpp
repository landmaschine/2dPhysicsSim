#pragma once
#include "SDL/SDL3/SDL.h"
#include <string>
#include <cassert>
#include "utils/logger.hpp"
#include "math/simMath.hpp"

struct ScreenData {
    int width = 1920;
    int height = 1080;
};

struct WindowMetaData {
    std::string name = "physim2D";
};

class Window {
    public:
        Window();
        ~Window();

        void pullSize();

        WindowMetaData getWindowMetaData() {
            return m_windowMetaData;
        }

        ScreenData& getWindowSize() {
            return m_screenData;
        }

        SDL_Window* getWindow() {
            return m_window;
        }

    private:
        ScreenData m_screenData;
        WindowMetaData m_windowMetaData;

        SDL_Window* m_window;
};