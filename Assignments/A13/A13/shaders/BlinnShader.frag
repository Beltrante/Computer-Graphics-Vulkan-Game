#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;


layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightPos;	// position of the spotlight
	vec3 lightDir;	// direction of the spotlight
	vec4 lightColor;// color of the spotlight	
	vec3 eyePos;	// position of the viewer
} gubo;

layout(binding = 3) uniform sampler2D texEmit;

const float beta = 2.0f;	// decay exponent of the spotlight
const float g = 3;			// target distance of the spotlight
const float cosout = 0.5;	// cosine of the outer angle of the spotlight
const float cosin  = 0.95;	// cosine of the inner angle of the spotlight

const float gamma = 160.0f;	// cosine power for the Blinn specular reflection

// coefficients for the spehrical harmonics ambient light term
const vec3 C00  = vec3( .38f, .43f, .45f)/8.0f;
const vec3 C1m1 = vec3( .29f, .36f, .41f)/8.0f;
const vec3 C10  = vec3( .04f, .03f, .01f)/8.0f;
const vec3 C11  = vec3(-.10f,-.10f,-.09f)/8.0f;
const vec3 C2m2 = vec3(-.06f,-.06f,-.04f)/8.0f;
const vec3 C2m1 = vec3( .01f,-.01f,-.05f)/8.0f;
const vec3 C20  = vec3(-.09f,-.13f,-.15f)/8.0f;
const vec3 C21  = vec3(-.06f,-.05f,-.04f)/8.0f;
const vec3 C22  = vec3( .02f, .00f,-.05f)/8.0f;

vec3 SphericalArmonics(vec3 N,vec3 MA) {
    // Compute the spherical harmonics
    vec3 ambient = 
        C00 +
		C1m1 * N.x +
        C10 * N.y +
        C11 * N.z +
        C2m2 * (N.x * N.y) +
        C2m1 * (N.y * N.z) +
		C21 * (N.x * N.z) +
		C22 * (N.x * N.x - N.y * N.y) +
        C20 * (3 * N.z * N.z - 1);
    return ambient * MA;
}

//Calculate Blinn Specular component
vec3 calculateBlinnspecular(vec3 Norm, vec3 lightDir, vec3 EyeDir, float gamma){
	return vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)),0.0,1.0), gamma));
}

//Calculate Lambert Diffuse component
vec3 calculateLambertDiffuse(vec3 Md,vec3 Norm, vec3 lightDir){
	return Md*max(dot(lightDir,Norm),0);
}

//Calculate SpotLight light model
vec3 calculateSpotLight(vec3 lightDir, vec3 lightColor){
	/////////Start by calculating L0(l,lx) as l*(g/|p-x|)^beta
	float distance = length(gubo.lightPos - fragPos);
	float attenuation = pow((g/distance),beta);
    lightColor *= attenuation;
	////////Calculate clamp((normalize*gubo.lightDir-cosout)/(cosin-cosout))
	float coneNumerator = dot(normalize(gubo.lightPos - fragPos),gubo.lightDir) - cosout;
	float coneDenominator = cosin - cosout;
	float cone = clamp(coneNumerator/coneDenominator,0.0f,1.0f); //Clamp is used to make the cone dimming effect
	////////Multiply the 2
	lightColor *= cone;
	return lightColor;
}

void main() {
	vec3 N = normalize(fragNorm);				// surface normal
	vec3 V = normalize(gubo.eyePos - fragPos);	// viewer direction
	vec3 MD = texture(tex, fragUV).rgb;			// diffuse color
	vec3 MA = MD;								// ambient color
	vec3 MS = vec3(1);							// specular color
	vec3 ME = texture(texEmit, fragUV).rgb;		// emission color

	// Write the shader here

    vec3 lightColor = gubo.lightColor.rgb;
    vec3 lightDir = normalize(gubo.lightPos - fragPos);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);


    vec3 lightModel = calculateSpotLight(lightDir,lightColor);
	vec3 Diffuse = calculateLambertDiffuse(MD,N,lightDir) * 0.995f;
	/////// make as vec3(0) to be the same as the slides
	vec3 Specular = calculateBlinnspecular(N,lightDir,EyeDir,160.0f);
	// vec3 Specular = vec3(0);	
	vec3 Ambient = SphericalArmonics(N,MA);
	outColor = vec4(clamp(ME + (Diffuse + Specular) * lightModel.rgb + Ambient,0.0,1.0), 1.0f);
}