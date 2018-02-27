//
//  ParticleSystem.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 07/02/2018.
//

#include "ParticleSystem.hpp"
#include "helper.hpp"
#include "shaders.hpp"

#include "../../glm/glm.hpp"
#include "../../glm/gtc/type_ptr.hpp"
#include "../../glm/gtc/matrix_transform.hpp"


ShaderProgram *CParticleSystemTransformFeedback::shpRenderParticles;
ShaderProgram *CParticleSystemTransformFeedback::shpUpdateParticles;
bool CParticleSystemTransformFeedback::shadered = false;


CParticleSystemTransformFeedback::CParticleSystemTransformFeedback() {
    if (!shadered) {
        shadered = true;
        
        const char* sVaryings[NUM_PARTICLE_ATTRIBUTES] = {
            "vPositionOut",
            "vVelocityOut",
            "vColorOut",
            "fLifeTimeOut",
            "fSizeOut",
            "iTypeOut",
        };
        
        // Updating program
        
        GLuint sh_puv = makeShaderFromFile(GL_VERTEX_SHADER, "particles_update", "vert");
        GLuint sh_pug = makeShaderFromFile(GL_GEOMETRY_SHADER, "particles_update", "geom");
        
        shpUpdateParticles = new ShaderProgram();
        shpUpdateParticles->addShader(sh_puv);
        shpUpdateParticles->addShader(sh_pug);
        FOR(i, NUM_PARTICLE_ATTRIBUTES)
        glTransformFeedbackVaryings(shpUpdateParticles->index(), 6, sVaryings, GL_INTERLEAVED_ATTRIBS);
        shpUpdateParticles->linkProgram();
        
        // Rendering program
        
        GLuint sh_prv = makeShaderFromFile(GL_VERTEX_SHADER, "particles_render", "vert");
        GLuint sh_prg = makeShaderFromFile(GL_GEOMETRY_SHADER, "particles_render", "geom");
        GLuint sh_prf = makeShaderFromFile(GL_FRAGMENT_SHADER, "particles_render", "frag");
        
        shpRenderParticles = new ShaderProgram();
        shpRenderParticles->addShader(sh_prv);
        shpRenderParticles->addShader(sh_prg);
        shpRenderParticles->addShader(sh_prf);
        shpRenderParticles->linkProgram();
    }
    
    glGenTransformFeedbacks(1, &uiTransformFeedbackBuffer);
    glGenQueries(1, &uiQuery);
    
    glGenBuffers(2, uiParticleBuffer);
    glGenVertexArrays(2, uiVAO);
    
    CParticle partInitialization;
    partInitialization.iType = PARTICLE_TYPE_GENERATOR;
    
    FOR(i, 2) {
        glBindVertexArray(uiVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, uiParticleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(CParticle)*MAX_PARTICLES_ON_SCENE, NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(CParticle), &partInitialization);
        
        FOR(i, NUM_PARTICLE_ATTRIBUTES)glEnableVertexAttribArray(i);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)0); // Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)12); // Velocity
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)24); // Color
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)36); // Lifetime
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)40); // Size
        glVertexAttribPointer(5, 1, GL_INT,	  GL_FALSE, sizeof(CParticle), (const GLvoid*)44); // Type
    }
    
    bInitialized = false;
    iCurReadBuffer = 0;
    iNumParticles = 1;
}

/*
 
 Name:	UpdateParticles
 
 Params:	fTimePassed - time passed since last frame
 
 Result:	Performs particle updating on GPU.
 
 */

float grandf(float fMin, float fAdd) {
    float fRandom = float( rand()%(RAND_MAX) )/float(RAND_MAX);
    return fMin+fAdd*fRandom;
}

void CParticleSystemTransformFeedback::UpdateParticles(float fTimePassed) {
    if(!bInitialized)return;
    
    shpUpdateParticles->use();
    
    glm::vec3 vUpload;
    glUniform1f(shpUpdateParticles->uniform("fTimePassed"), fTimePassed);
    glUniform3fv(shpUpdateParticles->uniform("vGenPosition"), 1, glm::value_ptr(vGenPosition));
    glUniform3fv(shpUpdateParticles->uniform("vGenVelocityMin"), 1, glm::value_ptr(vGenVelocityMin));
    glUniform3fv(shpUpdateParticles->uniform("vGenVelocityRange"), 1, glm::value_ptr(vGenVelocityRange));
    glUniform3fv(shpUpdateParticles->uniform("vGenColor"), 1, glm::value_ptr(vGenColor));
    glUniform3fv(shpUpdateParticles->uniform("vGenGravityVector"), 1, glm::value_ptr(vGenGravityVector));
    
    glUniform1f(shpUpdateParticles->uniform("fGenLifeMin"), fGenLifeMin);
    glUniform1f(shpUpdateParticles->uniform("fGenLifeRange"), fGenLifeRange);
    glUniform1f(shpUpdateParticles->uniform("fGenSize"), fGenSize);
    glUniform1i(shpUpdateParticles->uniform("iNumToGenerate"), 0);
    
    fElapsedTime += fTimePassed;
    
    if(fElapsedTime > fNextGenerationTime) {
        glUniform1i(shpUpdateParticles->uniform("iNumToGenerate"), iNumToGenerate);
        fElapsedTime -= fNextGenerationTime;
        
        glm::vec3 vRandomSeed = glm::vec3(grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f));
        glUniform3fv(shpUpdateParticles->uniform("vRandomSeed"), 1, glm::value_ptr(vRandomSeed));
    }
    
    glEnable(GL_RASTERIZER_DISCARD);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, uiTransformFeedbackBuffer);
    
    glBindVertexArray(uiVAO[iCurReadBuffer]);
    glEnableVertexAttribArray(1); // Re-enable velocity
    
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, uiParticleBuffer[1-iCurReadBuffer]);
    
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, uiQuery);
    glBeginTransformFeedback(GL_POINTS);
    
    glDrawArrays(GL_POINTS, 0, iNumParticles);
    
    glEndTransformFeedback();
    
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    glGetQueryObjectiv(uiQuery, GL_QUERY_RESULT, &iNumParticles);
    
    iCurReadBuffer = 1-iCurReadBuffer;
    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

