//
//  CubeSimplifier.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 24/02/2018.
//

#ifndef CubeSimplifier_hpp
#define CubeSimplifier_hpp

#include "ShaderProgram.hpp"


class CubeSimplifier {
private:
    /// Vertex coordinates
    float ver[3*6*4];
    /// Texture coordinates
    float tex[2*6*4];
    /// Elements indices
    unsigned char els[3*2*6];
    
    GLuint uiVBO[4];
    GLuint uiVAO[1];
    
public:
    CubeSimplifier();
    ~CubeSimplifier();
    void draw(GLuint cubemap, ShaderProgram *shpm, float w, float h, float mx, float my, float sx, float sy);
};

#endif /* CubeSimplifier_hpp */
