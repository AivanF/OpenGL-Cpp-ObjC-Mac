//
//  ItemModel.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 21/01/2018.
//

#include "ItemModel.hpp"

#include "ShaderProgram.hpp"
#include "helper.hpp"
#include "../../glm/gtc/type_ptr.hpp"


ItemModel::ItemModel(Model *m) : Item() {
    _m = m;
}

void ItemModel::draw(ShaderProgram *shp) {
    ShaderProgram *s = (ShaderProgram*)shp;
    
    glm::mat4 mModel = apply();
    glm::mat4 mNormal = transpose(inverse(mModel));
    glUniformMatrix4fv(s->uniform("matrices.modelMatrix"), 1, GL_FALSE, glm::value_ptr(mModel));
    glUniformMatrix4fv(s->uniform("matrices.normalMatrix"), 1, GL_FALSE, glm::value_ptr(mNormal));
    
    GetError();
    glUniform1i(s->uniform("gSamplers[0]"), 0);
    GetError();
    glUniform1f(s->uniform("fTextureContributions[0]"), 1.0f);
//    glUniform1f(s->uniform("fTextureContributions[1]"), 1.0f - cf);
    glUniform1i(s->uniform("numTextures"), 1);
    GetError();
    
    _m->draw(s);
}

