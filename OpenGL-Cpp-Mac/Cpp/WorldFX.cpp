//
//  WorldFX.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 24/01/2018.
//

#include "World.hpp"
#include "helper.hpp"

#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

void World::prepareFX() {
    /// Sunlight
    {
        _dirlight.fAmbient = 0.3;
        _dirlight.vColor = glm::vec3(1.0f, 0.98f, 0.9f) * (float)(1.0f + 0.5f * cos(degreesToRadians(par)));
//        _dirlight.vDirection = glm::vec3(0.4f, -0.8f, -0.447214f);
        glm::mat4 rot = glm::rotate(glm::mat4(), (float)degreesToRadians(45), glm::vec3(1.0f, 0.0f, 0.0f));
        rot = glm::rotate(rot, (float)degreesToRadians(par), glm::vec3(0.0f, 1.0f, 0.0f));
        rot = transpose(rot);
        glm::vec3 dir(0.0f, -1.0f, 0.0f);
        dir = glm::mat3(rot) * dir;
        _dirlight.vDirection = dir;
//        printMat(glm::value_ptr(rot));
//        printVec(glm::value_ptr(dir), 3);
        
        _dirlight.SetUniformData(_shpt, "sunLight[0]");
        GetError();
    }
    
    /// Set up point lights
    {
        // blue
        CPointLight *l = _points[0];
        l->vColor = glm::vec3(0.1f, 0.1f, 1.0f);
        l->vPosition = glm::vec3(2.0f, 1.0f - cos(degreesToRadians(4 * par)), 2.0f);
        l->fAmbient = 0.5f;
        l->fConstantAtt = 0.0f;
        l->fLinearAtt = 0.0f;
        l->fExpAtt = 0.25f;
        if (par == 0) {
            l->active = !l->active;
        }
        
        // yellow
        float len = 2.0f;
        l = _points[1];
        l->vColor = glm::vec3(1.0f, 0.5f, 0.1f);
        l->vPosition = glm::vec3(9.0f - len*cos(degreesToRadians(4 * par)),
                                 0.0f + 0.5 * len*sin(degreesToRadians(2 * par)),
                                 9.0f + 0.5 * len*sin(degreesToRadians(4 * par)));
        l->fAmbient = 1.0f;
        l->fConstantAtt = 0.0f;
        l->fLinearAtt = 0.0f;
        l->fExpAtt = 0.1f;
        
        float pr = 1.0f;
        parts[0].SetGeneratorProperties(
            l->vPosition, // Where the particles are generated
            glm::vec3(-pr, 0, -pr), // Minimal velocity
            glm::vec3(pr, 2, pr), // Maximal velocity
            glm::vec3(-1, -0.05, 0), // Gravity force applied to particles
            glm::vec3(1.0f, 0.7f, 0.2f), // Color
            0.5f, // Minimum lifetime in seconds
            2.0f, // Maximum lifetime in seconds
            0.25f, // Rendered size
            0.02f, // Spawn delay
            30); // And spawn 30 particles
    }
    
    /// Upload point lights
    {
        int cnt = 0;
        for (int i = 0; i < _points.size(); i++) {
            CPointLight *l = _points[i];
            if (l->active) {
                l->SetUniformData(_shpt, "pointLights[" + std::to_string(cnt) + "]");
                cnt++;
            }
        }
        glUniform1i(_shpt->uniform("countPoint"), cnt);
        GetError();
    }
    
    /// Set up spot lights
    {
        // green
        CSpotLight *l = _spots[0];
        l->vColor = glm::vec3(0.1f, 1.0f, 0.2f);
        l->vPosition = glm::vec3(-6.0f,
                                 0.5f + 0.5f * sin(degreesToRadians(4 * par)),
                                 8.0f + 0.5f * cos(degreesToRadians(4 * par)));
        l->vDirection = glm::normalize(glm::vec3(5.0f, 2.0f, 5.0f) - l->vPosition);
        l->fConeAngle = 90.0f;
        l->fLinearAtt = 2.0f;
        if (par == 0) {
            l->active = !l->active;
        }
        
        // purple
        l = _spots[1];
        l->vColor = glm::vec3(0.8f, 0.1f, 0.99f);
        l->vPosition = glm::vec3(0.0f, 2.0f, -6.0f);
        l->vDirection = glm::normalize(glm::vec3(0.0f, 2.0f, -5.0f)
                                       - l->vPosition
                                       + glm::vec3(0.25f * sin(degreesToRadians(par)),
                       0.5f * cos(degreesToRadians(par)),
                       0.0f));
        l->fConeAngle = 30.0f;
        l->fLinearAtt = 1.0f;
    }
    
    /// Upload spot lights
    {
        int cnt = 0;
        for (int i = 0; i < _spots.size(); i++) {
            CSpotLight *l = _spots[i];
            if (l->active) {
                l->SetUniformData(_shpt, "spotLights[" + std::to_string(cnt) + "]");
                cnt++;
            }
        }
        glUniform1i(_shpt->uniform("countSpot"), cnt);
        GetError();
    }
    
    // Set up fog
    {
        glUniform3fv(_shpt->uniform("vCameraPos"), 1, glm::value_ptr(glm::vec3(_px, _py, _pz)));
        _fog.SetUniformData(_shpt, "fogParams[0]");
        GetError();
    }
}


