//
//  ItemTorus.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 21/02/2018.
//  Copyright © 2018 AivanF. All rights reserved.
//

#ifndef ItemTorus_hpp
#define ItemTorus_hpp

#include <vector>
#include "Item.hpp"

/// Box with the same texture on each side
class ItemTorus : public Item {
    vector<float> v_pos;
    vector<float> v_tex;
    vector<float> v_nor;
    int vertCount;
    
    GLuint uiVAO[1];
    GLuint uiVBO[3];
    
public:
    ItemTorus(int at1, int at3, int cntin, int cntout, float radin, float radout,
              int turnscnt, float addheight, float addwidth, float texin, float texout);
    ~ItemTorus() override;
    void pushPoint(const glm::vec3 &pos, const glm::vec3 &norm, const glm::vec2 &tex);
    void draw(ShaderProgram *shp, bool full) override;
};

#endif /* ItemTorus_hpp */
