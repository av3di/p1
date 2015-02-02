attribute vec3 inPosition;
attribute vec3 inNormal;
uniform vec3 lightDirection; 
varying vec3 varColor;
void main()
{
    gl_Position = inPosition;
    vec3 normal = inNormal;
    float dotValue = max(dot(normal, lightDirection), 0.0);
    varColor = vec3(dotValue);
}