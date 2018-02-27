//
//  WorldDraw.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 24/01/2018.
//

#include "World.hpp"
#include "helper.hpp"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"


void World::draw(float w, float h) {
    if (moving)
        par = (par + 1) % 360;
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    draw3D(w, h);
    draw2D(w, h);
    
    glFlush();
    GetError();
}

void World::draw3D(float w, float h) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    /// Set up buffer and matrices
    glm::mat4 mView, mProj;
    mProj = glm::perspective(70.0f, w / h, 0.1f, 100.0f);
    mView = glm::rotate(mView, (float)degreesToRadians(_rx), glm::vec3(1.0f, 0.0f, 0.0f));
    mView = glm::rotate(mView, (float)degreesToRadians(_ry), glm::vec3(0.0f, 1.0f, 0.0f));
    //    printMat(glm::value_ptr(mModelView));
    glm::vec3 position = glm::vec3(_px, _py, _pz);
    glm::vec3 direction;
    {
        glm::mat4 m;
        m = glm::rotate(m, -(float)degreesToRadians(_ry), glm::vec3(0.0f, 1.0f, 0.0f));
        m = glm::rotate(m, -(float)degreesToRadians(_rx), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec4 t = m * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        direction = glm::vec3(t.x, t.y, t.z);
    }
    
    _shpt->use();
    glUniform1f(_shpt->uniform("screenWidth"), w);
    glUniform1f(_shpt->uniform("screenHeight"), h);
    glUniform1f(_shpt->uniform("time"), par / 360.0f);
    glUniformMatrix4fv(_shpt->uniform("matrices.viewMatrix"), 1, GL_FALSE, glm::value_ptr(mView));
    glUniformMatrix4fv(_shpt->uniform("matrices.projMatrix"), 1, GL_FALSE, glm::value_ptr(mProj));
    glUniform3fv(_shpt->uniform("vCameraPos"), 1, glm::value_ptr(glm::vec3(_px, _py, _pz)));
    GetError();
    
    /// Draw the skybox relative to camera
    {
        _sky->cf = 0.5f * (1.0f + cos(degreesToRadians(par)));
        glDepthMask(0);
        textures[_sky->t1].BindTexture(0);
        textures[_sky->t2].BindTexture(1);
        textures[_sky->t3].BindTexture(2);
        
        /// The Sun
        glUniform1i(_shpt->uniform("useNormalMap"), 0);
        glUniform1f(_shpt->uniform("useLights"), 0.5f);
        // changing color cycle: white -> warm -> white -> cold
        if (par > 180) {
            // sunrise, cold color
            float d = 1 - abs((par - 270)) / 90.0f;
            _sun.vColor = glm::vec3(0.2f, 0.3f, 1.0f) * d + (1-d) * glm::vec3(1.0f, 1.0f, 1.0f);
        } else {
            // sunset, warm color
            float d = 1 - abs((par - 90)) / 90.0f;
            _sun.vColor = glm::vec3(1.0f, 0.1f, 0.0f) * d + (1-d) * glm::vec3(1.0f, 1.0f, 1.0f);
        }
        _sun.vPosition = glm::vec3(5.0f * sin(degreesToRadians(par)), 5.0f * cos(degreesToRadians(par)), 1.0f);
        
        _sun.fAmbient = 1.0f;
        _sun.fConstantAtt = 0.0f;
        _sun.fLinearAtt = 0.0f;
        _sun.fExpAtt = 0.1f;
        _sun.SetUniformData(_shpt, "pointLights[0]");
        glUniform1i(_shpt->uniform("countPoint"), 1);
        glUniform1i(_shpt->uniform("countSpot"), 0);
        
        _sky->draw(_shpt);
        glDepthMask(1);
        GetError();
    }
    
    /// Prepare view
    glUniform1i(_shpt->uniform("useNormalMap"), normalmap);
    glm::mat4 mCamera = glm::translate(mView, position);
    
    // Prepare lights and fog
    prepareFX();
    
    /// Draw all the items
    glUniformMatrix4fv(_shpt->uniform("matrices.viewMatrix"), 1, GL_FALSE, glm::value_ptr(mCamera));
    glUniform1f(_shpt->uniform("useLights"), 1.0f);
    for (int i = 0; i < _items.size(); i++) {
        
        if (_items[i]->t1 >= 0) {
            textures[_items[i]->t1].BindTexture(0);
        }
        if (_items[i]->t2 >= 0) {
            textures[_items[i]->t2].BindTexture(1);
        }
        if (_items[i]->t3 >= 0) {
            textures[_items[i]->t3].BindTexture(2);
        }
        
        if (moving)
            _items[i]->processStep();
        _items[i]->draw(_shpt);
    }
    
    /// Draw particles systems
    bool nf = _fog.iEquation == 3;
    textures[7].BindTexture(0);
    FOR(i, NUMPARTS) {
        parts[i].SetMatrices(&mProj, &mCamera, direction);
        
        if (moving) {
            parts[i].UpdateParticles(0.1f);
        }
        
        if (nf) {
            parts[i].RenderParticles(position, 1.0f, 1.0f);
        } else {
            parts[i].RenderParticles(position, 0.9f, 0.999f);
        }
    }
}

void World::draw2D(float w, float h) {
    glDisable(GL_DEPTH_TEST);
    _shpf->use();
    glUniform1f(_shpf->uniform("screenWidth"), w);
    glUniform1f(_shpf->uniform("screenHeight"), h);
    GetError();
    
    /// Draw text
    glUniform4fv(_shpf->uniform("vColor"), 1, glm::value_ptr(glm::vec4(1, 1, 1, 1)));
    fnt1.Print("Maid by AivanF.", 32, 64, 96);
    
    glUniform4fv(_shpf->uniform("vColor"), 1, glm::value_ptr(glm::vec4(1, 0, 0, 1)));
    fnt2.Print("WASD to move, Escape - screenshot\nTab - fog, Space - stop time", 32, 192, 32);
    GetError();
}


