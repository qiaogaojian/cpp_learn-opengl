#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alpha;

void main()
{
	// FragColor = vec4(1.0f,0.5f,0.2f,1.0f);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x,TexCoord.y)), alpha);
}