#version 330

layout(location = 0) in vec4 vertexPosition;
uniform float inputTime;

void main()
{
    
    // Insert your code for "Slightly-More Advanced Shaders" here.
    vec4 modifiedVertexPosition =
    vertexPosition
    - vec4(0,1,0,0)*sin(inputTime)/2 // move up and down
    - vec4(0,1,0,0)/2; // move y position down a half to bottom of screen
    
    gl_Position = modifiedVertexPosition;
}
