//
//  helper.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 05/12/2017.
//

#include "helper.hpp"
#include <cstdio>
#import <Cocoa/Cocoa.h>
#include "../glm/gtc/type_ptr.hpp"
#include <OpenGL/gl3.h>


void printMat(glm::mat4 m) {
    printMat(glm::value_ptr(m));
}

void printMat(float* m) {
    printf("\nMatrix:\n%f %f %f %f \n%f %f %f %f \n%f %f %f %f \n%f %f %f %f \n",
           m[0], m[1], m[2], m[3],
           m[4], m[5], m[6], m[7],
           m[8], m[9], m[10], m[11],
           m[12], m[13], m[14], m[15]);
}

void printVec(glm::vec4 v) {
    printVec(glm::value_ptr(v), 4);
}

void printVec(glm::vec3 v) {
    printVec(glm::value_ptr(v), 3);
}

void printVec(float* v, char cnt) {
    printf("\nVector:");
    for (int i = 0; i < cnt; i++) {
        printf(" %f", v[i]);
    }
    printf("\n");
}

const char* getPath(const char *name, const char *ext) {
    NSBundle *myBundle = [NSBundle mainBundle];
    NSString *absPath = [myBundle pathForResource:[NSString stringWithUTF8String:name] ofType:[NSString stringWithUTF8String:ext]];
    if (!absPath) {
        printf("getPath: file was not found!\n");
        assert( 0 );
    }
    return [absPath cStringUsingEncoding:NSASCIIStringEncoding];
}

const char* extractDirectory(const char *path) {
    NSString *directory = [[NSString stringWithUTF8String:path] stringByDeletingLastPathComponent];
    return [[NSString stringWithFormat:@"%@/", directory] cStringUsingEncoding:NSASCIIStringEncoding];
}

void checkFramebuffer() {
    int st = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (st == GL_FRAMEBUFFER_COMPLETE) {
        return;
    }
    
    switch (st) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            NSLog(@"FB status: INCOMPLETE_ATTACHMENT!");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            NSLog(@"FB status: INCOMPLETE_MISSING_ATTACHMENT!");
            break;
//        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
//            NSLog(@"FB status: INCOMPLETE_DIMENSIONS!");
//            break;
//        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
//            NSLog(@"FB status: INCOMPLETE_FORMATS!");
//            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            NSLog(@"FB status: INCOMPLETE_DRAW_BUFFER!");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            NSLog(@"FB status: UNSUPPORTED!");
            break;
        case GL_FRAMEBUFFER_BINDING:
            NSLog(@"FB status: BINDING!");
            break;
        default:
            NSLog(@"FB status: unknown! 0x%.8X", st);
            break;
    }
    assert(0);
}
