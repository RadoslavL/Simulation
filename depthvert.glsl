#version 110
attribute vec3 Position;
uniform mat4 light;
uniform mat4 model;

void main(){
   gl_Position = light * model * vec4(Position, 1.0);
}
