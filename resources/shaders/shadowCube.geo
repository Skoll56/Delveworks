//Vertex shader for shadowmapping
uniform mat4 in_Model;

attribute vec3 in_Position;


void main()
{
  gl_Position = in_Model * vec4(in_Position, 1.0);  
}

