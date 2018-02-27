//
//  ItemBox.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 06/12/2017.
//

#ifndef ItemBox_hpp
#define ItemBox_hpp

#include "Item.hpp"

/// Box with the same texture on each side
class ItemBox : public Item {
    /// Vertex coordinates
    float ver[3*6*4];
    /// Vertex normals
    float nor[3*6*4];
    /// Texture coordinates
    float tex[2*6*4];
    /// Elements indices
    unsigned char els[3*2*6];
    
    GLuint uiVBO[4];
    GLuint uiVAO[1];
    
public:
    ItemBox(int at1, int at3);
    ~ItemBox() override;
    void draw(ShaderProgram *shp) override;
};


#endif /* ItemBox_hpp */
