#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>




constexpr const int chunkSize = 16;



extern unsigned int shaderProgram;


void setMat4(const std::string& name, const glm::mat4& mat);
