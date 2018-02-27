#include "spotLight.h"
#include "../../glm/gtc/type_ptr.hpp"

CSpotLight::CSpotLight() {
	vColor = glm::vec3(1.0f, 1.0f, 1.0f);
	vPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	vDirection = glm::vec3(0.0f, 0.0f, -1.0f);

	fConeAngle = 20.0f;
	fLinearAtt = 0.02f;
    
    active = true;
}

CSpotLight::CSpotLight(glm::vec3 a_vColor, glm::vec3 a_vPosition, glm::vec3 a_vDirection, float a_fConeAngle, float a_fLinearAtt) {
	vColor = a_vColor;
	vPosition = a_vPosition;
	vDirection = a_vDirection;
	
	fConeAngle = a_fConeAngle;
	fLinearAtt = a_fLinearAtt;
    
    active = true;
}

void CSpotLight::SetUniformData(ShaderProgram *shp, std::string sLightVarName) {
    glUniform3fv(shp->uniform((sLightVarName+".vColor").data()), 1, glm::value_ptr(vColor));
    glUniform3fv(shp->uniform((sLightVarName+".vPosition").data()), 1, glm::value_ptr(vPosition));
    glUniform3fv(shp->uniform((sLightVarName+".vDirection").data()), 1, glm::value_ptr(vDirection));
    
    glUniform1f(shp->uniform((sLightVarName+".fConeAngle").data()), fConeAngle);
    glUniform1f(shp->uniform((sLightVarName+".fConeCosine").data()), float(cos(fConeAngle*3.1415f/180.0f)));
    glUniform1f(shp->uniform((sLightVarName+".fLinearAtt").data()), fLinearAtt);
}

