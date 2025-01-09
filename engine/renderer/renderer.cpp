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