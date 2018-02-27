#pragma once

#include "ShaderProgram.hpp"
#include "../../glm/glm.hpp"
#include <string>

// Support class for adding spotlights to scene.
class CSpotLight {
public:
	glm::vec3 vColor;
	glm::vec3 vPosition;
	glm::vec3 vDirection;

	float fConeAngle;
    float fLinearAtt;
    
    bool active;

	void SetUniformData(ShaderProgram *shp, std::string sLightVarName);

	CSpotLight();
	CSpotLight(glm::vec3 a_vColor, glm::vec3 a_vPosition, glm::vec3 a_vDirection, float a_fConeAngle, float a_fLinearAtt);
private:
	// This shouldn't be changed from outside
	float fConeCosine;
};
