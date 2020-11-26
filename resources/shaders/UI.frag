precision mediump float;

uniform sampler2D in_Texture;

varying vec2 ex_TexCoord;

void main()
{
  gl_FragColor = texture2D(in_Texture, ex_TexCoord);
}
