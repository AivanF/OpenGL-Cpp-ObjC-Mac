#version 410

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vEyeSpacePos;
smooth in vec3 vWorldPos;
out vec4 outputColor;

uniform float screenWidth;
uniform float screenHeight;
uniform float time;
uniform vec3 vCameraPos;
uniform sampler2D gSamplers[3];
uniform float fTextureContributions[2];
uniform vec4 vColor;
uniform int numTextures;
uniform float useLights;


// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
// https://thebookofshaders.com/11/
// https://thebookofshaders.com/13/

vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
float noise (vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);
    
    // Four corners in 2D of a tile
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

// Fractional Brownian motion
#define NUM_OCTAVES 5
float fbm(vec2 x) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(x);
        x = rot * x * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

// Simplex Noise
float snoise(vec2 v){
    const vec4 C = vec4(0.211324865405187, 0.366025403784439,
                        -0.577350269189626, 0.024390243902439);
    vec2 i  = floor(v + dot(v, C.yy) );
    vec2 x0 = v -   i + dot(i, C.xx);
    vec2 i1;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod(i, 289.0);
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
                     + i.x + vec3(0.0, i1.x, 1.0 ));
    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
                            dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

struct FogParameters {
    vec4 vFogColor; // Fog color
    float fStart; // This is only for linear fog
    float fEnd; // This is only for linear fog
    float fDensity; // For exp and exp2 equation
    
    int iEquation; // 0 = linear, 1 = exp, 2 = exp2
};

uniform FogParameters fogParams[1];

float getFogFactor(FogParameters params, float fFogCoord) {
    float fResult = 0.0;
    
    if (params.iEquation == 3) {
        return fResult;  
    } else if (params.iEquation == 0) {
        fResult = (params.fEnd-fFogCoord)/(params.fEnd-params.fStart);
    } else if(params.iEquation == 1) {
        fResult = exp(-params.fDensity*fFogCoord);
    } else if(params.iEquation == 2) {
        fResult = exp(-pow(params.fDensity*fFogCoord, 2.0));
    }
    
    fResult = 1.0-clamp(fResult, 0.0, 1.0);
    
    return fResult;
}

struct DirectionalLight {
    vec3 vColor;
    vec3 vDirection;
    
    float fAmbient;
};

vec4 getDirectionalLightColor(DirectionalLight dirLight, vec3 vNormal) {
    float fDiffuseIntensity = max(0.0, dot(vNormal, -dirLight.vDirection));
    float fMult = clamp(dirLight.fAmbient+fDiffuseIntensity, 0.0, 1.0);
    return vec4(dirLight.vColor*fMult, 1.0);
}

struct PointLight {
    vec3 vColor; // Color of that point light
    vec3 vPosition;
    
    float fAmbient;
    
    float fConstantAtt;
    float fLinearAtt;
    float fExpAtt;
};

vec4 getPointLightColor(const PointLight ptLight, vec3 vWorldPos, vec3 vNormal) {
    vec3 vPosToLight = vWorldPos-ptLight.vPosition;
    float fDist = length(vPosToLight);
    vPosToLight = normalize(vPosToLight);
    
    float fDiffuse = max(0.0, dot(vNormal, -vPosToLight));
    
    float fAttTotal = ptLight.fConstantAtt + ptLight.fLinearAtt*fDist + ptLight.fExpAtt*fDist*fDist;
    
    return vec4(ptLight.vColor, 1.0)*(ptLight.fAmbient+fDiffuse)/fAttTotal;
}

struct SpotLight {
    vec3 vColor;
    vec3 vPosition;
    vec3 vDirection;
    
    // If cone angle is set from program, we also provide its cosine,
    // so that it isn't always recalculated
    float fConeAngle, fConeCosine;
    
    // Only linear attenuation, you can code const and exp as well
    float fLinearAtt;
};

vec4 getSpotLightColor(const SpotLight spotLight, vec3 vWorldPos) {
    // Distance from fragment's position
    float fDistance = distance(vWorldPos, spotLight.vPosition);
    
    // Get direction vector to fragment
    vec3 vDir = vWorldPos-spotLight.vPosition;
    vDir = normalize(vDir);
    
    // Cosine between spotlight direction and directional vector to fragment
    float fCosine = dot(spotLight.vDirection, vDir);
    
    // Difference between max cosine and current cosine
    float fDif = 1.0-spotLight.fConeCosine;
    
    // This is how strong light is depending whether its nearer to the center of
    // cone or nearer to its borders (onway factor in article), clamp to 0.0 and 1.0
    float fFactor = clamp((fCosine-spotLight.fConeCosine)/fDif, 0.0, 1.0);
    
    // If we're inside cone, calculate color
    if (fCosine > spotLight.fConeCosine)
        return vec4(spotLight.vColor, 1.0)*fFactor/(fDistance*spotLight.fLinearAtt);
    
    // No color otherwise
    return vec4(0.0, 0.0, 0.0, 0.0);
}

