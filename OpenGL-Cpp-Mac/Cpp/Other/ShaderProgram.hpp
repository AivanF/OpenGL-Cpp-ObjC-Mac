//
//  ShaderProgram.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 06/12/2017.
//

#ifndef ShaderProgram_hpp
#define ShaderProgram_hpp

#include <OpenGL/gl3.h>

class ShaderProgram {
protected:
    GLuint program;
    
public:
    ShaderProgram();
    ~ShaderProgram();
    
    void addShader(GLuint sh);
    void linkProgram();
    void use();
    
    GLuint index();
    GLuint attr(const GLchar *name);
    GLuint uniform(const GLchar *name);
};

#endif /* ShaderProgram_hpp */
