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
#include "CubeSimplifier.hpp"

#include "../glm/glm.hpp"

#define NUMTEXTURES 10
#define NUMPARTS 2

class World {
    Item* _sky;
    std::vector<Item*> _items;
    std::vector<CPointLight*> _points;
    std::vector<CSpotLight*> _spots;
    // Textured, Simple, Font, CubeToPlain.
    ShaderProgram *_shpt, *_shps, *_shpf, *_shpc2p;
    CDirectionalLight _dirlight;
    CPointLight _sun;
    CFog _fog;
    CTexture textures[NUMTEXTURES];
    CFreeTypeFont fnt1, fnt2;
    CParticleSystemTransformFeedback parts[NUMPARTS];
    ItemMap *map;
    CubeSimplifier c2p;
    
    glm::mat4 _savedMView[6], _savedMProj;
    
    // Time parameter, values 0 to 359
    int par = 0;
    bool moving = true;
    bool normalmap = true;
    
    void renderShadows();
    void prepareFX();
    void draw2D(float w, float h);
    void draw3D(float w, float h);
    
public:
    float _rx, _ry;
    float _px, _py, _pz;
    
    int light_target = 0;
    int light_cube_face = 6;
    
    World();
    ~World();
    
    void move(float mx, float my);
    void rotate(float rx, float ry);
    void draw(float w, float h);
    void keyTab();
    void keyEnter();
    void keySpace();
    void keyShift();
    void nextLightPoint();
    int fogtype();
    float getPX();
    float getPY();
    float getPZ();
};

#endif /* World_hpp */
