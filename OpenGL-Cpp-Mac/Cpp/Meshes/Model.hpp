//
//  Model.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 24/01/2018.
//

#ifndef Model_hpp
#define Model_hpp

#include <vector>
#include "Texture.hpp"

using namespace std;

class ShaderProgram;

class Model {
    vector<float> v_pos;
    vector<float> v_tex;
    vector<float> v_nor;
    int iTotalVertices;
    
    GLuint uiVAO[1];
    GLuint uiVBO[3];
    
    vector<CTexture> tTextures;
    vector<int> iMeshStartIndices;
    vector<int> iMeshSizes;
    vector<int> iMaterialIndices;
    int iNumMaterials;
    
public:
    Model(const char *modelPath);
    ~Model();
    
    void draw(ShaderProgram *s);
};

#endif /* Model_hpp */
