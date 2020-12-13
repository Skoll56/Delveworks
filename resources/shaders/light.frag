
precision mediump float;
//Fragment shader for light-affected objects



#define NUMPOINT 1
#define NUMDIR 1
#define NUMSPOT 1

struct PointLight
{
	vec3 m_pos;
	vec3 m_diffuse;
	float m_specIntens;
	float m_radius;
	float m_quadratic;
	samplerCube m_shadowMap;	
};

struct DirLight
{
	vec3 m_diffuse;
	float m_specIntens;
	vec3 m_direction;
	vec3 m_ambient;
	sampler2D m_shadowMap;
	mat4 m_lightMatrix;
};

struct SpotLight
{		
	vec3 m_pos;
	vec3 m_diffuse;
	float m_specIntens;
	float m_angle;
	float m_fadeAngle;
	float m_radius;
	float m_quadratic;
	vec3 m_direction;
	int m_antiLight;
	sampler2D m_shadowMap;
	mat4 m_lightMatrix;
};


uniform PointLight in_pLight[NUMPOINT];
uniform DirLight in_dLight[NUMDIR]; 
uniform SpotLight in_sLight[NUMSPOT];

uniform sampler2D in_Texture;


uniform vec3 in_Emissive;
varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;


uniform vec3 in_CamPos;
uniform int in_Shininess;

//Reference learn OpenGL. 
vec3 calcDifSpec(vec3 _norm, vec4 _tex, vec3 _difCol, float _specInt, float _attenuation, vec3 _lDir)
{
 //Diffuse Light
  float directness = max(dot(_norm, _lDir), 0.0);
  vec3 difLight = directness * vec3(_tex);
  vec3 dLight = _difCol * difLight;

  //Specular
  vec3 specLight = vec3(0.0, 0.0, 0.0);

  if (dot(_norm, _lDir) > 0) // Stop specular affecting facing away objects
  {
	vec3 camDir = normalize(in_CamPos - ex_FragPos);
	vec3 reflection = reflect(_lDir, _norm);
	float shine = pow(max(dot(camDir, reflection), 0.0), in_Shininess);	
	specLight = (_specInt * shine * _difCol);
  }

  return (dLight + specLight) * (_attenuation * 2); 
}


//Reference learn OpenGL
float ShadowCalculation(vec4 _fragPosLightSpace, sampler2D _shadowMap)
{
    //Linearises the depth in perspective matrixes
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;

    //Transform to [0,1] 
    projCoords = projCoords * 0.5 + 0.5;	
	if (projCoords.z > 1.0) {return 0.0; } //Early exit for objects beyond the far plane

    //Get closest depth value from the shadowmap (in 0-1)
    //float closestDepth = texture2D(_shadowMap, projCoords.xy).r;

    //Depth of the current fragment
    float currentDepth = projCoords.z;
   
    
	vec2 texelRes = 1.0 / textureSize(_shadowMap, 0); //The resolution of each texel... I think?

	//if (currentDepth - (0.002 / _fragPosLightSpace.w) > closestDepth) {return 1.0;} //Old code
	//else {return 0.0;}

	float samples = 5.0;
	float inc = 3.0;
	float shadow = 0.0f;
	for (float x = -inc; x < inc; x+= inc / (samples * 0.5)) 
	{
		for (float y = -inc; y < inc; y+= inc / (samples * 0.5)) 
		{			
			float closestDepth = texture2D(_shadowMap, projCoords.xy + vec2(x, y) * texelRes).r;				 
			shadow += currentDepth - (0.002 / _fragPosLightSpace.w) > closestDepth ? 1.0 : 0.0;			
		}
	}	
	
	return shadow / (samples * samples);
}  

