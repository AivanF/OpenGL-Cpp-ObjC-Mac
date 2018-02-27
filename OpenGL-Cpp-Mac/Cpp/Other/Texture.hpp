//
//  Texture.hpp
//

#ifndef Texture_hpp
#define Texture_hpp

#include <OpenGL/gl3.h>
#include <string>

using namespace std;

enum ETextureFiltering {
    TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
    TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
    TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
    TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
    TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
    TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
    TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};


// Wraps OpenGL texture object and performs their loading.
class CTexture {
    int iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
    GLuint uiTexture; // Texture name
    GLuint uiSampler; // Sampler name
    bool bMipMapsGenerated;
    
    int tfMinification, tfMagnification;
    
    string sPath;
    
public:
    CTexture();
    
    void CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format);
    void CreateFromData(GLbyte* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps = false);
    
    bool LoadTexture2D(std::string a_sPath, bool bGenerateMipMaps = false);
    void BindTexture(int iTextureUnit = 0);
    
    void SetFiltering(int a_tfMagnification, int a_tfMinification);
    
    void SetSamplerParameter(GLenum parameter, GLenum value);
    
    int GetMinificationFilter();
    int GetMagnificationFilter();
    
    int GetWidth();
    int GetHeight();
    int GetBPP();
    
    GLuint GetTextureID();
    
    string GetPath();
    
    void DeleteTexture();
};



#endif /* Texture_hpp */
