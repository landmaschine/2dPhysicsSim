#pragma once
#include "core/common.hpp"
#include "window/window.hpp"
#include "glad/glad.h"

class RendererGL {
public:
    RendererGL(std::shared_ptr<Window>& window);
    RendererGL();
    ~RendererGL() {
        glDeleteTextures(1, &texture);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VAO);

        if(m_glContext) {
            SDL_GL_DestroyContext(m_glContext);
        }
    }

    void renderCircle(Particle& particle);
    void renderCircles(std::vector<Particle>& particles);
    void renderClear();
    void renderScreen();

    void renderText(std::string text, SDL_Color color, float x, float y);
    void renderPerformanceText(const EnginePerformanceData& perf, const std::vector<Particle>& particles);
private:
    TTF_Font* font;
    unsigned int texture;

    SDL_GLContext m_glContext;
    Shader m_shader;
    Shader textShader;
    std::shared_ptr<Window> m_window;

    void generateCircleData(float radius, unsigned int segments);
    void updateProjection();
    void initCircleBuffer(int segments);
    void createQuad();
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;

    unsigned int textVAO;
    unsigned int textVBO;

    unsigned int instanceVBO;
    std::vector<glm::vec4> instanceData;

    glm::mat4 projection;
    glm::mat4 model;

    unsigned int circleSegments;
};