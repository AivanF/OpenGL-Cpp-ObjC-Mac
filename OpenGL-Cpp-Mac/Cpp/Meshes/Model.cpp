//
//  Model.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 24/01/2018.
//

#include "Model.hpp"

#include "helper.hpp"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "ShaderProgram.hpp"


Model::Model(const char *modelPath) {
#ifdef PRINT_MODELS
    printf("Model loading: %s\n", modelPath);
#endif
    
    Assimp::Importer imp;
    const aiScene* scene = imp.ReadFile(modelPath,
                                        aiProcess_JoinIdenticalVertices |
                                        aiProcess_SortByPType);
    
    if (!scene) {
        printf("ItemModel: scene was not loaded!");
    }
    
    iTotalVertices = 0;
    int size = 0;
    
    FOR(i, scene->mNumMeshes) {
        aiMesh* mesh = scene->mMeshes[i];
        int iMeshFaces = mesh->mNumFaces;
        iMaterialIndices.push_back(mesh->mMaterialIndex);
        int oldsize = size;
        iMeshStartIndices.push_back(oldsize);
        FOR(j, iMeshFaces) {
            const aiFace& face = mesh->mFaces[j];
            
            FOR(k, 3) {
                aiVector3D pos = mesh->mVertices[face.mIndices[k]];
                aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
                aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
                
                v_pos.push_back(pos.x);
                v_pos.push_back(pos.y);
                v_pos.push_back(pos.z);
                
                v_tex.push_back(uv.x);
                v_tex.push_back(uv.y);
                
                v_nor.push_back(normal.x);
                v_nor.push_back(normal.y);
                v_nor.push_back(normal.z);
                
                size++;
            }
            
            // add one more triangle to make a quad
            if (face.mNumIndices > 3) {
                FOR(p, 3) {
                    int k = (p + 2) % 4;
                    
                    aiVector3D pos = mesh->mVertices[face.mIndices[k]];
                    aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
                    aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
                    
                    v_pos.push_back(pos.x);
                    v_pos.push_back(pos.y);
                    v_pos.push_back(pos.z);
                    
                    v_tex.push_back(uv.x);
                    v_tex.push_back(uv.y);
                    
                    v_nor.push_back(normal.x);
                    v_nor.push_back(normal.y);
                    v_nor.push_back(normal.z);
                    
                    size++;
                }
            }
        }
        int iMeshVertices = mesh->mNumVertices;
        iTotalVertices += iMeshVertices;
        iMeshSizes.push_back(size - oldsize);
    }
#ifdef PRINT_MODELS
    printf("Total count: %d\n", (int)iTotalVertices);
#endif
    iTotalVertices = size;
#ifdef PRINT_MODELS
    printf("Vertex count: %d\n", (int)iTotalVertices);
#endif
    
    
    
    iNumMaterials = scene->mNumMaterials;
    vector<int> materialRemap(iNumMaterials);
#ifdef PRINT_MODELS
    printf("Materials count: %d\n", iNumMaterials);
#endif
    FOR(i, iNumMaterials) {
        const aiMaterial* material = scene->mMaterials[i];
        int texIndex = 0;
        aiString path;
        
        if(material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS) {
            string sDir = extractDirectory(modelPath);
            string sTextureName = path.data;
            string sFullPath = sDir + sTextureName;
            int iTexFound = -1;
#ifdef PRINT_MODELS
            printf("Texture: %s\n", sTextureName.c_str());
#endif
            
            FOR(j, ESZ(tTextures))
            if(sFullPath == tTextures[j].GetPath()) {
                iTexFound = j;
                break;
            }
            
            if (iTexFound != -1)
                materialRemap[i] = iTexFound;
            else {
                CTexture tNew;
                tNew.LoadTexture2D(sFullPath, true);
                materialRemap[i] = ESZ(tTextures);
                tTextures.push_back(tNew);
            }
        }
    }
    
    FOR(i, ESZ(iMeshSizes)) {
        int iOldIndex = iMaterialIndices[i];
        iMaterialIndices[i] = materialRemap[iOldIndex];
    }
#ifdef PRINT_MODELS
    printf("Meshes count: %d\n", (int)ESZ(iMeshSizes));
#endif
    
    GetError();
    
    glGenVertexArrays(1, &uiVAO[0]);
    glGenBuffers(3, &uiVBO[0]);
    GetError();
    
    glBindVertexArray(uiVAO[0]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    GetError();
    
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 3*iTotalVertices*sizeof(float), v_pos.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 3*iTotalVertices*sizeof(float), v_nor.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, 2*iTotalVertices*sizeof(float), v_tex.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    GetError();
}

Model::~Model() {
    glDeleteVertexArrays(1, uiVAO);
    glDeleteBuffers(3, uiVBO);
    GetError();
}

void Model::draw(ShaderProgram *s) {
    glBindVertexArray(uiVAO[0]);
    
    glUniform1i(s->uniform("useNormalMap"), 0);
    
    int iNumMeshes = ESZ(iMeshSizes);
    FOR(i, iNumMeshes) {
        int iMatIndex = iMaterialIndices[i];
        tTextures[iMatIndex].BindTexture(0);
        glDrawArrays(GL_TRIANGLES, iMeshStartIndices[i], iMeshSizes[i]);
    }
    
    glUniform1i(s->uniform("useNormalMap"), 1);
    GetError();
}


