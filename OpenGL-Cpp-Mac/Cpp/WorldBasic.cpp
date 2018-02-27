//
//  WorldBasic.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 24/01/2018.
//

#include "World.hpp"
#include "shaders.hpp"
#include "helper.hpp"

#include "../glm/gtc/matrix_transform.hpp"

#include "ItemPlane.hpp"
#include "ItemBox.hpp"
#include "ItemSkyBox.hpp"
#include "ItemModel.hpp"
#include "ItemTorus.hpp"


void loadTexture(CTexture &obj, const char* name) {
    const char* path = getPath(name, "jpg");
    obj.LoadTexture2D(path, true);
    obj.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
}

World::World() {
    //--------- Initial camera geometry
    
    _rx = 0; _ry = 0;
    _px = 0; _py = -1; _pz = 2.0f;
    map = nullptr;
    
    //--------- Create shaders
    {
        GLuint sh_full_vert = makeShaderFromFile(GL_VERTEX_SHADER, "full", "vert");
        GLuint sh_full_frag = makeShaderFromFile(GL_FRAGMENT_SHADER, "full", "frag");
        
        _shpt = new ShaderProgram();
        _shpt->addShader(sh_full_vert);
        _shpt->addShader(sh_full_frag);
        _shpt->linkProgram();
    }{
        GLuint sh_ortho_vert = makeShaderFromFile(GL_VERTEX_SHADER, "ortho2D", "vert");
//        GLuint sh_ortho_frag = makeShaderFromFile(GL_FRAGMENT_SHADER, "ortho2D", "frag");
        GLuint sh_font_frag = makeShaderFromFile(GL_FRAGMENT_SHADER, "font2D", "frag");
        
        _shpf = new ShaderProgram();
        _shpf->addShader(sh_ortho_vert);
        _shpf->addShader(sh_font_frag);
        _shpf->linkProgram();
    }
    
    //--------- Add basic items
    
    // plane
    _items.push_back(new ItemPlane(0, 1));
    
    // height map
//    map = new ItemMap(getPath("path", "jpg"), 0, 1);
//    _items.push_back(map);
    
    // tower
    _items.push_back((new ItemBox(2, 3))->place(5, 0, 6));
    _items.push_back((new ItemBox(2, 3))->place(5, 0, 5));
    _items.push_back((new ItemBox(2, 3))->place(5, 1, 5)->rotateY(30));
    _items.push_back((new ItemBox(2, 3))->place(5, 2, 5)->rotateY(60));
    
    // for green
    _items.push_back((new ItemBox(2, 3))->place(-5, 0, 8)->rotateY(35));
    // for blue
    _items.push_back((new ItemBox(2, 3))->place(0, 0, 0)->rotateY(45));
    _items.push_back((new ItemBox(2, 3))->place(3, 2, 2));
    // for purple
    _items.push_back((new ItemBox(2, 3))->place(0, 2, -5));
    
    //--------- Add generated items
    
    // torus
    _items.push_back((new ItemTorus(2, 3,
                                    12, // count in
                                    12, // count out
                                    1.0f, // rad in
                                    3.0f, // rad out
                                    1, // turns count
                                    0.0f, // add height
                                    0.0f, // addwidth
                                    0.5f, // tex in
                                    1 // tex out
                                    ))->place(0, 8, 0)->kick(0.5f, 1.0f, 0.25f)->scaleAll(0.5f));
    
    _items.push_back((new ItemTorus(8, 9,
                                    6, // count in
                                    24, // count out
                                    0.5f, // rad in
                                    2.0f, // rad out
                                    3, // turns count
                                    4.0f, // add height
                                    2.0f, // addwidth
                                    1, // tex in
                                    8 // tex out
                                    ))->place(10, 15, 10)->kick(0.5f, -0.5f, 0.0f));
    
    //--------- Add models
    
//    Model *wolf = new Model(getPath("Wolf", "obj"));
    Model *house = new Model(getPath("house", "3ds"));
    Model *tree = new Model(getPath("DeadTree", "obj"));
    
//    _items.push_back((new ItemModel(wolf))->place(-7, 0, -5.5)->rotateY(0));
//    _items.push_back((new ItemModel(wolf))->place(-5.5f, 0, -4.5)->rotateY(30));
//    _items.push_back((new ItemModel(wolf))->place(-3.5f, 0, -5.5)->rotateY(45));
    
    _items.push_back((new ItemModel(house))->place(-6, 0, -12.5)->scaleAll(2)->rotateY(15));
    
    _items.push_back((new ItemModel(tree))->place(4, 0, -6.5)->rotateY(60));
    _items.push_back((new ItemModel(tree))->place(5, 0, -12.5)->rotateY(90));
    
    _items.push_back((new ItemModel(tree))->place(-14, 0, -6.5)->rotateY(45));
    _items.push_back((new ItemModel(tree))->place(-17, 0, -11.5)->rotateY(30));
    _items.push_back((new ItemModel(tree))->place(-18, 0, -3.5)->rotateY(120));
    
    //--------- Add Sky Box
    
    _sky = new ItemSkyBox(4, 5, 6);
    _sky->_sc = -5; _sky->_py = -0.4;
    
    //--------- Add particle systems
    float pr;
    
    // tower
    pr = 1.0f;
    parts[1].SetGeneratorProperties(
        glm::vec3(5.0f, 2.0f, 5.0f), // Where the particles are generated
        glm::vec3(-pr, 1, -pr), // Minimal velocity
        glm::vec3(pr, 5, pr), // Maximal velocity
        glm::vec3(0, -3, 0), // Gravity force applied to particles
        glm::vec3(0.25f, 0.75f, 1.0f), // Color
        0.5f, // Minimum lifetime in seconds
        3.0f, // Maximum lifetime in seconds
        0.25f, // Rendered size
        0.02f, // Spawn delay
        30); // And spawn 30 particles
    
//    // house
//    pr = 0.5f;
//    parts[2].SetGeneratorProperties(
//        glm::vec3(-6.0f, 0.0f, -12.5f), // Where the particles are generated
//        glm::vec3(-pr, 0, -pr), // Minimal velocity
//        glm::vec3(pr, 2, pr), // Maximal velocity
//        glm::vec3(0, -0.05, 0), // Gravity force applied to particles
//        glm::vec3(1.0f, 0.7f, 1.0f), // Color
//        0.5f, // Minimum lifetime in seconds
//        2.0f, // Maximum lifetime in seconds
//        0.25f, // Rendered size
//        0.05f, // Spawn delay
//        10); // And spawn 30 particles
//
//    // field
//    pr = 4;
//    parts[3].SetGeneratorProperties(
//        glm::vec3(-15.0f, -1.0f, 20.0f), // Where the particles are generated
//        glm::vec3(-pr, 1, -pr), // Minimal velocity
//        glm::vec3(pr, 5, pr), // Maximal velocity
//        glm::vec3(0, -3, 0), // Gravity force applied to particles
//        glm::vec3(0.2f, 1.0f, 0.3f), // Color
//        2.0f, // Minimum lifetime in seconds
//        6.0f, // Maximum lifetime in seconds
//        0.5f, // Rendered size
//        10.0f, // Spawn delay
//        30); // And spawn 30 particles
    
    //--------- Add point lights
    
    _points.push_back(new CPointLight());
    _points.push_back(new CPointLight());
    
    //--------- Add spot lights
    
    _spots.push_back(new CSpotLight());
    _spots.push_back(new CSpotLight());
    
    //--------- Load textures
    
//    loadTexture(textures[0], "grass"); loadTexture(textures[1], "grass_n");
    loadTexture(textures[0], "mygrass"); loadTexture(textures[1], "mygrass_n");
    
    loadTexture(textures[2], "box"); loadTexture(textures[3], "box_n");
    loadTexture(textures[4], "sky_sunset");
    loadTexture(textures[5], "sky_snow"); loadTexture(textures[6], "sky_snow_n");
    
    loadTexture(textures[7], "particle");
    
    loadTexture(textures[8], "clothes"); loadTexture(textures[9], "clothes_n");
    
    //--------- Load fonts
//    fnt2.LoadFont(getPath("Kaushan", "otf"), 128);
    fnt2.LoadFont(getPath("tarzeau", "ttf"), 128);
    fnt1.LoadFont(getPath("Allura", "otf"), 128);
//    fnt1.LoadFont(getPath("Pacifico", "ttf"), 128);
    
    fnt1.SetShaderProgram(_shpf);
    fnt2.SetShaderProgram(_shpf);
}

