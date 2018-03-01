//
//  Item.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 05/12/2017.
//

#ifndef Item_hpp
#define Item_hpp

#include "ShaderProgram.hpp"

#include "../../glm/glm.hpp"
#include <vector>
#include <string>

using namespace std;


/// Abstract class
class Item {
public:
    // Rotation speed
    float _rsx, _rsy, _rsz;
    // Rotation
    float _rx, _ry, _rz;
    // Position
    float _px, _py, _pz;
    // Scale
    float _sx, _sy, _sz, _sc;
    // Textures: just image, second image, and normal map
    char t1, t2, t3;
    float cf;
    
    Item();
    virtual ~Item() {};
    virtual void draw(ShaderProgram *shpm, bool full) = 0;
    glm::mat4 apply();
    
    /// Sets position of the Item.
    Item* place(float x, float y, float z);
    /// Sets rotation of the Item.
    Item* rotate(float rx, float ry, float rz);
    /// Sets rotation of the Item for X axes.
    Item* rotateX(float rx);
    /// Sets rotation of the Item for Y axes.
    Item* rotateY(float ry);
    /// Sets rotation of the Item for Z axes.
    Item* rotateZ(float rz);
    /// Sets rotation speed of the Item.
    Item* kick(float rsx, float rsy, float rsz);
    /// Sets scale by axes of the Item.
    Item* scale(float sx, float sy, float sz);
    /// Sets scale of the Item.
    Item* scaleAll(float sc);
    
    void processStep();
};

#endif /* Item_hpp */
