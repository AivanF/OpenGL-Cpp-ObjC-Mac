//
//  ItemTorus.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 21/02/2018.
//  Copyright © 2018 AivanF. All rights reserved.
//

/*
 AivanF. License:
 
 This code is provided 'as-is', without any express or implied warranty.
 You may not hold the author liable.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:
 
 1 The origin of this code must not be misrepresented. You must not claim
 that you wrote the original software. When use the code, you must give appropriate credit,
 provide an active link to the original software, and indicate if changes were made.
 
 2 Altered source versions must be plainly marked as such, and must not be misrepresented
 as being the original software.
 
 3 This notice may not be removed or altered from any source distribution.
 */


#include "ItemTorus.hpp"
#include "ShaderProgram.hpp"
#include "helper.hpp"
#include "../../glm/glm.hpp"
#include "../../glm/gtc/type_ptr.hpp"
#include "../../glm/gtc/matrix_transform.hpp"


void printVecVec(const vector<glm::vec3> &vv) {
    for (int i = 0; i < vv.size(); i++) {
        printVec(vv[i]);
    }
}

glm::vec3 transformit(const glm::vec3 &in, const glm::mat4 &how) {
    glm::vec4 t = how * glm::vec4(in.x, in.y, in.z, 1);
    return glm::vec3(t.x, t.y, t.z);
}

vector<glm::vec3> transformit(const vector<glm::vec3> &ins, const glm::mat4 &how) {
    vector<glm::vec3> res;
    for (int i = 0; i < ins.size(); i++) {
        glm::vec4 t = how * glm::vec4(ins[i].x, ins[i].y, ins[i].z, 1);
        res.push_back(transformit(ins[i], how));
    }
    return res;
}


ItemTorus::ItemTorus(int at1, int at3, int cntin, int cntout, float radin, float radout,
                     int turnscnt, float addheight, float addwidth, float texin, float texout) : Item() {
    t1 = at1; t3 = at3;
    
    if ((0)) {
        // - It can help to understand the math
        glm::vec3 in(0,0,0);
        glm::mat4 mod = glm::mat4();
        mod = glm::rotate(mod, (float)(M_PI), glm::vec3(0, 1, 0));
        mod = glm::translate(mod, glm::vec3(1, 0, 0));
        glm::vec3 out = transformit(in, mod);
        
        printVec(in);
        printMat(mod);
        printVec(out);
    }
    
    vector<glm::vec3> circle;
    int i, j;
    
    // - Firstly, create a circle in X/Y axes
    for (i = 0; i < cntin; i++) {
        float a = 2 * M_PI * i / cntin;
        circle.push_back(glm::vec3( radin*cos(a), radin*sin(a), 0 ));
    }
    
    // - Compute the count of circles
    if (turnscnt < 1)
        turnscnt = 1;
    cntout *= turnscnt;
    vertCount = 0;
    
    for (i = 0; i < cntout; i++) {
        // - Move the circles by X axe and rotate around Y twice to get two circle of the torus
        // - Add vertical / horizontal shift to taste
        glm::mat4 mod1 = glm::mat4();
        mod1 = glm::rotate(mod1, (float)(turnscnt * 2 * M_PI * i / cntout), glm::vec3(0, 1, 0));
        mod1 = glm::translate(mod1, glm::vec3(radout + addwidth * i / cntout,
                                              addheight * (i-cntout/2) / cntout, 0));
        
        glm::mat4 mod2 = glm::mat4();
        mod2 = glm::rotate(mod2, (float)(turnscnt * 2 * M_PI * (i+1) / cntout), glm::vec3(0, 1, 0));
        mod2 = glm::translate(mod2, glm::vec3(radout + addwidth * (i+1) / cntout,
                                              addheight * (i+1-cntout/2) / cntout, 0));
        
        vector<glm::vec3> news1 = transformit(circle, mod1);
        vector<glm::vec3> news2 = transformit(circle, mod2);
        
        glm::vec3 c1 = transformit(glm::vec3(), mod1);
        glm::vec3 c2 = transformit(glm::vec3(), mod2);
        
        // - Create a surface by connecting the vertices of two circles
        for (j = 0; j <= cntin; j++) {
            int c = j % cntin;
            int p = (j+1) % cntin;
            glm::vec3 p11 = news1[c];
            glm::vec3 p12 = news1[p];
            glm::vec3 p21 = news2[c];
            glm::vec3 p22 = news2[p];
            
            glm::vec3 n11 = glm::normalize(p11 - c1);
            glm::vec3 n12 = glm::normalize(p12 - c1);
            glm::vec3 n21 = glm::normalize(p21 - c2);
            glm::vec3 n22 = glm::normalize(p22 - c2);
            
            // - Stretch the texture
            // If scaler is negative, then each triangles pair gets a full texture.
            // Otherwise, they specify count of texture for inner and outer circles.
            float u0, u1, v0, v1;
            if (texout > 0) {
                u0 = texout * (float)i / (float)cntout;
                u1 = texout * (float)(i+1) / (float)cntout;
            } else {
                u0 = 0;
                u1 = 1;
            }
            if (texin > 0) {
                v0 = texin * (float)j / (float)cntin;
                v1 = texin * (float)(j+1) / (float)cntin;
            } else {
                v0 = 0;
                v1 = 1;
            }
            glm::vec2 t11 = glm::vec2(u0, v0);
            glm::vec2 t12 = glm::vec2(u0, v1);
            glm::vec2 t21 = glm::vec2(u1, v0);
            glm::vec2 t22 = glm::vec2(u1, v1);
            
            pushPoint(p11, n11, t11);
            pushPoint(p21, n21, t21);
            pushPoint(p22, n22, t22);
            
            pushPoint(p22, n22, t22);
            pushPoint(p12, n12, t12);
            pushPoint(p11, n11, t11);
        }
    }
    printf("Torus vertices count: %d\n", vertCount);
    
    // - Send data to OpenGL buffers
    glGenVertexArrays(1, &uiVAO[0]);
    glGenBuffers(3, &uiVBO[0]);
    GetError();
    
    glBindVertexArray(uiVAO[0]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    GetError();
    
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 3*vertCount*sizeof(float), v_pos.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 3*vertCount*sizeof(float), v_nor.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, 2*vertCount*sizeof(float), v_tex.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    GetError();
    
    // OpenGL copies the data, so, we have no need to store it.
    v_pos.clear();
    v_nor.clear();
    v_tex.clear();
}

ItemTorus::~ItemTorus() {
    glDeleteVertexArrays(1, uiVAO);
    glDeleteBuffers(3, uiVBO);
    GetError();
}

void ItemTorus::pushPoint(const glm::vec3 &pos, const glm::vec3 &norm, const glm::vec2 &tex) {
    vertCount++;
    v_pos.push_back(pos.x);
    v_pos.push_back(pos.y);
    v_pos.push_back(pos.z);
    v_nor.push_back(norm.x);
    v_nor.push_back(norm.y);
    v_nor.push_back(norm.z);
    v_tex.push_back(tex.x);
    v_tex.push_back(tex.y);
}

void ItemTorus::draw(ShaderProgram *s) {
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
    glUniform1f(s->uniform("fTextureContributions[0]"), 1.0f);
    glUniform1f(s->uniform("fTextureContributions[1]"), 0.0f);
    glUniform1i(s->uniform("numTextures"), 1);
    GetError();
    
    glBindVertexArray(uiVAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, vertCount);
    GetError();
}


