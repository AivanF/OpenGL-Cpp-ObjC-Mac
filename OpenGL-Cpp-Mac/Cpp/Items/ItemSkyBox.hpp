//
//  ItemSkyBox.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 06/12/2017.
//

#ifndef ItemSkyBox_hpp
#define ItemSkyBox_hpp

#include "Item.hpp"

/// Box for special 6-sides texture
class ItemSkyBox : public Item {
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
    ItemSkyBox(int at1, int at2, int at3);
    ~ItemSkyBox() override;
    void draw(ShaderProgram *shp) override;
};


#endif /* ItemSkyBox_hpp */
