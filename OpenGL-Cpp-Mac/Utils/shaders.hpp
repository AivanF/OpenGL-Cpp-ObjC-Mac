//
//  shaders.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 05/12/2017.
//

#ifndef shaders_hpp
#define shaders_hpp

#include <OpenGL/gl3.h>

GLuint makeShader(GLenum type, const char *source);
GLuint makeShaderFromFile(GLenum type, const char *name, const char *ext);

#endif /* shaders_hpp */
