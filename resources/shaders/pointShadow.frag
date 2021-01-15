/* !This has been created as part of the GRAPHICS UNIT! */
#ifdef GL_ES
precision mediump float;
#endif
varying vec3 ex_FragPos;
uniform vec3 in_lightPos;
uniform float in_farPlane;


void main()
{             
	#ifndef GL_ES
    float d = length(ex_FragPos.xyz - in_lightPos);
	d = d / in_farPlane;
	gl_FragDepth = d;	
	#endif
}  