#define MAX_SPOT 8
#define MAX_POINT 16

uniform DirectionalLight sunLight[1];
uniform SpotLight spotLights[MAX_SPOT];
uniform PointLight pointLights[MAX_POINT];
uniform int countPoint;
uniform int countSpot;
uniform int useNormalMap;


mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv ) {
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
    
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
    
    // construct a scale-invariant frame
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord, vec3 map ) {
    // assume N, the interpolated vertex normal and
    // V, the view vector (vertex to eye)
//    map.z = sqrt( 1. - dot( map.xy, map.xy ) ); // If 2 CHANNEL
    mat3 TBN = cotangent_frame( N, -V, texcoord );
    return normalize( TBN * map );
}


//vec3 rotateVector(vec3 target, vec3 basis) {
//    vec3 new_y = normalize(basis);
//    vec3 new_z = normalize(cross(new_y, vec3(0, 1, 0)));
//    vec3 new_x = normalize(cross(new_y, new_z));
//    return mat3(new_x, new_y, new_z) * target;
//}


void main() {
    float x = gl_FragCoord.x/screenWidth;
    float y = gl_FragCoord.y/screenHeight;
    float z = gl_FragCoord.z;
    
    /// For tests
//    outputColor = vec4(vTexCoord, 1.0, 1.0);
    
    /// Calculate color by texture
    vec4 vTexColor1 = texture(gSamplers[0], vTexCoord);
    vec4 vTexColor2 = texture(gSamplers[1], vTexCoord);
    vec4 vMixedColor = vTexColor1 * fTextureContributions[0];
    if (numTextures > 1)
        vMixedColor += vTexColor2 * fTextureContributions[1];
    outputColor = vMixedColor;
    
    /// Add lights
    vec3 dist = vCameraPos-vWorldPos;
    vec3 vModelNormal = normalize(vNormal);
    vec3 vRealNormal;
//    vRealNormal = rotateVector(vMapNormal, vNormal);
    if (useNormalMap == 1) {
        vec3 vMapNormal = 2 * texture(gSamplers[2], vTexCoord).xyz - 1;
        vRealNormal = perturb_normal( vModelNormal, normalize(dist), vTexCoord, vMapNormal );
    } else {
        vRealNormal = vModelNormal;
    }
    
    vec4 vDirLightColor = max(vec4(0,0,0,0), getDirectionalLightColor(sunLight[0], vRealNormal));
    vec4 vSpotlightColor = vec4(0,0,0,1);
    vec4 vPointlightColor = vec4(0,0,0,1);
    int i;
    for (i = 0; i < MAX_SPOT; i++) {
        if (i < countSpot) {
            vSpotlightColor += max(vec4(0,0,0,0), getSpotLightColor(spotLights[i], vWorldPos));
        }
    }
    for (i = 0; i < MAX_POINT; i++) {
        if (i < countPoint) {
            vPointlightColor += max(vec4(0,0,0,0), getPointLightColor(pointLights[i], vWorldPos, vRealNormal));
        }
    }
    /// Light may be added partially
    // make light not negative
    vec4 u = useLights * (vDirLightColor + vSpotlightColor + vPointlightColor);
    vec4 n = (1 - useLights) * vec4(1,1,1,1);
    vMixedColor = vMixedColor * (u + n);
    outputColor = vMixedColor;
//    outputColor = vec4(sunLight[0].vColor, 1.0);
    
    float no1 = fbm(vec2(x * 15 + dist.y + time * 7, y * 15 + dist.x + time * 11));
    float no2 = fbm(vec2(x * 13 + dist.x + time * 11, y * 13 + dist.y + time * 7));
    float no = 0.3 + 0.7 * fbm(vec2(no1, no2));
//    no = 1;
//    float no = 0.3 + 0.7 * fbm(vec2( no2,fbm(vec2(no1, no2)) ));
//    outputColor = vec4(no1, no2, 0, 1);
    
    float fFogCoord = distance(-vCameraPos, vWorldPos) * no;
    outputColor = mix(outputColor, fogParams[0].vFogColor, getFogFactor(fogParams[0], fFogCoord));
//    outputColor = vec4(x, y, z, 1);
}


