//
//  WorldRenderShadows.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 24/02/2018.
//

#include "World.hpp"
#include "helper.hpp"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#define NUM_OF_LAYERS 6

struct CameraDirection {
    GLenum CubemapFace;
    glm::vec3 Target;
    glm::vec3 Up;
};

CameraDirection gCameraDirections[NUM_OF_LAYERS] = {
    { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }
};

void World::renderShadows() {
    _shps->use();
    
    glm::mat4 mProj = glm::perspective((float)M_PI_2, 1.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(_shps->uniform("matrices.projMatrix"), 1, GL_FALSE, glm::value_ptr(mProj));
    
    for (int pnt = 0; pnt < _points.size(); pnt++) {
        CPointLight *l = _points[pnt];
        glm::vec3 pos = glm::vec3(-l->vPosition.x, -l->vPosition.y, -l->vPosition.z);
        glUniform3fv(_shps->uniform("vCameraPos"), 1, glm::value_ptr(pos));
        
        glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
        GetError();
        
        for (int i = 0; i < NUM_OF_LAYERS; i++) {
            glm::mat4 mCamera;
            mCamera = glm::lookAt(glm::vec3(0,0,0), gCameraDirections[i].Target, gCameraDirections[i].Up);
            mCamera = glm::translate(mCamera, pos);
            glUniformMatrix4fv(_shps->uniform("matrices.viewMatrix"), 1, GL_FALSE, glm::value_ptr(mCamera));
            
            if (pnt == light_target) {
                _savedMView[i] = mCamera;
            }
            
            l->shadowMap.bindForWriting(gCameraDirections[i].CubemapFace);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS); 
            
            for (int i = 0; i < _items.size(); i++) {
                _items[i]->draw(_shps, false);
            }
            GetError();
        }
    }
    _savedMProj = mProj;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GetError();
}
