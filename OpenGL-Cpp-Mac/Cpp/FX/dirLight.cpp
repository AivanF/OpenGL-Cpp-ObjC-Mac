#include "dirLight.h"
#include "../../glm/gtc/type_ptr.hpp"
#include "helper.hpp"

CDirectionalLight::CDirectionalLight() {
	vColor = glm::vec3(1.0f, 1.0f, 1.0f);
	vDirection = glm::vec3(0.0f, -1.0f, 0.0f);

	fAmbient = 0.25f;
}

CDirectionalLight::CDirectionalLight(glm::vec3 a_vColor, glm::vec3 a_vDirection, float a_fAmbient) {
	vColor = a_vColor;
	vDirection = a_vDirection;

	fAmbient = a_fAmbient;
}

void CDirectionalLight::SetUniformData(ShaderProgram *shp, std::string sLightVarName) {
    // Mac OS X struct bug:
    // http://renderingpipeline.com/2012/07/macos-x-opengl-driver-bugs/
    glUniform3fv(shp->uniform((sLightVarName+".vColor").data()), 1, glm::value_ptr(vColor));
    glUniform3fv(shp->uniform((sLightVarName+".vDirection").data()), 1, glm::value_ptr(vDirection));
    glUniform1f(shp->uniform((sLightVarName+".fAmbient").data()), fAmbient);
    GetError();
}
