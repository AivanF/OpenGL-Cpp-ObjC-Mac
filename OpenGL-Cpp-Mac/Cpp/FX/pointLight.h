#pragma once

#include "ShaderProgram.hpp"
#include "../../glm/glm.hpp"
#include <string>

// Support class for adding point lights to scene.
class CPointLight {
public:
	glm::vec3 vColor; // Color of point light
	glm::vec3 vPosition; // and position

	float fAmbient;
	float fConstantAtt;
	float fLinearAtt;
	float fExpAtt;
    
    bool active;

	void SetUniformData(ShaderProgram *shp, std::string sLightVarName);

	CPointLight();
	CPointLight(glm::vec3 a_vColor, glm::vec3 a_vPosition, float a_fAmbient, float a_fConstantAtt, float a_fLinearAtt, float a_fExpAtt);
};
