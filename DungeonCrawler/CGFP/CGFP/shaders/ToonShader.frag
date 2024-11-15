layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
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

//Calculate Lambert Diffuse component
vec3 calculateLambertDiffuse(vec3 Md,vec3 Norm, vec3 lightDir){
	return Md * max(dot(lightDir,Norm),0);
}

//Calculate Phong Specular component
vec3 calculatePhongSpecular(vec3 Ms,vec3 Norm, vec3 lightDir, vec3 EyeDir, float gamma){
	vec3 rl = - reflect(lightDir,Norm);
	return Ms * clamp(pow(dot(EyeDir,rl),gamma),0,1);
}

//Calculate Toon Md component
vec3 calculateToonMd(vec3 DiffColor,vec3 Norm, vec3 lightDir){
	float intensity = dot(lightDir,Norm);
	if (intensity > 0.95)
		return vec3(0.8f * DiffColor);
	else if (intensity > 0.5)
		return vec3(0.6f * DiffColor);
	else if (intensity > 0.25)
		return vec3(0.4f * DiffColor);
	else
		return vec3(0.2f * DiffColor);
}

//Calculate Toon Ms component
vec3 calculateToonMs(vec3 Norm, vec3 lightDir,vec3 EyeDir){
	vec3 rl = - reflect(lightDir,Norm);
	float intensity = dot(EyeDir,rl);
	if (intensity > 0.95)
		return vec3(0.8f * vec3(1,1,1));
	else
		return vec3(0,0,0);
}


void main(){
    vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);
    vec3 lightDir = gubo.lightDir;
    vec3 lightColor = gubo.lightColor.rgb;
    vec3 MainCol = texture(tex, fragUV).rgb;

    vec3 Md = calculateToonMd(MainCol,Norm,lightDir);
    vec3 fdiffuse = calculateLambertDiffuse(Md,Norm,lightDir);

    vec3 Ms = calculateToonMs(Norm,lightDir,EyeDir);
    vec3 fspecular = calculatePhongSpecular(Ms,Norm,lightDir,EyeDir,1);//with gamma = 1
	
    vec3 Ambient = MainCol * 0.005f;

    outColor = vec4(clamp((fdiffuse + fspecular) + Ambient,0.0,1.0), 1.0f);
    }