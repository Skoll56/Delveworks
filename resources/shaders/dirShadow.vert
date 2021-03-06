//Vertex shader for shadowmapping
/* !This has been created as part of the GRAPHICS UNIT! */

uniform mat4 in_LightSpaceMatrix;
uniform mat4 in_Model;

attribute vec3 in_Position;


void main()
{
  gl_Position = in_LightSpaceMatrix * in_Model * vec4(in_Position, 1.0);  
}

