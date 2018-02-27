//
//  ItemSkyBox.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 06/12/2017.
//

#include "ItemSkyBox.hpp"
#include "ShaderProgram.hpp"
#include "helper.hpp"
#include "../../glm/gtc/type_ptr.hpp"

#define R1O3 1.01f/3.0f
#define R2O3 1.99f/3.0f
#define S1O3 0.577350269189626f

ItemSkyBox::ItemSkyBox(int at1, int at2, int at3) : Item() {
    _py = 1;
    _ry = 0.0f; _sc = 1.0f;
    t1 = at1;
    t2 = at2;
    t3 = at3;
    
    int i = 0;
    ver[i++] = -1.0f; ver[i++] = 1.0f; ver[i++] = -1.0f;//0
    ver[i++] = -1.0f; ver[i++] = 1.0f; ver[i++] = 1.0f;//1
    ver[i++] = -1.0f; ver[i++] = -1.0f; ver[i++] = 1.0f;//2
    ver[i++] = -1.0f; ver[i++] = -1.0f; ver[i++] = -1.0f;//3
    
    ver[i++] = -1.0f; ver[i++] = 1.0f; ver[i++] = 1.0f;//4
    ver[i++] = 1.0f; ver[i++] = 1.0f; ver[i++] = 1.0f;//5
    ver[i++] = 1.0f; ver[i++] = -1.0f; ver[i++] = 1.0f;//6
    ver[i++] = -1.0f; ver[i++] = -1.0f; ver[i++] = 1.0f;//7
    
    ver[i++] = 1.0f; ver[i++] = 1.0f; ver[i++] = 1.0f;//8
    ver[i++] = 1.0f; ver[i++] = 1.0f; ver[i++] = -1.0f;//9
    ver[i++] = 1.0f; ver[i++] = -1.0f; ver[i++] = -1.0f;//10
    ver[i++] = 1.0f; ver[i++] = -1.0f; ver[i++] = 1.0f;//11
    
    ver[i++] = 1.0f; ver[i++] = 1.0f; ver[i++] = -1.0f;//12
    ver[i++] = -1.0f; ver[i++] = 1.0f; ver[i++] = -1.0f;//13
    ver[i++] = -1.0f; ver[i++] = -1.0f; ver[i++] = -1.0f;//14
    ver[i++] = 1.0f; ver[i++] = -1.0f; ver[i++] = -1.0f;//15
    
    ver[i++] = -1.0f; ver[i++] = -1.0f; ver[i++] = 1.0f;//16
    ver[i++] = 1.0f; ver[i++] = -1.0f; ver[i++] = 1.0f;//17
    ver[i++] = 1.0f; ver[i++] = -1.0f; ver[i++] = -1.0f;//18
    ver[i++] = -1.0f; ver[i++] = -1.0f; ver[i++] = -1.0f;//19
    
    ver[i++] = -1.0f; ver[i++] = 1.0f; ver[i++] = -1.0f;//20
    ver[i++] = 1.0f; ver[i++] = 1.0f; ver[i++] = -1.0f;//21
    ver[i++] = 1.0f; ver[i++] = 1.0f; ver[i++] = 1.0f;//22
    ver[i++] = -1.0f; ver[i++] = 1.0f; ver[i++] = 1.0f;//23
    
    i = 0;
    nor[i++] = -S1O3; nor[i++] = S1O3; nor[i++] = -S1O3;//0
    nor[i++] = -S1O3; nor[i++] = S1O3; nor[i++] = S1O3;//1
    nor[i++] = -S1O3; nor[i++] = -S1O3; nor[i++] = S1O3;//2
    nor[i++] = -S1O3; nor[i++] = -S1O3; nor[i++] = -S1O3;//3
    
    nor[i++] = -S1O3; nor[i++] = S1O3; nor[i++] = S1O3;//4
    nor[i++] = S1O3; nor[i++] = S1O3; nor[i++] = S1O3;//5
    nor[i++] = S1O3; nor[i++] = -S1O3; nor[i++] = S1O3;//6
    nor[i++] = -S1O3; nor[i++] = -S1O3; nor[i++] = S1O3;//7
    
    nor[i++] = S1O3; nor[i++] = S1O3; nor[i++] = S1O3;//8
    nor[i++] = S1O3; nor[i++] = S1O3; nor[i++] = -S1O3;//9
    nor[i++] = S1O3; nor[i++] = -S1O3; nor[i++] = -S1O3;//10
    nor[i++] = S1O3; nor[i++] = -S1O3; nor[i++] = S1O3;//11
    
    nor[i++] = S1O3; nor[i++] = S1O3; nor[i++] = -S1O3;//12
    nor[i++] = -S1O3; nor[i++] = S1O3; nor[i++] = -S1O3;//13
    nor[i++] = -S1O3; nor[i++] = -S1O3; nor[i++] = -S1O3;//14
    nor[i++] = S1O3; nor[i++] = -S1O3; nor[i++] = -S1O3;//15
    
    nor[i++] = -S1O3; nor[i++] = -S1O3; nor[i++] = S1O3;//16
    nor[i++] = S1O3; nor[i++] = -S1O3; nor[i++] = S1O3;//17
    nor[i++] = S1O3; nor[i++] = -S1O3; nor[i++] = -S1O3;//18
    nor[i++] = -S1O3; nor[i++] = -S1O3; nor[i++] = -S1O3;//19
    
    nor[i++] = -S1O3; nor[i++] = S1O3; nor[i++] = -S1O3;//20
    nor[i++] = S1O3; nor[i++] = S1O3; nor[i++] = -S1O3;//21
    nor[i++] = S1O3; nor[i++] = S1O3; nor[i++] = S1O3;//22
    nor[i++] = -S1O3; nor[i++] = S1O3; nor[i++] = S1O3;//23
    
    i = 0;
    tex[i++] = 0; tex[i++] = R1O3;//0
    tex[i++] = 0.25; tex[i++] = R1O3;//1
    tex[i++] = 0.25; tex[i++] = R2O3;//2
    tex[i++] = 0; tex[i++] = R2O3;//3
    
    tex[i++] = 0.25; tex[i++] = R1O3;//4
    tex[i++] = 0.5; tex[i++] = R1O3;//5
    tex[i++] = 0.5; tex[i++] = R2O3;//6
    tex[i++] = 0.25; tex[i++] = R2O3;//7
    
    tex[i++] = 0.5; tex[i++] = R1O3;//8
    tex[i++] = 0.75; tex[i++] = R1O3;//9
    tex[i++] = 0.75; tex[i++] = R2O3;//10
    tex[i++] = 0.5; tex[i++] = R2O3;//11
    
    tex[i++] = 0.75; tex[i++] = R1O3;//12
    tex[i++] = 1.0; tex[i++] = R1O3;//13
    tex[i++] = 1.0; tex[i++] = R2O3;//14
    tex[i++] = 0.75; tex[i++] = R2O3;//15
    
    tex[i++] = 0.25; tex[i++] = R2O3;//16
    tex[i++] = 0.5; tex[i++] = R2O3;//17
    tex[i++] = 0.5; tex[i++] = 1.0;//18
    tex[i++] = 0.25; tex[i++] = 1.0;//19
    
    tex[i++] = 0.25; tex[i++] = 0;//20
    tex[i++] = 0.5; tex[i++] = 0;//21
    tex[i++] = 0.5; tex[i++] = R1O3;//22
    tex[i++] = 0.25; tex[i++] = R1O3;//23
    
    i = 0;
    els[i++] = 0; els[i++] = 1; els[i++] = 3;//0
    els[i++] = 2; els[i++] = 1; els[i++] = 3;//1
    els[i++] = 4; els[i++] = 5; els[i++] = 7;//2
    els[i++] = 7; els[i++] = 6; els[i++] = 5;//3
    
    els[i++] = 8; els[i++] = 11; els[i++] = 9;//4
    els[i++] = 11; els[i++] = 10; els[i++] = 9;//5
    els[i++] = 15; els[i++] = 12; els[i++] = 13;//6
    els[i++] = 15; els[i++] = 14; els[i++] = 13;//7
    
    els[i++] = 19; els[i++] = 16; els[i++] = 17;//8
    els[i++] = 19; els[i++] = 18; els[i++] = 17;//9
    els[i++] = 23; els[i++] = 20; els[i++] = 21;//10
    els[i++] = 23; els[i++] = 22; els[i++] = 21;//11
    
    glGenVertexArrays(1, &uiVAO[0]);
    glGenBuffers(4, &uiVBO[0]);
    GetError();
    
    glBindVertexArray(uiVAO[0]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    GetError();
    
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 3*6*4*sizeof(float), ver, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GetError();
    
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 3*6*4*sizeof(float), nor, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, 2*6*4*sizeof(float), tex, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    GetError();
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiVBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*2*6*sizeof(char), els, GL_STATIC_DRAW);
    GetError();
}

ItemSkyBox::~ItemSkyBox() {
    glDeleteVertexArrays(1, uiVAO);
    glDeleteBuffers(4, uiVBO);
    GetError();
}

void ItemSkyBox::draw(ShaderProgram *shp) {
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
    glUniform1f(s->uniform("fTextureContributions[0]"), cf);
    glUniform1f(s->uniform("fTextureContributions[1]"), 1.0f - cf);
    glUniform1i(s->uniform("numTextures"), 2);
    GetError();
    glUniform1f(s->uniform("sunLight.fAmbient"), 1.0f);
    glUniform3f(s->uniform("sunLight.vColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(s->uniform("sunLight.vDirection"), 0.5f, 0.5f, 0.5f);
    GetError();
    
    glBindVertexArray(uiVAO[0]);
    glDrawElements(GL_TRIANGLES, 3*2*6, GL_UNSIGNED_BYTE, (void*)0);
    GetError();
}
