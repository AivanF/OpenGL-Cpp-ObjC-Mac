//
//  ShaderProgram.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 06/12/2017.
//

#include "ShaderProgram.hpp"

GLuint ShaderProgram::index() {
    return program;
}

GLuint ShaderProgram::attr(const GLchar *name) {
    return glGetAttribLocation(program, name);
}

GLuint ShaderProgram::uniform(const GLchar *name) {
    return glGetUniformLocation(program, name);
}
