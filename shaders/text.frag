#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textMaterial;

void main()
{
	vec4 texColor = texture(textMaterial, TexCoords);
	if (texColor.a < 0.1)
		discard;
	if (texColor.r == 0.0)
	{
		FragColor = vec4(0.0, 0.0, 0.0, 0.5);
	}
	else
	{
    FragColor = texColor;
	}
}
