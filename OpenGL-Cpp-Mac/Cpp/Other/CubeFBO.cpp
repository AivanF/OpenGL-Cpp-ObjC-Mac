//
//  CubeFBO.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 24/02/2018.
//

#include "CubeFBO.hpp"
#include "helper.hpp"


CubeFBO::CubeFBO() {
    _fbo = 0;
    _shadowMapTex = 0;
    _depth = 0;
}

CubeFBO::~CubeFBO() {
    if (_fbo != 0) {
        glDeleteFramebuffers(1, &_fbo);
    }
    if (_shadowMapTex != 0) {
        glDeleteTextures(1, &_shadowMapTex);
    }
    if (_depth != 0) {
        glDeleteTextures(1, &_depth);
    }
}

void CubeFBO::init(unsigned int size) {
    _size = size;
    // Create FBO
    glGenFramebuffers(1, &_fbo);
    
    // Create depth buffer
    glGenTextures(1, &_depth);
    glBindTexture(GL_TEXTURE_2D, _depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Create cube map
    glGenTextures(1, &_shadowMapTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _shadowMapTex);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, size, size, 0, GL_RED, GL_FLOAT, NULL);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth, 0);
    
    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE);
    
    // Disable reads from the color buffer
    glReadBuffer(GL_NONE);
    
    checkFramebuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeFBO::bindForWriting(GLenum CubeFace) {
    checkFramebuffer();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _size, _size);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CubeFace, _shadowMapTex, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void CubeFBO::bindForReading(ShaderProgram *shp, int index, GLenum TextureUnit) {
    glActiveTexture(GL_TEXTURE0 + TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _shadowMapTex);
    glUniform1i(shp->uniform(("gCubeMaps[" + to_string(index) + "]").c_str()), TextureUnit);
    GetError();
}







