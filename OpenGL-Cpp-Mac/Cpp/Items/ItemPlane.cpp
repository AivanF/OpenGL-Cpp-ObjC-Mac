//
//  ItemPlane.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 06/12/2017.
//

#include "ItemPlane.hpp"
#include "ShaderProgram.hpp"
#include "helper.hpp"
#include "../../glm/gtc/type_ptr.hpp"

ItemPlane::ItemPlane(int at1, int at3) : Item() {
    t1 = at1; t3 = at3;
    _sc = 20; _py = -0.51f;
    
    int i = 0;
    ver[0] = 2.0f; ver[1] = 0.0f; ver[2] = -2.0f;
    ver[3] = -2.0f; ver[4] = 0.0f; ver[5] = -2.0f;
    ver[6] = 2.0f; ver[7] = 0.0f; ver[8] = 2.0f;
    ver[9] = -2.0f; ver[10] = 0.0f; ver[11] = 2.0f;
    
    // convex illusion of 45 degrees
    nor[0] = 0.5f; nor[1] = 0.707107f; nor[2] = -0.5f;
    nor[3] = -0.5f; nor[4] = 0.707107f; nor[5] = -0.5f;
    nor[6] = 0.5f; nor[7] = 0.707107f; nor[8] = 0.5f;
    nor[9] = -0.5f; nor[10] = 0.707107f; nor[11] = 0.5f;
    // simple plane
//    nor[0] = 0.0f; nor[1] = 0.1f; nor[2] = 0.0f;
//    nor[3] = -0.0f; nor[4] = 0.1f; nor[5] = 0.0f;
//    nor[6] = 0.0f; nor[7] = 0.1f; nor[8] = 0.0f;
//    nor[9] = 0.0f; nor[10] = 0.1f; nor[11] = 0.0f;
    
    i = 0;
    float uvm = 30.0f;
    tex[i++] = uvm; tex[i++] = 0.0f;
    tex[i++] = 0.0f; tex[i++] = 0.0f;
    tex[i++] = uvm; tex[i++] = uvm;
    tex[i++] = 0.0f; tex[i++] = uvm;
    
    glGenVertexArrays(1, &uiVAO[0]);
    glGenBuffers(3, &uiVBO[0]);
    GetError();
    
    glBindVertexArray(uiVAO[0]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    GetError();
    
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), ver, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GetError();
    
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), nor, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GetError();
    
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), tex, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    GetError();
}

ItemPlane::~ItemPlane() {
    glDeleteVertexArrays(1, uiVAO);
    glDeleteBuffers(3, uiVBO);
    GetError();
}

void ItemPlane::draw(ShaderProgram *shp) {
    ShaderProgram *s = (ShaderProgram*)shp;
    
    glm::mat4 mModel = apply();
    glm::mat4 mNormal = transpose(inverse(mModel));
    glUniformMatrix4fv(s->uniform("matrices.modelMatrix"), 1, GL_FALSE, glm::value_ptr(mModel));
    glUniformMatrix4fv(s->uniform("matrices.normalMatrix"), 1, GL_FALSE, glm::value_ptr(mNormal));
    GetError();
    glUniform1i(s->uniform("gSamplers[0]"), 0);
    GetError();
    glUniform1i(s->uniform("gSamplers[1]"), 1);
    GetError();
    glUniform1i(s->uniform("gSamplers[2]"), 2);
    GetError();
    glUniform1f(s->uniform("fTextureContributions[0]"), cf);
    glUniform1f(s->uniform("fTextureContributions[1]"), 1.0f - cf);
    glUniform1i(s->uniform("numTextures"), 2);
    GetError();
    
    glBindVertexArray(uiVAO[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    GetError();
}
