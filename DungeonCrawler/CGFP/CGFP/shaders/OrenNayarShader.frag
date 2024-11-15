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
	//No Ms since not metallic, white reflection
	return vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)),0.0,1.0), gamma));
}

//Calculate Lambert Diffuse component
vec3 calculateLambertDiffuse(vec3 Md,vec3 Norm, vec3 lightDir){
	return Md*max(dot(lightDir,Norm),0);
}


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

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);

	vec3 lightDir = gubo.lightDir;
	vec3 lightColor = gubo.lightColor.rgb;
	vec3 lightPos = gubo.lightPos;

	vec3 lightPosGem = gubo.lightPos4;
	vec3 lightDirGem = gubo.lightDir2;
	vec3 lightColorGem = gubo.lightColor4;

	vec3 MainCol = texture(tex, fragUV).rgb;
	float sigma = 1.5f;

	vec3 lightModel = calculateSpotLight(lightPos,lightDir,lightColor,g,beta,cosin,cosout);
    vec3 Diffuse = calculateOrenNayarDiffuse(EyeDir,Norm,lightDir,MainCol,sigma)  * lightModel; // OrenNayar typically has no specular (sigma [0,pi/2] for roughness)

    vec3 gemSpotLightCol = calculateSpotLight(lightPosGem,lightDirGem,lightColorGem,gubo.coneInOutDecayExp.z,gubo.coneInOutDecayExp.w,gubo.coneInOutDecayExp.x,gubo.coneInOutDecayExp.y);
    vec3 Diffuse2 = calculateLambertDiffuse(MainCol,Norm, lightDirGem);
	vec3 Spec2 = calculateBlinnspecular(Norm,lightDirGem,EyeDir,160.0f);
	vec3 DiffuseSpec2 = (Diffuse2 + Spec2)* gemSpotLightCol;

    // vec3 Ambient = MainCol * 0.005f;
	vec3 Ambient = vec3(0,0,0);
	outColor = vec4(clamp( Diffuse + DiffuseSpec2 + Ambient,0.0,1.0), 1.0f);
}