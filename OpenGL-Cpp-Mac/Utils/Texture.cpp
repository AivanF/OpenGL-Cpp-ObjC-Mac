//
//  Texture.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 05/12/2017.
//

#include "Texture.hpp"
#include "../FreeImage/FreeImage.h"
#include "helper.hpp"

CTexture::CTexture() {
    bMipMapsGenerated = false;
}

// Creates texture from provided data.
// a_iWidth, a_iHeight - dimensions
// format - format of data
void CTexture::CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format) {
    glGenTextures(1, &uiTexture);
    glBindTexture(GL_TEXTURE_2D, uiTexture);
    if(format == GL_RGBA || format == GL_BGRA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
    // We must handle this because of internal format parameter
    else if(format == GL_RGB || format == GL_BGR)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
    
    glGenSamplers(1, &uiSampler);
}

// Creates texture from provided data.
// a_sPath - path to the texture
// format - format of data
// bGenerateMipMaps - whether to create mipmaps
void CTexture::CreateFromData(GLbyte* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps) {
    // Generate an OpenGL texture ID for this texture
    glGenTextures(1, &uiTexture);
    glBindTexture(GL_TEXTURE_2D, uiTexture);
    if(format == GL_RGBA || format == GL_BGRA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
    // We must handle this because of internal format parameter
    else if(format == GL_RGB || format == GL_BGR)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
    if(bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
    glGenSamplers(1, &uiSampler);
    
    sPath = "";
    bMipMapsGenerated = bGenerateMipMaps;
    iWidth = a_iWidth;
    iHeight = a_iHeight;
    iBPP = a_iBPP;
}

// Loads texture from a file, supports most graphics formats.
// a_sPath - path to the texture
// bGenerateMipMaps - whether to create mipmaps
bool CTexture::LoadTexture2D(std::string a_sPath, bool bGenerateMipMaps) {
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* dib(0);
    
    fif = FreeImage_GetFileType(a_sPath.c_str(), 0); // Check the file signature and deduce its format
    
    if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
        fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());
    
    if(fif == FIF_UNKNOWN) {// If still unknown, return failure
        printf("LoadTexture2D: unknown type!\n");
        return false;
    }
    
    if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
        dib = FreeImage_Load(fif, a_sPath.c_str());
    if(!dib) {
        printf("LoadTexture2D: type is not supported!\n");
        return false;
    }
    
    GLbyte* bDataPointer = (GLbyte*)FreeImage_GetBits(dib); // Retrieve the image data
    
    // If somehow one of these failed (they shouldn't), return failure
    if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0) {
        printf("LoadTexture2D: unknown error!\n");
        return false;
    }
    
    GLenum format = GL_RED;
//    int bada = FreeImage_GetBPP(dib);
    if(FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
    if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
//    if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;
    CreateFromData(bDataPointer, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, bGenerateMipMaps);
    
    FreeImage_Unload(dib);
    
    sPath = a_sPath;
    
    return true; // Success
}

void CTexture::SetSamplerParameter(GLenum parameter, GLenum value) {
    glSamplerParameteri(uiSampler, parameter, value);
}

// Sets magnification and minification texture filter.
// tfMagnification - mag. filter, must be from ETextureFiltering enum
// tfMinification - min.filter, must be from  ETextureFiltering enum
void CTexture::SetFiltering(int a_tfMagnification, int a_tfMinification) {
    glBindSampler(0, uiSampler);
    
    // Set magnification filter
    if(a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
        glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    else if(a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
        glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Set minification filter
    if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
        glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
        glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    else if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
        glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
        glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    else if(a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
        glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    tfMinification = a_tfMinification;
    tfMagnification = a_tfMagnification;
}

// Binds texture.
// iTextureUnit - texture unit to bind texture to
void CTexture::BindTexture(int iTextureUnit) {
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, uiTexture);
    glBindSampler(iTextureUnit, uiSampler);
}

// Frees all memory used by texture.
void CTexture::DeleteTexture() {
    glDeleteSamplers(1, &uiSampler);
    glDeleteTextures(1, &uiTexture);
}

int CTexture::GetMinificationFilter() {
    return tfMinification;
}

int CTexture::GetMagnificationFilter() {
    return tfMagnification;
}

int CTexture::GetWidth() {
    return iWidth;
}

int CTexture::GetHeight() {
    return iHeight;
}

int CTexture::GetBPP() {
    return iBPP;
}

GLuint CTexture::GetTextureID() {
    return uiTexture;
}

string CTexture::GetPath() {
    return sPath;
}
