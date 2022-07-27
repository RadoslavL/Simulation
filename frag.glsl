#version 110
uniform vec3 color;
uniform sampler2D tex0;
uniform sampler2D shadowmap;
uniform vec3 lightPos;
varying vec3 outcolor;
varying vec2 texcoords;
varying vec3 outnormal;
varying vec3 FragPos;
varying vec4 FragLight;
void main(){
   float ambientstrength = 0.1;
   vec3 ambient = ambientstrength * vec3(1.0, 1.0, 1.0);
   vec3 norm = normalize(outnormal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
   float shadow = 0.0;
   vec3 lightcoords = FragLight.xyz / FragLight.w;
   lightcoords = lightcoords * 0.5 + 0.5;
   float closestdepth = texture2D(shadowmap, lightcoords.xy).r;
   float currentdepth = lightcoords.z;
   if(currentdepth > closestdepth){
      shadow = 0.0;
   }
   vec4 result = vec4(ambient + diffuse * (1.0 - shadow), 1.0) * vec4(outcolor, 1.0);
   gl_FragColor = result;
}
