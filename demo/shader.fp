#version 150

in vec3 fragmentColor;

out vec4 finalFragmentColor;

void
main()
{
	finalFragmentColor = vec4(fragmentColor, 1.0);
}
