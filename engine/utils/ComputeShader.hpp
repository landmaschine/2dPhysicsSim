#pragma once
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class ComputeShader {
public:
    unsigned int ID;
    ComputeShader() {}

    void init(const char* computePath) {
        std::string computeCode;
        std::ifstream computeFile;
        
        computeFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            computeFile.open(computePath);
            std::stringstream computeStream;
            
            computeStream << computeFile.rdbuf();
            
            computeFile.close();
            
            computeCode = computeStream.str();
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* cShaderCode = computeCode.c_str();
        
        unsigned int compute;
        
        compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &cShaderCode, NULL);
        glCompileShader(compute);
        checkCompileErrors(compute, "COMPUTE");
        
        ID = glCreateProgram();
        glAttachShader(ID, compute);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(compute);
    }
    
    
    void use() { 
        glUseProgram(ID); 
    }
    
    void setBool(const std::string& name, bool& value) const {
        int location;
        auto it = uniformCache.find(name);
        if (it != uniformCache.end()) {
            location = it->second;
        } else {
            location = glGetUniformLocation(ID, name.c_str());
            if (location == -1) {
                std::cerr << "Warning: Uniform '" << name << "' doesn't exist or is not used in the shader." << std::endl;
            }
            uniformCache[name] = location;
        }
        glUniform1i(location, (int)value);
    }

    void setInt(const std::string& name, int value) const {
        int location;
        auto it = uniformCache.find(name);
        if (it != uniformCache.end()) {
            location = it->second;
        } else {
            location = glGetUniformLocation(ID, name.c_str());
            if (location == -1) {
                std::cerr << "Warning: Uniform '" << name << "' doesn't exist or is not used in the shader." << std::endl;
            }
            uniformCache[name] = location;
        }
        glUniform1i(location, value);
    }

    void setFloat(const std::string& name, float value) const {
        int location;
        auto it = uniformCache.find(name);
        if (it != uniformCache.end()) {
            location = it->second;
        } else {
            location = glGetUniformLocation(ID, name.c_str());
            if (location == -1) {
                std::cerr << "Warning: Uniform '" << name << "' doesn't exist or is not used in the shader." << std::endl;
            }
            uniformCache[name] = location;
        }
        glUniform1f(location, value);
    }

    void setVec2(const std::string& name, vec2& value) {
        int location;
        auto it = uniformCache.find(name);
        if (it != uniformCache.end()) {
            location = it->second;
        } else {
            location = glGetUniformLocation(ID, name.c_str());
            if (location == -1) {
                std::cerr << "Warning: Uniform '" << name << "' doesn't exist or is not used in the shader." << std::endl;
            }
            uniformCache[name] = location;
        }
        glUniform2f(location, value.x, value.y);
    }

    void setMat4(const std::string& name, glm::mat4& value) const {
        int location;
        auto it = uniformCache.find(name);
        if(it != uniformCache.end()) {
            location = it->second;
        } else {
            location = glGetUniformLocation(ID, name.c_str());
            if(location == -1) {
                std::cerr << "Warning: Uniform '" << name << "' doesnt exist or is not used in the shader." << std::endl;
            }
            uniformCache[name] = location;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    mutable std::unordered_map<std::string, int> uniformCache;
};