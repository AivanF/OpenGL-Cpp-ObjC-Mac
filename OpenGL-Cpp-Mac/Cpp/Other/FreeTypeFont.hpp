//
//  FreeTypeFont.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 07/02/2018.
//

#ifndef FreeTypeFont_hpp
#define FreeTypeFont_hpp

#include "Texture.hpp"
#include "ShaderProgram.hpp"

#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H


class CFreeTypeFont {
private:
    void CreateChar(int iIndex);
    
    CTexture tCharTextures[256];
    int iAdvX[256], iAdvY[256];
    int iBearingX[256], iBearingY[256];
    int iCharWidth[256], iCharHeight[256];
    int iLoadedPixelSize, iNewLine;
    
    bool bLoaded;
    
    GLuint uiVAO;
    GLuint uiVBO;
    vector<char> data;
    
    FT_Library ftLib;
    FT_Face ftFace;
    ShaderProgram* shp;
    
public:
    bool LoadFont(string sFile, int iPXSize);
    
    int GetTextWidth(string sText, int iPXSize);
    
    void Print(string sText, int x, int y, int iPXSize = -1);
    void PrintFormatted(int x, int y, int iPXSize, char* sText, ...);
    
    void DeleteFont();
    
    void SetShaderProgram(ShaderProgram* ashp);
    
    CFreeTypeFont();
};

#endif /* FreeTypeFont_hpp */
