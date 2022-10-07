#version 110
attribute vec3 Position;
attribute vec3 incolor;
attribute vec2 Tex;
attribute vec3 innormal;
attribute vec3 tangent;
attribute vec3 bitangent;
varying vec3 outcolor;
varying vec2 texcoords;
varying vec3 outnormal;
varying vec3 FragPos;
varying vec4 FragLight;
varying vec3 lightdir;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 light;
uniform vec3 inlightdir;
uniform float normalmap;
void main(){
   gl_Position = proj * view * model * vec4(Position, 1.0);
   FragPos = (model * vec4(Position, 1.0)).xyz;
   outnormal = (model * vec4(innormal, 0.0)).xyz;
   outcolor = incolor;
   texcoords = Tex;
   FragLight = light * model * vec4(Position, 1.0);
   /*
   mat4 MV4x4 = view * model;
   mat3 MV3x3 = mat3(MV4x4[0].xyz, MV4x4[1].xyz, MV4x4[2].xyz);
   mat3 invTBN = mat3(MV3x3 * normalize(tangent), MV3x3 * normalize(bitangent), MV3x3 * normalize(outnormal));
   mat3 TBN = mat3(vec3(invTBN[0].x, invTBN[1].x, invTBN[2].x), vec3(invTBN[0].y, invTBN[1].y, invTBN[2].y), vec3(invTBN[0].z, invTBN[1].z, invTBN[2].z)); 
   */
   //vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
   vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
   vec3 N = normalize(vec3(model * vec4(innormal, 0.0)));
   //vec3 T = normalize(cross(B, N));
   vec3 T;
   if(length(tangent) < 0.001){
      T = normalize(cross(B, N));
   }else{
      T = normalize(vec3(model * vec4(tangent, 0.0)));
   }
   mat3 invTBN = mat3(T, B, N);
   mat3 TBN = mat3(vec3(invTBN[0].x, invTBN[1].x, invTBN[2].x), vec3(invTBN[0].y, invTBN[1].y, invTBN[2].y), vec3(invTBN[0].z, invTBN[1].z, invTBN[2].z));
   lightdir = /*-normalize(inlightdir);*/ normalize(normalmap * TBN * (-inlightdir) + (1.0 - normalmap) * (-inlightdir));
}
