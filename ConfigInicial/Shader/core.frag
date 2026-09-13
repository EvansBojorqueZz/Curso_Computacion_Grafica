#version 330 core
in vec3 ourColor;
out vec4 color;
 
uniform vec3 colorPieza;   // color solido de la pieza que se esta dibujando
 
void main()
{
    color = vec4(colorPieza, 1.0f);
}
 