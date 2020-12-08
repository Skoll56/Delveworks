
precision mediump float;
uniform sampler2D in_Texture;

varying vec2 ex_TexCoord;

void main()
{
   float depthValue = texture2D(in_Texture, ex_TexCoord).r;
   //gl_FragColor = texture2D(in_Texture, ex_TexCoord);

   //Use this one to draw the depth map
   gl_FragColor = vec4(vec3(depthValue), 1.0);
}
