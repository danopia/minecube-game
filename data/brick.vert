varying vec3 texture_coordinate;	

void main(void)
{
	// Transforming The Vertex
    gl_Position = ftransform();

	// Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader
	texture_coordinate = vec3(gl_MultiTexCoord0);
}
