#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

#include "../game_object/game_object.h"
#include "../render/color.h"

class ShaderManager {
   public:
    ShaderManager();
    GLuint compileAndLinkShaders();
    void loadShaderSourcesFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void transform(glm::mat4 modelMatrix, Color* color);

   private:
    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;
    GLuint m_shaderProgram;
};