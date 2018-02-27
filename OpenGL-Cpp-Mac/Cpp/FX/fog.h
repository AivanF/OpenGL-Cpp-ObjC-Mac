#pragma once

#include "ShaderProgram.hpp"
#include "../../glm/glm.hpp"
#include <string>

// Support class for adding point lights to scene.
class CFog {
public:
    glm::vec4 vFogColor; // Fog color
    float fStart; // This is only for linear fog
    float fEnd; // This is only for linear fog
    float fDensity; // For exp and exp2 equation
    
    int iEquation; // 0 = linear, 1 = exp, 2 = exp2, 3 = none

	void SetUniformData(ShaderProgram *shp, std::string sFogVarName);

	CFog();
	CFog(glm::vec4 a_vFogColor, float a_fStart, float a_fEnd, float a_fDensity, float a_iEquation);
};
