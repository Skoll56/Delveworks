
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

//Reference learn OpenGL
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
int ShadowCalculation(vec4 _fragPosLightSpace, sampler2D _shadowMap)
{
    // perform perspective divide
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;	

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(_shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - (0.005 / _fragPosLightSpace.w) > closestDepth  ? 1.0 : 0.0;

    if (shadow == 1.0) {return 1;}    
    else {return 0;} 
	
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
	int inShadow = ShadowCalculation(FragPosLightSpace, in_dLight[i].m_shadowMap);
	if (inShadow == 0)
	{	   
	   lDir = -in_dLight[i].m_direction;
	   light += max(calcDifSpec(norm, tex, in_dLight[i].m_diffuse, in_dLight[i].m_specIntens, attenuation/2.0, lDir), 0.0);
	}

  }

  for (int i = 0; i < NUMPOINT; i++) // For each point light
  {
	lDir = normalize(in_pLight[i].m_pos - ex_FragPos);
	float d = length(in_pLight[i].m_pos - ex_FragPos);	
	float linear = 4.5 / in_pLight[i].m_radius;
	attenuation = 1.0 / (1.0 + linear * d + in_pLight[i].m_quadratic * (d * d));
	light += max(calcDifSpec(norm, tex, in_pLight[i].m_diffuse, in_pLight[i].m_specIntens, attenuation, lDir), 0.0); // Don't add negative light. That would be silly.
  }

   for (int i = 0; i < NUMSPOT; i++) // For each spotLight
   {
	 FragPosLightSpace = (in_sLight[i].m_lightMatrix) * vec4(ex_FragPos, 1.0);
	 int inShadow = ShadowCalculation(FragPosLightSpace, in_sLight[i].m_shadowMap);
	 if (inShadow == 0)
	 {
		lDir = normalize(in_sLight[i].m_pos - ex_FragPos);
		float theta = dot(normalize(in_sLight[i].m_direction), -lDir);   
		

		float epsilon = in_sLight[i].m_angle - in_sLight[i].m_fadeAngle;
		float intensity = clamp((theta - in_sLight[i].m_fadeAngle) / epsilon, 0.0, 1.0);
		//if (intensity < 1.0) {intensity = 1.0;}
		
		float d = length(in_sLight[i].m_pos - ex_FragPos);	
		float linear = 4.5 / in_sLight[i].m_radius;
		attenuation = 1.0 / (1.0 + linear * d + in_sLight[i].m_quadratic * (d * d));
		
		if (in_sLight[i].m_antiLight == 0) // If a normal light
		{
			light+= max(calcDifSpec(norm, tex, in_sLight[i].m_diffuse, in_sLight[i].m_specIntens, attenuation, lDir), 0.0) * intensity;		
		}

		else //If an anti light
		{
			
			light+= min(calcDifSpec(norm, tex, in_sLight[i].m_diffuse, in_sLight[i].m_specIntens, attenuation, lDir), 0.0) * intensity; // sLight[2] is a 'blotch' (anti-light)
		}
	 }
   }  
   gl_FragColor = tex * vec4(light, 1.0);
}