/*
 
 Name:	RenderParticles
 
 Params:	none
 
 Result:	Performs particle rendering on GPU.
 
 */

void CParticleSystemTransformFeedback::RenderParticles(glm::vec3 cameraPos, float dn, float dx) {
    if(!bInitialized)return;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(0);
    
    glDisable(GL_RASTERIZER_DISCARD);
    shpRenderParticles->use();
    
    glUniform3fv(shpRenderParticles->uniform("vCameraPos"), 1, glm::value_ptr(cameraPos));
    glUniform1f(shpRenderParticles->uniform("dstmin"), dn);
    glUniform1f(shpRenderParticles->uniform("dstmax"), dx);
    
    glUniformMatrix4fv(shpRenderParticles->uniform("matrices.mProj"), 1, GL_FALSE, glm::value_ptr(matProjection));
    glUniformMatrix4fv(shpRenderParticles->uniform("matrices.mView"), 1, GL_FALSE, glm::value_ptr(matView));
    glUniform3fv(shpRenderParticles->uniform("vQuad1"), 1, glm::value_ptr(vQuad1));
    glUniform3fv(shpRenderParticles->uniform("vQuad2"), 1, glm::value_ptr(vQuad2));
    glUniform1i(shpRenderParticles->uniform("gSampler"), 0);
    
    glBindVertexArray(uiVAO[iCurReadBuffer]);
    glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering
    
    glDrawArrays(GL_POINTS, 0, iNumParticles);
    
    glDepthMask(1);
    glDisable(GL_BLEND);
}

/*
 
 Name:	SetMatrices
 
 Params:	a_matProjection - projection matrix
 vEye, vView, vUpVector - definition of view matrix
 
 Result:	Sets the projection matrix and view matrix,
 that shaders of transform feedback particle system
 need.
 
 */

void CParticleSystemTransformFeedback::SetMatrices(glm::mat4* a_matProjection, glm::mat4* a_matView, glm::vec3 dir) {
    matProjection = *a_matProjection;
    
    matView = *a_matView;
    
    vQuad1 = glm::cross(dir, glm::vec3(0, 1, 0));
    vQuad1 = glm::normalize(vQuad1);
    vQuad2 = glm::cross(dir, vQuad1);
    vQuad2 = glm::normalize(vQuad2);
}

/*
 
 Name:	SetGeneratorProperties
 
 Params:	many properties of particle generation
 
 Result:	Guess what it does :)
 
 */

void CParticleSystemTransformFeedback::SetGeneratorProperties(glm::vec3 a_vGenPosition,
                                                              glm::vec3 a_vGenVelocityMin,
                                                              glm::vec3 a_vGenVelocityMax,
                                                              glm::vec3 a_vGenGravityVector,
                                                              glm::vec3 a_vGenColor,
                                                              float a_fGenLifeMin,
                                                              float a_fGenLifeMax,
                                                              float a_fGenSize,
                                                              float fEvery,
                                                              int a_iNumToGenerate) {
    bInitialized = true;
    
    vGenPosition = a_vGenPosition;
    vGenVelocityMin = a_vGenVelocityMin;
    vGenVelocityRange = a_vGenVelocityMax - a_vGenVelocityMin;
    
    vGenGravityVector = a_vGenGravityVector;
    vGenColor = a_vGenColor;
    fGenSize = a_fGenSize;
    
    fGenLifeMin = a_fGenLifeMin;
    fGenLifeRange = a_fGenLifeMax - a_fGenLifeMin;
    
    fNextGenerationTime = fEvery;
    fElapsedTime = 0.8f;
    
    iNumToGenerate = a_iNumToGenerate;
}

/*
 
 Name:	GetNumParticles
 
 Params:	none
 
 Result:	Retrieves current number of particles.
 
 */

int CParticleSystemTransformFeedback::GetNumParticles() {
    return iNumParticles;
}
