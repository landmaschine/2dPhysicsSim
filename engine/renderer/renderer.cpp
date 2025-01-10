#include "renderer.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

RendererGL::RendererGL() {

}

RendererGL::RendererGL(std::shared_ptr<Window>& window) : m_window(window) {
    m_glContext = SDL_GL_CreateContext(window->getWindow());
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        ERRLOG(SDL_GetError());
        assert(false && "Failed to init GLAD!");
    }

    glViewport(0, 0, m_window->getWindowSize().width, m_window->getWindowSize().height);

    SDL_GL_MakeCurrent(window->getWindow(), m_glContext);

    m_shader.init("C:/Users/leon/Documents/dev/2dPhysicsSim/bin/shaders/vertex.glsl", "C:/Users/leon/Documents/dev/2dPhysicsSim/bin/shaders/fragment.glsl");

    font = TTF_OpenFont("C:/Users/leon/Documents/dev/2dPhysicsSim/bin/fonts/DepartureMono-Regular.otf", 22);
    if(!font) {
        ERRLOG("Failed to load font");
        assert(false);
    }

    texture = 0;

    textShader.init("C:/Users/leon/Documents/dev/2dPhysicsSim/bin/shaders/text_vertex.glsl", "C:/Users/leon/Documents/dev/2dPhysicsSim/bin/shaders/text_fragment.glsl");
    createQuad();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &instanceVBO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    initCircleBuffer(16);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    projection = glm::ortho(
        0.0f,
        (float)m_window->getWindowSize().width,
        (float)m_window->getWindowSize().height,
        0.0f,
        -10.f,
        10.f
    );
}

void RendererGL::generateCircleData(float radius, unsigned int segments) {
    vertices.clear();
    indices.clear();

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    circleSegments = segments;

    float angleStep = 2.0f * M_PI / segments;
    for(int i = 0; i <= segments; ++i) {
        float angle = i * angleStep;
        vertices.push_back(radius * glm::cos(angle));
        vertices.push_back(radius * glm::sin(angle));
        vertices.push_back(0.0f);
    }

    for(unsigned int i = 1; i <= segments; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i % segments + 1);
    }
}

void RendererGL::renderCircle(Particle& particle) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(particle.curr_pos.x, particle.curr_pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(particle.radius, particle.radius, 0.0f));

    m_shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * circleSegments, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void RendererGL::renderCircles(std::vector<Particle>& particles) {
    instanceData.clear();
    instanceData.reserve(particles.size());

    for(const auto& particle : particles) {
        instanceData.push_back(glm::vec4(
            particle.curr_pos.x,
            particle.curr_pos.y,
            particle.radius,
            0.f
        ));
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(glm::vec4), instanceData.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, 3 * circleSegments,
                            GL_UNSIGNED_INT, 0, particles.size());
    glBindVertexArray(0);
}

void RendererGL::renderClear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader.use();
    updateProjection();

    m_shader.setMat4("projection", projection);
}

void RendererGL::renderScreen() {
    glViewport(0, 0, m_window->getWindowSize().width, m_window->getWindowSize().height);
    SDL_GL_SwapWindow(m_window->getWindow());
}

void RendererGL::updateProjection() {
    m_window->pullSize();
    projection = glm::ortho(
        0.0f,
        (float)m_window->getWindowSize().width,
        (float)m_window->getWindowSize().height,
        0.0f,
        -10.f,
        10.f
    );

}

void RendererGL::initCircleBuffer(int segments) {
    generateCircleData(1.0f, segments);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void RendererGL::createQuad() {
    float quadVertices[] = {
        // pos (x,y,z)    // tex coords (s,t)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f   // top right
    };

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void RendererGL::renderText(std::string text, SDL_Color color, float x, float y) {
   if(!font) {
        ERRLOG("Font not loaded!");
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
    if (!surface) {
        ERRLOG("Failed to render text surface!");
        return;
    }

    SDL_Surface* rgbaSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);
    
    if (!rgbaSurface) {
        ERRLOG("Failed to convert text surface to RGBA!");
        return;
    }

    GLuint textTexture;
    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaSurface->w, rgbaSurface->h, 
                 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaSurface->pixels);

    textShader.use();
    textShader.setMat4("projection", projection);
    textShader.setInt("textTexture", 0);
    textShader.setVec4("textColor", glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(rgbaSurface->w, rgbaSurface->h, 1.0f));
    textShader.setMat4("model", model);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glBindVertexArray(textVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    
    SDL_DestroySurface(rgbaSurface);
    glDeleteTextures(1, &textTexture); 
}

void RendererGL::renderPerformanceText(const EnginePerformanceData& perf, const std::vector<Particle>& particles) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2)
       << "Physics time: " << perf.updateTime << " ms | "
       << "Render Time: " << perf.renderTime << " ms | "
       << "FPS: " << 1000.0f / perf.frameTime << " | "
       << "Particles: " << particles.size();
    
    SDL_Color white = {255, 255, 255, 255};
    renderText(ss.str(), white, 10.0f, 10.0f);
}