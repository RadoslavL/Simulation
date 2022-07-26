#version 110
uniform vec3 color;
uniform float textureorcolor;
uniform float shadows;
uniform float lighting;
uniform float normalmap;
uniform sampler2D tex0;
uniform sampler2D shadowmap;
uniform sampler2D normalmap0;
varying vec3 outcolor;
varying vec2 texcoords;
varying vec3 outnormal;
varying vec3 FragPos;
varying vec4 FragLight;
varying vec3 lightdir;
void main(){
   float ambientstrength = 0.2;
   vec3 ambient = ambientstrength * vec3(1.0, 1.0, 0.9);
   //vec3 norm = normalize(outnormal);
   vec3 norm = normalmap * normalize(texture2D(normalmap0, texcoords).rgb * 2.0 - 1.0) + (1.0 - normalmap) * normalize(outnormal);
   //vec3 lightDir = normalize(lightPos - FragPos);
   //vec3 lightdir = normalmap * (TBN * inlightdir) + (1.0 - normalmap) * inlightdir;
   vec3 normlightdir = lightdir; //-normalize(lightdir);
   float diff = max(dot(norm, normlightdir), 0.0);
   vec3 diffuse = diff * vec3(1.0, 1.0, 0.9);
   float shadow = 0.0;
   vec3 lightcoords = FragLight.xyz / FragLight.w;
   if(lightcoords.z <= 1.0){
      lightcoords = lightcoords * 0.5 + 0.5;
      float closestdepth; // = texture2D(shadowmap, lightcoords.xy).r;
      float currentdepth = lightcoords.z;
      //float bias = max(0.00075 * (1.0 - dot(norm, normlightdir)), 0.00001);
      //float bias = 0.00025;
      int sampleradius = 2;
      vec2 pixelsize = 1.0 / vec2(8192, 8192);
      for(int y = -sampleradius; y <= sampleradius; y++){
         for(int x = -sampleradius; x <= sampleradius; x++){
            closestdepth = texture2D(shadowmap, lightcoords.xy + vec2(x, y) * pixelsize).r;
	    if(currentdepth > closestdepth){
               shadow += 1.0;
            }
	 }
      }
      shadow /= pow(float(sampleradius * 2 + 1), float(2));
      /*
      if(currentdepth > closestdepth + bias){
         shadow = 1.0;
      }
      */
   }
   vec4 result = (lighting * vec4(ambient + diffuse * ((1.0 - shadows) + (1.0 - shadow) * shadows), 1.0) + (1.0 - lighting)) * (texture2D(tex0, texcoords) * textureorcolor + vec4(outcolor, 1.0) * (1.0 - textureorcolor)); //vec4(1.0, 1.0, 1.0, 1.0);
   gl_FragColor = result;
}
