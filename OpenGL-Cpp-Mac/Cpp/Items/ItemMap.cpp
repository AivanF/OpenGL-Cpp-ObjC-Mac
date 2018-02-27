//
//  ItemMap.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Aivan on 07/02/2018.
//  Copyright © 2018 AivanF. All rights reserved.
//

#include "ItemMap.hpp"
#include "ShaderProgram.hpp"
#include "helper.hpp"

#include "../../glm/gtc/type_ptr.hpp"
#include "../../FreeImage/FreeImage.h"


ItemMap::ItemMap(string sImagePath, int at1, int at3) : Item() {
    _sx = 160.0f;
    _sz = _sx;
    
    _sy = 10.0f;
//    _py = -0.5f - _sy / 2;
    
    t1 = at1; t3 = at3;
    
    bLoaded = false;
    LoadHeightMapFromImage(sImagePath);
}

ItemMap::~ItemMap() {
    ReleaseHeightmap();
}

bool ItemMap::LoadHeightMapFromImage(string sImagePath) {
    if (bLoaded) {
        bLoaded = false;
        ReleaseHeightmap();
    }
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* dib(0);
    
    fif = FreeImage_GetFileType(sImagePath.c_str(), 0); // Check the file signature and deduce its format
    
    if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
        fif = FreeImage_GetFIFFromFilename(sImagePath.c_str());
    
    if(fif == FIF_UNKNOWN) // If still unknown, return failure
        return false;
    
    if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
        dib = FreeImage_Load(fif, sImagePath.c_str());
    if(!dib)
        return false;
    
    BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
    iRows = FreeImage_GetHeight(dib);
    iCols = FreeImage_GetWidth(dib);
    
    // We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
    if(bDataPointer == NULL || iRows == 0 || iCols == 0 || (FreeImage_GetBPP(dib) != 24 && FreeImage_GetBPP(dib) != 8))
        return false;
    
    // How much to increase data pointer to get to next pixel data
    unsigned int ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;
    // Length of one row in data
    unsigned int row_step = ptr_inc*iCols;
    
    vboHeightmapData.CreateVBO();
    // All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
    vector< vector< glm::vec3> > vVertexData(iRows, vector<glm::vec3>(iCols));
    vector< vector< glm::vec2> > vCoordsData(iRows, vector<glm::vec2>(iCols));
    heights = new vector< vector< float> >(iRows, vector<float>(iCols));
    
    float fTextureU = float(iCols)*0.1f;
    float fTextureV = float(iRows)*0.1f;
    
    FOR(i, iRows)
    {
        FOR(j, iCols)
        {
            float fScaleC = float(j)/float(iCols-1);
            float fScaleR = float(i)/float(iRows-1);
            float fVertexHeight = float(*(bDataPointer+row_step*i+j*ptr_inc))/255.0f - 0.5f;
            vVertexData[i][j] = glm::vec3(-0.5f+fScaleC, fVertexHeight, -0.5f+fScaleR);
            vCoordsData[i][j] = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR);
            heights[0][i][j] = fVertexHeight;
        }
    }
    
    // Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
    vector< vector<glm::vec3> > vNormals[2];
    FOR(i, 2)vNormals[i] = vector< vector<glm::vec3> >(iRows-1, vector<glm::vec3>(iCols-1));
    
    FOR(i, iRows-1)
    {
        FOR(j, iCols-1)
        {
            glm::vec3 vTriangle0[] =
            {
                vVertexData[i][j],
                vVertexData[i+1][j],
                vVertexData[i+1][j+1]
            };
            glm::vec3 vTriangle1[] =
            {
                vVertexData[i+1][j+1],
                vVertexData[i][j+1],
                vVertexData[i][j]
            };
            
            glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0]-vTriangle0[1], vTriangle0[1]-vTriangle0[2]);
            glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0]-vTriangle1[1], vTriangle1[1]-vTriangle1[2]);
            
            vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
            vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
        }
    }
    
    vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));
    
    FOR(i, iRows)
    FOR(j, iCols)
    {
        // Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
        // of all adjacent triangles' normals
        
        glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);
        
        // Look for upper-left triangles
        if(j != 0 && i != 0)
            FOR(k, 2)vFinalNormal += vNormals[k][i-1][j-1];
        // Look for upper-right triangles
        if(i != 0 && j != iCols-1)vFinalNormal += vNormals[0][i-1][j];
        // Look for bottom-right triangles
        if(i != iRows-1 && j != iCols-1)
            FOR(k, 2)vFinalNormal += vNormals[k][i][j];
        // Look for bottom-left triangles
        if(i != iRows-1 && j != 0)
            vFinalNormal += vNormals[1][i][j-1];
        vFinalNormal = glm::normalize(vFinalNormal);
        
        vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
    }
    
    // First, create a VBO with only vertex data
    // Preallocate memory
    vboHeightmapData.CreateVBO(iRows*iCols*(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
    
    FOR(i, iRows)
    {
        FOR(j, iCols)
        {
            vboHeightmapData.AddData(&vVertexData[i][j], sizeof(glm::vec3)); // Add vertex
            vboHeightmapData.AddData(&vCoordsData[i][j], sizeof(glm::vec2)); // Add tex. coord
            vboHeightmapData.AddData(&vFinalNormals[i][j], sizeof(glm::vec3)); // Add normal
        }
    }
    // Now create a VBO with heightmap indices
    vboHeightmapIndices.CreateVBO();
    int iPrimitiveRestartIndex = iRows*iCols;
    FOR(i, iRows-1)
    {
        FOR(j, iCols)
        FOR(k, 2)
        {
            int iRow = i+(1-k);
            int iIndex = iRow*iCols+j;
            vboHeightmapIndices.AddData(&iIndex, sizeof(int));
        }
        // Restart triangle strips
        vboHeightmapIndices.AddData(&iPrimitiveRestartIndex, sizeof(int));
    }
    
    glGenVertexArrays(1, &uiVAO);
    glBindVertexArray(uiVAO);
    // Attach vertex data to this VAO
    vboHeightmapData.BindVBO(GL_ARRAY_BUFFER);
    vboHeightmapData.UploadDataToGPU(GL_STATIC_DRAW);
    
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
    
    // And now attach index data to this VAO
    vboHeightmapIndices.BindVBO(GL_ELEMENT_ARRAY_BUFFER);
    vboHeightmapIndices.UploadDataToGPU(GL_STATIC_DRAW);
    
    GetError();
    bLoaded = true; // If get here, we succeeded with generating heightmap
    return true;
}

void ItemMap::ReleaseHeightmap() {
    if (!bLoaded)
        return;
    vboHeightmapData.DeleteVBO();
    vboHeightmapIndices.DeleteVBO();
    glDeleteVertexArrays(1, &uiVAO);
    GetError();
    bLoaded = false;
    delete heights;
}

void ItemMap::draw(ShaderProgram *shp) {
    ShaderProgram *s = (ShaderProgram*)shp;
    
    glm::mat4 mModel = apply();
    glm::mat4 mNormal = transpose(inverse(mModel));
    glUniformMatrix4fv(s->uniform("matrices.modelMatrix"), 1, GL_FALSE, glm::value_ptr(mModel));
    glUniformMatrix4fv(s->uniform("matrices.normalMatrix"), 1, GL_FALSE, glm::value_ptr(mNormal));
    
    GetError();
    glUniform1i(s->uniform("gSamplers[0]"), 0);
    GetError();
    glUniform1i(s->uniform("gSamplers[1]"), 1);
    GetError();
    glUniform1i(s->uniform("gSamplers[2]"), 2);
    GetError();
    glUniform1f(s->uniform("fTextureContributions[0]"), cf);
    glUniform1f(s->uniform("fTextureContributions[1]"), 1.0f - cf);
    glUniform1i(s->uniform("numTextures"), 2);
    GetError();
    
    glBindVertexArray(uiVAO);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(iRows*iCols);
    
    int iNumIndices = (iRows-1)*iCols*2 + iRows-1;
    glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);
    
    glDisable(GL_PRIMITIVE_RESTART);
    GetError();
}

float ItemMap::getHeight(float x, float z) {
    if (!bLoaded)
        return 2.0f;
    
    int p = (int)(0.5f * iRows * x / _sx + 0.5f);
    int q = (int)(0.5f * iCols * z / _sz + 0.5f);
    p = min(max(0, p), iRows - 1);
    q = min(max(0, q), iCols - 1);
    return heights[0][p][q] * _sy + _py;
}

