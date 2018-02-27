//
//  ShaderProgram.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 06/12/2017.
//

#include "ShaderProgram.hpp"
#include "helper.hpp"

ShaderProgram::ShaderProgram() {
    program = glCreateProgram();
    GetError();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program);
}

void ShaderProgram::addShader(GLuint sh) {
    glAttachShader(program, sh);
    GetError();
}

void ShaderProgram::linkProgram() {
    glLinkProgram(program);
    GetError();
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    GetError();
    if (0 == status) {
        printf("Failed to link Texture shader program!\n");
        assert( 0 );
    }
}

void ShaderProgram::use() {
    glUseProgram(index());
}

GLuint ShaderProgram::index() {
    return program;
}

GLuint ShaderProgram::attr(const GLchar *name) {
    return glGetAttribLocation(program, name);
}

GLuint ShaderProgram::uniform(const GLchar *name) {
    return glGetUniformLocation(program, name);
}
