layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
	vec3 lightPos;
	vec3 lightPos2;
	vec3 lightColor;
	vec3 lightColor2;
    vec3 eyePos;
    vec2 pointDecayExp;
	vec4 coneInOutDecayExp;
	vec3 lightDir;
} gubo;

layout(set = 1, binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
	mat4 mMat;
	mat4 nMat;
    vec2 isEmitting;
} ubo;

layout(set = 1, binding = 1) uniform sampler2D tex;

vec3 calculateOrenNayarDiffuse(vec3 EyeDir, vec3 Norm, vec3 lightDir, vec3 Md, float sigma){
	//vec3 EyeDir  - direction of the viewer
	//vec3 Norm  - normal vector to the surface
	//vec3 lightDir  - light vector (from the light model)
	//vec3 Md - main color of the surface
	//float sigma - Roughness of the model

	vec3 vi = normalize(lightDir - (dot(lightDir,Norm) * Norm));
	vec3 vr = normalize(EyeDir - (dot(EyeDir,Norm) * Norm));
	float G = max (0,dot(vi,vr));
	vec3 l = Md * clamp(dot(lightDir,Norm),0,1);

	float Oi = acos(dot(lightDir,Norm));
	float Or = acos(dot(EyeDir,Norm));
	float alfa = max(Oi,Or);
	float beta = min(Oi,Or);
	
	float A = 1 - 0.5 * (pow(sigma,2)/(pow(sigma,2) + 0.33));
	float B = 0.45 * (pow(sigma,2)/(pow(sigma,2) + 0.09));

	return l* (A + B * G * sin(alfa) * tan(beta));
}

//Calculate Blinn Specular component
vec3 calculateBlinnspecular(vec3 Norm, vec3 lightDir, vec3 EyeDir, float gamma){
	//No Ms since not metallic
	return vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)),0.0,1.0), gamma));
}

//Calculate Lambert Diffuse component
vec3 calculateLambertDiffuse(vec3 Md,vec3 Norm, vec3 lightDir){
	return Md*max(dot(lightDir,Norm),0);
}

//Calculate PointLight light model (Sun)
vec3 calculatePointLight(vec3 lightPos,vec3 lightColor, float g, float beta){
	//g is distance where light reduction is 1
	// beta is decay factor 0,1,2 for const,inverse lin, inverse squared
	float distance = length(fragPos - lightPos);
	float attenuation = pow((g/distance),beta);
	lightColor *= attenuation;
	return lightColor;
}

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

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);
	vec3 MainCol = texture(tex, fragUV).rgb;

    vec3 lightDir = normalize(gubo.lightPos - fragPos);
	vec3 lightPos = gubo.lightPos;
	vec3 lightColor = gubo.lightColor;

	vec3 lightPosGem = gubo.lightPos2;
	vec3 lightDirGem = gubo.lightDir;
	vec3 lightColorGem = gubo.lightColor2;

    float sigma = 1.5f;
	vec3 emittingColor = vec3(0.5, 0.0, 0.0) * ubo.isEmitting.x;

	vec3 Ambient = 0.005f * MainCol + 0.5*MainCol*ubo.isEmitting.x;// If emitting increase ambient color intensity
    vec3 pointLightCol = calculatePointLight(lightPos,lightColor,gubo.pointDecayExp.x,gubo.pointDecayExp.y);
	vec3 spotLightCol = calculateSpotLight(lightPosGem,lightDirGem,lightColorGem,gubo.coneInOutDecayExp.z,gubo.coneInOutDecayExp.w,gubo.coneInOutDecayExp.x,gubo.coneInOutDecayExp.y);
	vec3 DiffuseSpec1 = calculateOrenNayarDiffuse(EyeDir, Norm, lightDir, MainCol, sigma) * pointLightCol;
	vec3 Diffuse2 = calculateLambertDiffuse(MainCol,Norm, lightDirGem);
	vec3 Spec2 = calculateBlinnspecular(Norm,lightDirGem,EyeDir,160.0f);
	vec3 DiffuseSpec2 = (Diffuse2 + Spec2)* spotLightCol;
    outColor = vec4(clamp(DiffuseSpec1 + DiffuseSpec2 + Ambient + emittingColor,0.0,1.0), 1.0f);
}