//
//  ItemPlane.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 06/12/2017.
//

#ifndef ItemPlane_hpp
#define ItemPlane_hpp

#include "Item.hpp"

/// Plane with a texture
class ItemPlane : public Item {
    /// Vertex coordinates
    float ver[12];
    /// Vertex normals
    float nor[12];
    /// Texture coordinates
    float tex[12];
    
    GLuint uiVBO[3];
    GLuint uiVAO[1];
    
public:
    ItemPlane(int at1, int at3);
    ~ItemPlane() override;
    void draw(ShaderProgram *shp) override;
};

#endif /* ItemPlane_hpp */