float ShadowCubeCalculation(vec3 _fragPos, vec3 _lightPos, samplerCube _shadowMap, float _farPlane)
{
	vec3 fragToLight = _fragPos - _lightPos;    
   
    // check whether current frag pos is in shadow
    //int shadow = currentDepth - 0.1 > closestDepth  ? 1 : 0;
	vec3 directionList[20] = vec3[] //This is an optimisation (although it does degrade quality a bit, it saves 44 samples)
	(
		 vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
		 vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
		 vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
		 vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
		 vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);   
	
	float samples = 20.0;
	float shadow = 0.0f;
	for (int i = 0; i < samples; i++)
	{
		float closestDepth = texture(_shadowMap, fragToLight + directionList[i] * 0.05).r; 
		closestDepth *= _farPlane;
		float currentDepth = length(fragToLight);  
		shadow += currentDepth - 0.4 > closestDepth ? 1.0 : 0.0;
	}
	return shadow / samples;
}  


void main()
{
  vec4 tex = texture2D(in_Texture, ex_TexCoord);
  
  vec3 norm = normalize(ex_Normal);
  vec3 ambient = vec3(0.0, 0.0, 0.0);
  

  for (int i = 0; i < NUMDIR; i++) // For each ambient source
  {
	ambient += in_dLight[i].m_ambient;
  }
  vec3 light = ambient + in_Emissive;
  
  vec3 lDir;
  float attenuation = 1.0;
  vec4 FragPosLightSpace; 
  for (int i = 0; i < NUMDIR; i++) // For each directional light
  {	
	FragPosLightSpace = (in_dLight[i].m_lightMatrix) * vec4(ex_FragPos, 1.0);
	float inShadow = ShadowCalculation(FragPosLightSpace, in_dLight[i].m_shadowMap);	   
	lDir = -in_dLight[i].m_direction;
	light += (1.0 - inShadow) * max(calcDifSpec(norm, tex, in_dLight[i].m_diffuse, in_dLight[i].m_specIntens, attenuation/2.0, lDir), 0.0);
  }

  for (int i = 0; i < NUMPOINT; i++) // For each point light
  {
		float inShadow =  ShadowCubeCalculation(ex_FragPos, in_pLight[i].m_pos, in_pLight[i].m_shadowMap, in_pLight[i].m_radius);	
	
		lDir = normalize(in_pLight[i].m_pos - ex_FragPos);
		float d = length(in_pLight[i].m_pos - ex_FragPos);	
		float linear = 4.5 / in_pLight[i].m_radius;
		attenuation = 1.0 / (1.0 + linear * d + in_pLight[i].m_quadratic * (d * d));
		light += (1.0 - inShadow) * max(calcDifSpec(norm, tex, in_pLight[i].m_diffuse, in_pLight[i].m_specIntens, attenuation, lDir), 0.0); // Don't add negative light. That would be silly.
	
  }

   for (int i = 0; i < NUMSPOT; i++) // For each spotLight
   {
	 FragPosLightSpace = (in_sLight[i].m_lightMatrix) * vec4(ex_FragPos, 1.0);
	 float inShadow = ShadowCalculation(FragPosLightSpace, in_sLight[i].m_shadowMap);
	
		lDir = normalize(in_sLight[i].m_pos - ex_FragPos);
		float theta = dot(normalize(in_sLight[i].m_direction), -lDir);   
		

		float epsilon = in_sLight[i].m_angle - in_sLight[i].m_fadeAngle;
		float intensity = clamp((theta - in_sLight[i].m_fadeAngle) / epsilon, 0.0, 1.0);		
		
		float d = length(in_sLight[i].m_pos - ex_FragPos);	
		float linear = 4.5 / in_sLight[i].m_radius;
		attenuation = 1.0 / (1.0 + linear * d + in_sLight[i].m_quadratic * (d * d));
		light+= (1.0 - inShadow) * max(calcDifSpec(norm, tex, in_sLight[i].m_diffuse, in_sLight[i].m_specIntens, attenuation, lDir), 0.0) * intensity;		 
   }  

   gl_FragColor = tex * vec4(light, 1.0);
}

