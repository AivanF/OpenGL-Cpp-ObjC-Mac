//
//  VertexBufferObject.hpp
//

#ifndef VertexBufferObject_hpp
#define VertexBufferObject_hpp

#include <OpenGL/gl3.h>
#include <vector>
#include <string>

using namespace std;


class CVertexBufferObject {
public:
    void CreateVBO(int a_iSize = 0);
    void DeleteVBO();
    
    void* MapBufferToMemory(int iUsageHint);
    void* MapSubBufferToMemory(int iUsageHint, GLuint uiOffset, GLuint uiLength);
    void UnmapBuffer();
    
    void BindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
    void UploadDataToGPU(int iUsageHint);
    
    void AddData(void* ptrData, GLuint uiDataSize);
    
    void* GetDataPointer();
    GLuint GetBufferID();
    
    int GetCurrentSize();
    
    CVertexBufferObject();
    
private:
    GLuint uiBuffer;
    int iSize;
    int iCurrentSize;
    int iBufferType;
    vector<char> data;
    
    bool bDataUploaded;
};

#endif /* VertexBufferObject_hpp */
