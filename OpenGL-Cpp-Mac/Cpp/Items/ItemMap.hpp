//
//  ItemMap.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Aivan on 07/02/2018.
//  Copyright © 2018 AivanF. All rights reserved.
//

#ifndef ItemMap_hpp
#define ItemMap_hpp

#include "Item.hpp"
#include "VertexBufferObject.hpp"


class ItemMap : public Item {
    bool bLoaded;
    int iRows, iCols;
    
    CVertexBufferObject vboHeightmapData;
    CVertexBufferObject vboHeightmapIndices;
    GLuint uiVAO;
    
    vector< vector< float> > *heights;
    
public:
    ItemMap(string sImagePath, int at1, int at3);
    ~ItemMap() override;
    
    bool LoadHeightMapFromImage(string sImagePath);
    void ReleaseHeightmap();
    
    float getHeight(float x, float z);
    
    void draw(ShaderProgram *shp) override;
};

#endif /* ItemMap_hpp */
