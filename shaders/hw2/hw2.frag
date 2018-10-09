#version 330
uniform float inputTime;

out vec4 fragColor;

void main() 
{
    // Insert your code for "Slightly-More Advanced Shaders" here.
    vec4 finalColor = vec4(1,1,1,1) - vec4(0,1,1,1)*sin(inputTime);
    
    fragColor = finalColor;
}
