#version 110
attribute vec3 Position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
   gl_Position = proj * view * model * vec4(Position, 1.0);
}
