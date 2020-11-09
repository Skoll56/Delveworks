//Fragment shader for light-affected objects
precision mediump float;


struct SpotLight
{		
	vec3 m_pos;
	float m_specIntens;
	float m_angle;
	float m_fadeAngle;
	float m_radius;
	float m_quadratic;
	vec3 m_direction;
	int m_antiLight;
	float m_time;
};


uniform SpotLight in_sLight;


uniform sampler2D in_Texture;
uniform vec3 in_Emissive;
varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;

//uniform mat4 in_Mod;
uniform vec3 in_CamPos;
uniform int in_Shininess;


vec3 calcDifSpec(vec3 _norm, vec4 _tex, float _specInt, float _attenuation, vec3 _lDir)
{
 //Diffuse Light  
  float directness = max(dot(_norm, _lDir), 0.0);


  vec3 t; // 't' is a monster filter. It makes monsters respond in a spooky, Lovecraftian way to light.

  t =  cos(sqrt(vec3(_tex)) * cos(sqrt(in_sLight.m_time)) * cos(tan(_lDir * _lDir)) * in_sLight.m_time) * (length(in_CamPos  - ex_FragPos) * _lDir);
  vec3 cam = in_CamPos;
  cam.y *= tan(in_sLight.m_time) * cam.y;
  t/= _lDir * in_CamPos;
  t.y *= t.y;
  if (t.z < 1.0) {t.z = 2.5;}
  if (t.x > 5.0) 
  {
	t.z = 0.0;
  }


  vec3 difLight = 1.0 - directness * vec3(_tex);
  vec3 dLight = t * difLight; 
  

  //Specular
  vec3 specLight = vec3(0.0, 0.0, 0.0);

  if (dot(_norm, _lDir) > 0.0) // Stop specular affecting facing away objects
  {
	vec3 camDir = normalize(in_CamPos - ex_FragPos);
	vec3 reflection = reflect(_lDir, _norm);
	float shine = pow(dot(camDir, reflection), 1.0);
	
	specLight = (_specInt * shine * t);
  }

  return (dLight + specLight) * (_attenuation / 2.0); 
}


void main()
{
	vec4 tex = texture2D(in_Texture, ex_TexCoord);
	vec3 norm = normalize(ex_Normal);

	vec3 light = vec3(0.0, 0.0, 0.0);
	vec3 lDir;
	float attenuation = 1.0;
   
	lDir = normalize(in_sLight.m_pos - ex_FragPos);
	float theta = dot(normalize(in_sLight.m_direction), -lDir);   
	

	float epsilon = in_sLight.m_angle - in_sLight.m_fadeAngle;
	float intensity = clamp((theta - in_sLight.m_fadeAngle) / epsilon, 0.0, 1.0);
	
	
	float d = length(in_sLight.m_pos - ex_FragPos);	
	float linear = 4.5 / in_sLight.m_radius;
	attenuation = 1.0 / (1.0 + linear * d + in_sLight.m_quadratic * (d * d));
	
	if (intensity > 0.001)
	{		
		light+= calcDifSpec(norm, tex, in_sLight.m_specIntens, attenuation, lDir) * intensity;	
	}
   
  gl_FragColor = tex * vec4(light, 1.0);
}