World::~World() {
    for (int i = 0; i < _items.size(); i++) {
        delete _items[i];
    }
    for (int i = 0; i < _points.size(); i++) {
        delete _points[i];
    }
    for (int i = 0; i < _spots.size(); i++) {
        delete _spots[i];
    }
    delete _sky;
    delete _shpt;
    
    fnt1.DeleteFont();
    fnt2.DeleteFont();
}

void World::move(float mx, float my) {
    glm::mat4 m;
    m = glm::rotate(m, -(float)degreesToRadians(_ry), glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::rotate(m, -(float)degreesToRadians(_rx), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec4 v(-mx, 0.0f, my, 0.0f);
    v = m * v;
//    printf("- %f %f\n", _rx, _ry);
//    printf("%f %f %f %f\n", v[0], v[1], v[2], v[3]);
    _px += v[0];
    _py += v[1];
    _pz += v[2];
    
    if (map) {
        _py = -map->getHeight(_px, _pz) - 2;
    }
}

void World::rotate(float rx, float ry) {
    _rx += rx;
    _ry += ry;
    if (_rx > 90)
        _rx = 90;
    if (_rx < -90)
        _rx = -90;
    
    if (_ry > 180)
        _ry -= 360;
    if (_ry < -180)
        _ry += 360;
}

void World::keyTab() {
    _fog.iEquation = (_fog.iEquation + 1) % 4;
}

void World::keyEnter() {
    CPointLight *l1 = _points[0];
    l1->active = !l1->active;
    CSpotLight *l2 = _spots[0];
    l2->active = !l2->active;
}

void World::keySpace() {
    moving = !moving;
}

void World::keyShift() {
    normalmap = !normalmap;
}

int World::fogtype() {
    return _fog.iEquation;
}


