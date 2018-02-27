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
