#version 130

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragmentUV;

uniform mat4 cameraMatrix;

void main()
{
	//set the x, y position on the screen
	//gl_Position.xy = vertexPosition;
	gl_Position.xy = (cameraMatrix * vec4(vertexPosition, 0.0, 1.0)).xy;
	
	//the z position is zero since we are in 2D
	gl_Position.z = 0.0;
	
	//indicate that the coordinates are normalized
	gl_Position.w = 1;
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	//fragmentUV = vertexUV;
	
	//OpenGL use weird coordinate for texture which flips the v coordinate
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}