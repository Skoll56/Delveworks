precision mediump float;
/* !DISCLAIMER! */
/* The specular/diffuse lighting code has been re-used from a previously submitted assignment. Only the addition of shadows is new. */


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
};

struct DirLight
{
	vec3 m_diffuse;
	float m_specIntens;
	vec3 m_direction;
	vec3 m_ambient;	
	vec2 m_textureSize;
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
	vec2 m_textureSize;
	mat4 m_lightMatrix;
};

uniform PointLight in_pLight[NUMPOINT];
uniform samplerCube in_pointShadowMap[NUMDIR];

uniform DirLight in_dLight[NUMDIR]; 
uniform sampler2D in_dirShadowMap[NUMDIR];

uniform SpotLight in_sLight[NUMSPOT];
uniform sampler2D in_spotShadowMap[NUMPOINT];

uniform sampler2D in_Texture;
uniform float in_alpha;
uniform vec3 in_color;
uniform vec3 in_Emissive;
uniform vec3 in_CamPos;
uniform int in_Shininess;
uniform int in_rShadows;

varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;

vec3 directionList[20]; //This is an optimisation for PCF (although it does degrade quality a bit, it saves 44 samples)


/* !This function is for lighting calculations and is not part of this Graphics Assignment! */
vec3 calcDifSpec(vec3 _norm, vec4 _tex, vec3 _difCol, float _specInt, float _attenuation, vec3 _lDir)
{
 //Diffuse Light
  float directness = max(dot(_norm, _lDir), 0.0);
  vec3 difLight = directness * vec3(_tex);
  vec3 dLight = _difCol * difLight;

  //Specular
  vec3 specLight = vec3(0.0, 0.0, 0.0);

  if (dot(_norm, _lDir) > 0.0) // Stop specular affecting facing away objects
  {
	vec3 camDir = normalize(in_CamPos - ex_FragPos);
	vec3 reflection = reflect(_lDir, _norm);
	float shine = pow(max(dot(camDir, reflection), 0.0), float(in_Shininess));	
	specLight = (_specInt * shine * _difCol);
  }

  return (dLight + specLight) * (_attenuation * 2.0); 
}

void fillDirList()
{
	directionList[0] = vec3( 1,  1,  1);
	directionList[1] = vec3( 1, -1,  1);
	directionList[2] = vec3(-1, -1,  1);
	directionList[3] = vec3(-1,  1,  1);
	directionList[4] = vec3( 1,  1, -1);
	directionList[5] = vec3( 1, -1, -1);
	directionList[6] = vec3(-1, -1, -1);
	directionList[7] = vec3(-1,  1, -1);
	directionList[8] = vec3( 1,  1,  0);
	directionList[9] = vec3( 1, -1,  0);
	directionList[10] = vec3(-1, -1,  0);
	directionList[11] = vec3(-1,  1,  0);
	directionList[12] = vec3( 1,  0,  1);
	directionList[13] = vec3(-1,  0,  1);
	directionList[14] = vec3( 1,  0, -1);
	directionList[15] = vec3(-1,  0, -1);
	directionList[16] = vec3( 0,  1,  1);
	directionList[17] = vec3( 0, -1,  1);
	directionList[18] = vec3( 0, -1, -1);
	directionList[19] = vec3( 0,  1, -1);
}



/* This function is to calculate shadows with 2D Depth Maps and is part of the GRAPHICS UNIT */
//This code has been based on the tutorial from LearnOpenGL
float ShadowCalculation(vec4 _fragPosLightSpace, sampler2D _shadowMap, vec2 _textureSize)
{
    //Linearises the depth in perspective matrixes
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;

    //Transform to [0,1] 
    projCoords = projCoords * 0.5 + 0.5;	
	if (projCoords.z > 1.0) {return 0.0; } //Early exit for objects beyond the far plane

    //Depth of the current fragment
    float currentDepth = projCoords.z;

	//The resolution of each texel
    //ivec2 ts = textureSize(_shadowMap, 0);
	//float x = float(ts.x);
	//float y = float(ts.y);
	//vec2 fTextureSize = vec2(ts.x, ts.y);
	vec2 texelRes = (1.0 / _textureSize); 
	
	const float samples = 5.0;
	const float inc = 3.0;
	float shadow = 0.0;
	for (float x = -inc; x < inc; x+= inc / (samples * 0.5)) 
	{
		for (float y = -inc; y < inc; y+= inc / (samples * 0.5)) 
		{			
			float closestDepth = texture2D(_shadowMap, projCoords.xy + vec2(x, y) * texelRes).r; //Sample surrounding texels on the X and Y for PCF			
			shadow += currentDepth - (0.002 / _fragPosLightSpace.w) > closestDepth ? 1.0 : 0.0;			
		}
	}		
	return shadow / (samples * samples); //Average out the surrounding samples to create a 'blur' like effect (PCF)
}  

