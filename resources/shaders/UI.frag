/* !This has been created as part of the GRAPHICS UNIT! */
//This shader simply draws the scene to a quad on the screen, but can be edited to view a depth map texture also
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D in_Texture;
uniform float in_alpha;
uniform float in_nearPlane;
uniform float in_farPlane;
uniform vec3 in_col;
varying vec2 ex_TexCoord;

void main()
{
	//Use this one to draw the depth map
   //float depthValue = texture2D(in_Texture, ex_TexCoord).r;
   //float z = depthValue * 2.0 - 1.0;
   //depthValue = (2.0 * in_nearPlane * in_farPlane) / (in_farPlane + in_nearPlane - z * (in_farPlane - in_nearPlane));  
   //gl_FragColor = vec4(vec3(depthValue), 1.0);
   vec4 col = texture2D(in_Texture, ex_TexCoord);
   col *= vec4(in_col, 1.0);
   col.a *= in_alpha;
   gl_FragColor = col;
   
}
