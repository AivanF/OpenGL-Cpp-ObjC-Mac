//
//  ParticleSystem.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 07/02/2018.
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include "ShaderProgram.hpp"
#include "../../glm/glm.hpp"


#define NUM_PARTICLE_ATTRIBUTES 6
#define MAX_PARTICLES_ON_SCENE 100000

#define PARTICLE_TYPE_GENERATOR 0
#define PARTICLE_TYPE_NORMAL 1


class CParticle {
public:
    glm::vec3 vPosition;
    glm::vec3 vVelocity;
    glm::vec3 vColor;
    float fLifeTime;
    float fSize;
    int iType;
};


class CParticleSystemTransformFeedback {
public:
    void RenderParticles(glm::vec3 cameraPos, float dn, float dx);
    void UpdateParticles(float fTimePassed);
    
    void SetGeneratorProperties(glm::vec3 a_vGenPosition, glm::vec3 a_vGenVelocityMin, glm::vec3 a_vGenVelocityMax, glm::vec3 a_vGenGravityVector, glm::vec3 a_vGenColor, float a_fGenLifeMin, float a_fGenLifeMax, float a_fGenSize, float fEvery, int a_iNumToGenerate);
    
    void ClearAllParticles();
    bool ReleaseParticleSystem();
    
    int GetNumParticles();
    
    void SetMatrices(glm::mat4* a_matProjection, glm::mat4* a_matView, glm::vec3 dir);
    
    CParticleSystemTransformFeedback();
    
private:
    bool bInitialized;
    
    GLuint uiTransformFeedbackBuffer;
    
    GLuint uiParticleBuffer[2];
    GLuint uiVAO[2];
    
    GLuint uiQuery;
    GLuint uiTexture;
    
    int iCurReadBuffer;
    int iNumParticles;
    
    glm::mat4 matProjection, matView;
    glm::vec3 vQuad1, vQuad2;
    
    float fElapsedTime;
    float fNextGenerationTime;
    
    glm::vec3 vGenPosition;
    glm::vec3 vGenVelocityMin, vGenVelocityRange;
    glm::vec3 vGenGravityVector;
    glm::vec3 vGenColor;
    
    float fGenLifeMin, fGenLifeRange;
    float fGenSize;
    
    int iNumToGenerate;
    
    static ShaderProgram *shpRenderParticles;
    static ShaderProgram *shpUpdateParticles;
    static bool shadered;
};

#endif /* ParticleSystem_hpp */
