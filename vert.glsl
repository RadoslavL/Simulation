#version 110
attribute vec3 Position;
attribute vec3 incolor;
attribute vec2 Tex;
attribute vec3 innormal;
varying vec3 outcolor;
varying vec2 texcoords;
varying vec3 outnormal;
varying vec3 FragPos;
varying vec4 FragLight;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 light;
void main(){
   gl_Position = proj * view * model * vec4(Position, 1.0);
   FragPos = (model * vec4(Position, 1.0)).xyz;
   outcolor = incolor;
   texcoords = Tex;
   outnormal = (model * vec4(innormal, 0.0)).xyz;
   FragLight = light * model * vec4(Position, 1.0);
}
