#include "pointLight.h"
#include "../../glm/gtc/type_ptr.hpp"

CPointLight::CPointLight() {
	vPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	vColor = glm::vec3(1.0f, 1.0f, 1.0f);

	fAmbient = 0.1f;

	fConstantAtt = 0.3f;
	fLinearAtt = 0.007f;
    fExpAtt = 0.00008f;
    
    active = true;
}

CPointLight::CPointLight(glm::vec3 a_vColor, glm::vec3 a_vPosition, float a_fAmbient, float a_fConstantAtt, float a_fLinearAtt, float a_fExpAtt) {
	vColor = a_vColor;
	vPosition = a_vPosition;

	fAmbient = a_fAmbient;
	
	fConstantAtt = a_fConstantAtt;
	fLinearAtt = a_fLinearAtt;
    fExpAtt = a_fExpAtt;
    
    active = true;
}

void CPointLight::SetUniformData(ShaderProgram *shp, std::string sLightVarName) {
    glUniform3fv(shp->uniform((sLightVarName+".vColor").data()), 1, glm::value_ptr(vColor));
    glUniform3fv(shp->uniform((sLightVarName+".vPosition").data()), 1, glm::value_ptr(vPosition));
    
    glUniform1f(shp->uniform((sLightVarName+".fAmbient").data()), fAmbient);
    glUniform1f(shp->uniform((sLightVarName+".fConstantAtt").data()), fConstantAtt);
    glUniform1f(shp->uniform((sLightVarName+".fLinearAtt").data()), fLinearAtt);
    glUniform1f(shp->uniform((sLightVarName+".fExpAtt").data()), fExpAtt);
}

