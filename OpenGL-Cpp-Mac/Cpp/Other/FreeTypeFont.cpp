//
//  FreeTypeFont.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 07/02/2018.
//

#include "FreeTypeFont.hpp"
#include "helper.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"

CFreeTypeFont::CFreeTypeFont() {
    bLoaded = false;
}

/*
 
 Name:	CreateChar
 
 Params:	iIndex - character index in Unicode.
 
 Result:	Creates one single character (its texture).
 
 */

inline int next_p2(int n){int res = 1; while(res < n)res <<= 1; return res;}

void CFreeTypeFont::CreateChar(int iIndex) {
    FT_Load_Glyph(ftFace, FT_Get_Char_Index(ftFace, iIndex), FT_LOAD_DEFAULT);
    
    FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);
    FT_Bitmap* pBitmap = &ftFace->glyph->bitmap;
    
    int iW = pBitmap->width, iH = pBitmap->rows;
    int iTW = next_p2(iW), iTH = next_p2(iH);
    
    GLbyte* bData = new GLbyte[iTW*iTH];
    // Copy glyph data and add dark pixels elsewhere
    FOR(ch, iTH)FOR(cw, iTW)
    bData[ch*iTW+cw] = (ch >= iH || cw >= iW) ? 0 : pBitmap->buffer[(iH-ch-1)*iW+cw];
    
    // And create a texture from it
    
    tCharTextures[iIndex].CreateFromData(bData, iTW, iTH, 8, GL_DEPTH_COMPONENT, false);
    tCharTextures[iIndex].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
    
    tCharTextures[iIndex].SetSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    tCharTextures[iIndex].SetSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Calculate glyph data
    iAdvX[iIndex] = (int)ftFace->glyph->advance.x>>6;
    iBearingX[iIndex] = (int)ftFace->glyph->metrics.horiBearingX>>6;
    iCharWidth[iIndex] = (int)ftFace->glyph->metrics.width>>6;
    
    iAdvY[iIndex] = (int)(ftFace->glyph->metrics.height - ftFace->glyph->metrics.horiBearingY)>>6;
    iBearingY[iIndex] = (int)ftFace->glyph->metrics.horiBearingY>>6;
    iCharHeight[iIndex] = (int)ftFace->glyph->metrics.height>>6;
    
    iNewLine = max(iNewLine, int(ftFace->glyph->metrics.height>>6));
    
    // Rendering data, texture coordinates are always the same, so now we waste a little memory
    glm::vec2 vQuad[] =
    {
        glm::vec2(0.0f, float(-iAdvY[iIndex]+iTH)),
        glm::vec2(0.0f, float(-iAdvY[iIndex])),
        glm::vec2(float(iTW), float(-iAdvY[iIndex]+iTH)),
        glm::vec2(float(iTW), float(-iAdvY[iIndex]))
    };
    glm::vec2 vTexQuad[] = {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)};
    
    // Add this char to VBO
    FOR(i, 4) {
        data.insert(data.end(), (char*)&vQuad[i], (char*)&vQuad[i]+sizeof(glm::vec2));
        data.insert(data.end(), (char*)&vTexQuad[i], (char*)&vTexQuad[i]+sizeof(glm::vec2));
    }
    delete[] bData;
}

/*
 
 Name:	LoadFont
 
 Params:	sFile - path to font file
 iPXSize - desired font pixel size
 
 Result:	Loads whole font.
 
 */

bool CFreeTypeFont::LoadFont(string sFile, int iPXSize) {
    bool bError = FT_Init_FreeType(&ftLib);
    bError = FT_New_Face(ftLib, sFile.c_str(), 0, &ftFace);
    if (bError) {
        printf("Font loading error!\n");
        assert(0);
        return false;
    }
    FT_Set_Pixel_Sizes(ftFace, iPXSize, iPXSize);
    iLoadedPixelSize = iPXSize;
    
    glGenVertexArrays(1, &uiVAO);
    glBindVertexArray(uiVAO);
    glGenBuffers(1, &uiVBO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    
    FOR(i, 128)CreateChar(i);
    bLoaded = true;
    
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);
    
    glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2)*2, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2)*2, (void*)(sizeof(glm::vec2)));
    return true;
}

/*
 
 Name:	GetTextWidth
 
 Params:	sText - text to get width of
 iPXSize - it's printed size
 
 Result:	Returns width as number of pixels the
 text will occupy.
 
 */

int CFreeTypeFont::GetTextWidth(string sText, int iPXSize) {
    int iResult = 0;
    FOR(i, ESZ(sText))iResult += iAdvX[sText[i]];
    
    return iResult*iPXSize/iLoadedPixelSize;
}

/*
 
 Name:	Print
 
 Params:	sText - text to print
 x, y - 2D position
 iPXSize - printed text size
 
 Result:	Prints text at specified position
 with specified pixel size.
 
 */

void CFreeTypeFont::Print(string sText, int x, int y, int iPXSize) {
    if(!bLoaded)return;
    
    glBindVertexArray(uiVAO);
    glUniform1i(shp->uniform("gSampler"), 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int iCurX = x, iCurY = y;
    if(iPXSize == -1)iPXSize = iLoadedPixelSize;
    float fScale = float(iPXSize)/float(iLoadedPixelSize);
    FOR(i, ESZ(sText)) {
        if(sText[i] == '\n') {
            iCurX = x;
            iCurY -= iNewLine*iPXSize/iLoadedPixelSize;
            continue;
        }
        int iIndex = int(sText[i]);
        iCurX += iBearingX[iIndex]*iPXSize/iLoadedPixelSize;
        if(sText[i] != ' ') {
            tCharTextures[iIndex].BindTexture(0);
            glm::mat4 mModelView = glm::translate(glm::mat4(1.0f), glm::vec3(float(iCurX), float(iCurY), 0.0f));
            mModelView = glm::scale(mModelView, glm::vec3(fScale));
            
            glUniformMatrix4fv(shp->uniform("matrices.modelViewMatrix"),
                               1, GL_FALSE, glm::value_ptr(mModelView));
            
            // Draw character
            glDrawArrays(GL_TRIANGLE_STRIP, iIndex*4, 4);
        }
        
        iCurX += (iAdvX[iIndex]-iBearingX[iIndex])*iPXSize/iLoadedPixelSize;
    }
    glDisable(GL_BLEND);
}

/*
 
 Name:	PrintFormatted
 
 Params:	x, y - 2D position
 iPXSize - printed text size
 sText - text to print
 
 Result:	Prints formatted text at specified position
 with specified pixel size.
 
 */

void CFreeTypeFont::PrintFormatted(int x, int y, int iPXSize, char* sText, ...) {
    char buf[512];
    va_list ap;
    va_start(ap, sText);
    vsprintf(buf, sText, ap);
    va_end(ap);
    Print(buf, x, y, iPXSize);
}

/*
 
 Name:	DeleteFont
 
 Params:	none
 
 Result:	Deletes all font textures.
 
 */

void CFreeTypeFont::DeleteFont() {
    FOR(i, 128)tCharTextures[i].DeleteTexture();
    data.clear();
    glDeleteBuffers(1, &uiVBO);
    glDeleteVertexArrays(1, &uiVAO);
}

/*
 
 Name:	SetShaderProgram
 
 Params:	a_shShaderProgram - shader program
 
 Result:	Sets shader program that font uses.
 
 */

void CFreeTypeFont::SetShaderProgram(ShaderProgram* ashp) {
    shp = ashp;
}
