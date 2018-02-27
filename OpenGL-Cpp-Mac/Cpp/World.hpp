//
//  World.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 05/12/2017.
//

#ifndef World_hpp
#define World_hpp

#include <vector>

#include "Item.hpp"

#include "ShaderProgram.hpp"
#include "Texture.hpp"

#include "dirLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "fog.h"

#include "FreeTypeFont.hpp"
#include "ParticleSystem.hpp"
#include "ItemMap.hpp"

#define NUMTEXTURES 10
#define NUMPARTS 2

class World {
    Item* _sky;
    std::vector<Item*> _items;
    std::vector<CPointLight*> _points;
    std::vector<CSpotLight*> _spots;
    // Texturd, Font
    ShaderProgram *_shpt, *_shpf;
    CDirectionalLight _dirlight;
    CPointLight _sun;
    CFog _fog;
    CTexture textures[NUMTEXTURES];
    CFreeTypeFont fnt1, fnt2;
    CParticleSystemTransformFeedback parts[NUMPARTS];
    ItemMap *map;
    
    // Time parameter, values 0 to 359
    int par = 0;
    bool moving = true;
    bool normalmap = true;
    
    void prepareFX();
    void draw2D(float w, float h);
    void draw3D(float w, float h);
    
public:
    float _rx, _ry;
    float _px, _py, _pz;
    
    World();
    ~World();
    
    void move(float mx, float my);
    void rotate(float rx, float ry);
    void draw(float w, float h);
    void keyTab();
    void keyEnter();
    void keySpace();
    void keyShift();
    int fogtype();
};

#endif /* World_hpp */
