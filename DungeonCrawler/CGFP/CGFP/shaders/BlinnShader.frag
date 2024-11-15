layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
	vec3 lightPos;
	vec3 lightPos3;
	vec3 lightPos4;
	vec3 lightDir;
	vec3 lightDir2;
	vec4 lightColor;
	vec3 lightColor3;
	vec3 lightColor4;
	vec3 eyePos;
	vec4 coneInOutDecayExp;
} gubo;

layout(set = 1, binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
	mat4 mMat;
	mat4 nMat;
} ubo;

layout(set = 1, binding = 1) uniform sampler2D tex;


const float beta = 2.0f;
const float g = 1.5;
const float cosout = 0.85;
const float cosin  = 0.95;

//Calculate SpotLight light model
vec3 calculateSpotLight(vec3 lightPos,vec3 lightDir, vec3 lightColor,float g,float beta, float cosin, float cosout){
	float distance = length(lightPos - fragPos);
	float attenuation = pow((g/distance),beta);
    lightColor *= attenuation;
	float coneNumerator = dot(normalize(lightPos - fragPos),lightDir) - cosout;
	float coneDenominator = cosin - cosout;
	float cone = clamp(coneNumerator/coneDenominator,0.0f,1.0f); //Clamp is used to make the cone dimming effect
	lightColor *= cone;
	return lightColor;
}

//Calculate Blinn Specular component
vec3 calculateBlinnspecular(vec3 Norm, vec3 lightDir, vec3 EyeDir, float gamma){
	return vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)),0.0,1.0), gamma));
}

//Calculate Lambert Diffuse component
vec3 calculateLambertDiffuse(vec3 Md,vec3 Norm, vec3 lightDir){
	return Md*max(dot(lightDir,Norm),0);
}

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);
	vec3 MainCol = texture(tex, fragUV).rgb;

	vec3 lightDir = gubo.lightDir;
	vec3 lightColor = gubo.lightColor.rgb;
	vec3 lightPos = gubo.lightPos;

	vec3 lightPosGem = gubo.lightPos3;
	vec3 lightDirGem = gubo.lightDir2;
	vec3 lightColorGem = gubo.lightColor3;

	vec3 lightModel = calculateSpotLight(lightPos,lightDir,lightColor,g,beta,cosin,cosout);
	vec3 Diffuse = calculateLambertDiffuse(MainCol,Norm,lightDir) * 0.995f;
	vec3 Specular = calculateBlinnspecular(Norm,lightDir,EyeDir,160.0f);
    
	vec3 gemSpotLightCol = calculateSpotLight(lightPosGem,lightDirGem,lightColorGem,gubo.coneInOutDecayExp.z,gubo.coneInOutDecayExp.w,gubo.coneInOutDecayExp.x,gubo.coneInOutDecayExp.y);
    vec3 Diffuse2 = calculateLambertDiffuse(MainCol,Norm, lightDirGem);
	vec3 Spec2 = calculateBlinnspecular(Norm,lightDirGem,EyeDir,160.0f);
	vec3 DiffuseSpec2 = (Diffuse2 + Spec2)* gemSpotLightCol;

	vec3 Ambient = MainCol * 0.005f;
	outColor = vec4(clamp((Diffuse + Specular) * lightModel + DiffuseSpec2 + Ambient,0.0,1.0), 1.0f);
}