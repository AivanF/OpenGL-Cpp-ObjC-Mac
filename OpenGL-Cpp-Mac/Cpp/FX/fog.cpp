#include "fog.h"
#include "helper.hpp"
#include "../../glm/gtc/type_ptr.hpp"

CFog::CFog() {
    vFogColor = glm::vec4(0.5f, 0.55f, 0.6f, 1.0f);
    fStart = 2;
    fEnd = 32;
    fDensity = 0.05;
    iEquation = 3;
}

CFog::CFog(glm::vec4 a_vFogColor, float a_fStart, float a_fEnd, float a_fDensity, float a_iEquation) {
    vFogColor = a_vFogColor;
	fStart = a_fStart;
    fEnd = a_fEnd;
    fDensity = a_fDensity;
    iEquation = a_iEquation;
}

void CFog::SetUniformData(ShaderProgram *shp, std::string sFogVarName) {
    glUniform4fv(shp->uniform((sFogVarName+".vFogColor").data()), 1, glm::value_ptr(vFogColor));
    GetError();
    glUniform1f(shp->uniform((sFogVarName+".fStart").data()), fStart);
    GetError();
    glUniform1f(shp->uniform((sFogVarName+".fEnd").data()), fEnd);
    GetError();
    glUniform1f(shp->uniform((sFogVarName+".fDensity").data()), fDensity);
    GetError();
    glUniform1i(shp->uniform((sFogVarName+".iEquation").data()), iEquation);
    GetError();
}

