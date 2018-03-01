//
//  CubeFBO.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 24/02/2018.
//

#ifndef CubeFBO_hpp
#define CubeFBO_hpp

#include "ShaderProgram.hpp"
#include <string>

using namespace std;

#define FBO_CUBE_SIZE 1024


class CubeFBO {
public:
    GLuint _fbo, _shadowMapTex, _depth;
    unsigned int _size;
    
    CubeFBO();
    ~CubeFBO();
    void init(unsigned int size);
    void bindForWriting(GLenum CubeFace);
    void bindForReading(ShaderProgram *shp, int index, GLenum TextureUnit);
};

#endif /* CubeFBO_hpp */
