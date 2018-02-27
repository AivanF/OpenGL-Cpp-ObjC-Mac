//
//  shaders.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 05/12/2017.
//

#include "shaders.hpp"
#include "helper.hpp"
#import <Cocoa/Cocoa.h>


GLuint makeShader(GLenum type, const char *source) {
    GLuint shader;
    
    shader = glCreateShader(type);
    GetError();
    glShaderSource(shader, 1, &source, NULL);
    GetError();
    glCompileShader(shader);
    GetError();
    
#if defined(DEBUG)
    GLint logLength;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    GetError();
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc((size_t)logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        GetError();
        NSLog(@"Shader compilation failed with error:\n%s", log);
        free(log);
    }
#endif
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    GetError();
    if (0 == status) {
        glDeleteShader(shader);
        GetError();
        NSLog(@"Shader compilation failed from code!");
        assert(0);
    }
    
    return shader;
}

GLuint makeShaderFromFile(GLenum type, const char *name, const char *ext) {
    NSBundle *myBundle = [NSBundle mainBundle];
    NSString *absPath = [myBundle pathForResource:[NSString stringWithUTF8String:name] ofType:[NSString stringWithUTF8String:ext]];
    NSString *cont = [NSString stringWithContentsOfFile:absPath encoding:NSASCIIStringEncoding error:nil];
    const GLchar *source = (GLchar *)[cont cStringUsingEncoding:NSASCIIStringEncoding];
    
    if (nil == source) {
        NSLog(@"Shader file reading failed!");
        assert(0);
    }
    return makeShader(type, source);
}

