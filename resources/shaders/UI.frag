
precision mediump float;
uniform sampler2D in_Texture;
uniform float in_nearPlane;
uniform float in_farPlane;
varying vec2 ex_TexCoord;

void main()
{
   float depthValue = texture2D(in_Texture, ex_TexCoord).r;
   float z = depthValue * 2.0 - 1.0;
   depthValue = (2.0 * in_nearPlane * in_farPlane) / (in_farPlane + in_nearPlane - z * (in_farPlane - in_nearPlane));

   gl_FragColor = texture2D(in_Texture, ex_TexCoord);
   //Use this one to draw the depth map
   //gl_FragColor = vec4(vec3(depthValue), 1.0);
}