/* This function is to calculate shadows with 3D Depth Cubes and is part of the GRAPHICS UNIT */
//This code has been based on the tutorial from LearnOpenGL
float ShadowCubeCalculation(vec3 _fragPos, vec3 _lightPos, samplerCube _shadowMap, float _farPlane)
{
	vec3 fragToLight = _fragPos - _lightPos;    
	
	
	const int samples = 20;
	float shadow = 0.0;
	for (int i = 0; i < samples; i++)
	{
		float closestDepth = textureCube(_shadowMap, fragToLight + directionList[i] * 0.05).r; //Sample surrounding texels (for PCF)
		closestDepth *= _farPlane;
		float currentDepth = length(fragToLight);  		

		shadow += currentDepth - 0.4 > closestDepth ? 1.0 : 0.0;
	}
	return shadow / float(samples); //Average out the surrounding texels (for PCF)
}  

/* !This function has been ADAPTED slightly to support the graphics unit (Adding the shadows into the final light value)! */
/* The rest is part of the pre-existing lighting pipeline */
void main()
{
  vec4 tex = texture2D(in_Texture, ex_TexCoord);
  fillDirList();
  vec3 norm = normalize(ex_Normal);
  vec3 ambient = vec3(0.0, 0.0, 0.0);
  float inShadow = 0.0; // <---- This bit is the new bit for the GRAPHICS UNIT

  for (int i = 0; i < NUMDIR; i++) // For each ambient source
  {
	ambient += in_dLight[i].m_ambient; //All fragments get ambient light, even ones in shadow.
  }
  vec3 light = ambient + in_Emissive; //Emissive light is also irrelevant to shadow.
  
  vec3 lDir;
  float attenuation = 1.0;
  vec4 FragPosLightSpace; //The fragment's position for SHADOWMAPS (GRAPHICS UNIT)


  for (int i = 0; i < NUMDIR; i++) // For each directional light
  {	
	if (in_rShadows == 1)
	{
		FragPosLightSpace = (in_dLight[i].m_lightMatrix) * vec4(ex_FragPos, 1.0);
		inShadow = ShadowCalculation(FragPosLightSpace, in_dirShadowMap[i], in_dLight[i].m_textureSize); //Calculate how much in-shadow the fragment is	(GRAPHICS UNIT)   
	}
	lDir = -in_dLight[i].m_direction;
	light += (1.0 - inShadow) * max(calcDifSpec(norm, tex, in_dLight[i].m_diffuse, in_dLight[i].m_specIntens, attenuation/2.0, lDir), 0.0); //Add to the light value
  }

  for (int i = 0; i < NUMPOINT; i++) // For each point light
  {
	if (in_rShadows == 1)
	{
		inShadow =  ShadowCubeCalculation(ex_FragPos, in_pLight[i].m_pos, in_pointShadowMap[i], in_pLight[i].m_radius); //Calculate how much in-shadow the fragment is
	}
		lDir = normalize(in_pLight[i].m_pos - ex_FragPos);
		float d = length(in_pLight[i].m_pos - ex_FragPos);	
		float linear = 4.5 / in_pLight[i].m_radius;
		attenuation = 1.0 / (1.0 + linear * d + in_pLight[i].m_quadratic * (d * d));
		light += (1.0 - inShadow) * max(calcDifSpec(norm, tex, in_pLight[i].m_diffuse, in_pLight[i].m_specIntens, attenuation, lDir), 0.0); //Add to the light value
	
  }

   for (int i = 0; i < NUMSPOT; i++) // For each spotLight
   {
		 if (in_rShadows == 1)
		{
			FragPosLightSpace = (in_sLight[i].m_lightMatrix) * vec4(ex_FragPos, 1.0);
			inShadow = ShadowCalculation(FragPosLightSpace, in_spotShadowMap[i], in_sLight[i].m_textureSize); //Calculate how much in-shadow the fragment is	 
		}
		lDir = normalize(in_sLight[i].m_pos - ex_FragPos);
		float theta = dot(normalize(in_sLight[i].m_direction), -lDir);   		

		float epsilon = in_sLight[i].m_angle - in_sLight[i].m_fadeAngle;
		float intensity = clamp((theta - in_sLight[i].m_fadeAngle) / epsilon, 0.0, 1.0);		
		
		float d = length(in_sLight[i].m_pos - ex_FragPos);	
		float linear = 4.5 / in_sLight[i].m_radius;
		attenuation = 1.0 / (1.0 + linear * d + in_sLight[i].m_quadratic * (d * d));
		light+= (1.0 - inShadow) * max(calcDifSpec(norm, tex, in_sLight[i].m_diffuse, in_sLight[i].m_specIntens, attenuation, lDir), 0.0) * intensity;	//Add to the light value	 
   }  
   
	tex.a *= in_alpha;
	tex *= vec4(in_color, 1.0);
    gl_FragColor = tex * vec4(light, 1.0); //Use the sum of the light from each source to add light to the colour of the fragment
}

