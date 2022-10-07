#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<stdbool.h>
#include<string.h>
#include<stb/stb_image.h>
#include<cglm/cglm.h>
#include"obj.h"

const unsigned int width = 1054;
const unsigned int height = 1057;
float sensitivity = 0.1f;
float degreesx = 1.0f;
float degreesy = 0.5f;
float lastx = (float)width/2;
float lasty = (float)height/2;
float xoffset;
float yoffset;
float yaw = -90.0f;
float pitch = 0.0f;
float jumpacceleration = 5.0f;
float gravity = -0.3f;
float fallinggravity = -4.0f;
GLuint checkfront = 0;
GLuint checkback = 0;
GLuint checkleft = 0;
GLuint checkright = 0;
GLuint jumppressed = 0;
GLuint jump = 0;
GLuint falling = 0;
GLuint kill = 0;
GLuint firstclick = 1;
GLuint speedincrease = 0;
GLuint slow = 0;
GLuint zoom = 0;
GLuint zoomedin = 0;
GLuint texturechange = 0;
GLuint texturestate = 1;
GLuint shadowchange = 0;
GLuint shadowstate = 1;
GLuint lightingchange = 0;
GLuint lightingstate = 1;
GLuint normalmapchange = 0;
GLuint normalmapstate = 1;
GLuint uniColor;
GLuint xploc;
GLuint yploc;
GLuint tex0uni;
void key_callback();
void mouse_callback();

int main(int argc, char *argv[]){
   FILE* vertexshader = fopen("vert.glsl", "r");
   FILE* fragmentshader = fopen("frag.glsl", "r");
   FILE* depthvertex = fopen("depthvert.glsl", "r");
   FILE* depthfragment = fopen("depthfrag.glsl", "r");
   FILE* lightvertex = fopen("lightvert.glsl", "r");
   FILE* lightfragment = fopen("lightfrag.glsl", "r");
   char pVertexShaderText[5000] = {'\0'};
   char pFragmentShaderText[5000] = {'\0'};
   char pDepthVertexShaderText[5000] = {'\0'};
   char pDepthFragmentShaderText[5000] = {'\0'};
   char pLightVertexShaderText[5000] = {'\0'};
   char pLightFragmentShaderText[5000] = {'\0'};
   char buf[100];
   while(fgets(buf, 100, vertexshader) != NULL){
      strcat(pVertexShaderText, buf);
   }
   while(fgets(buf, 100, fragmentshader) != NULL){
      strcat(pFragmentShaderText, buf);
   }
   while(fgets(buf, 100, depthvertex) != NULL){
      strcat(pDepthVertexShaderText, buf);
   }
   while(fgets(buf, 100, depthfragment) != NULL){
      strcat(pDepthFragmentShaderText, buf);
   }
   while(fgets(buf, 100, lightvertex) != NULL){
      strcat(pLightVertexShaderText, buf);
   }
   while(fgets(buf, 100, lightfragment) != NULL){
      strcat(pLightFragmentShaderText, buf);
   }
   unsigned int verticessize = 0;
   unsigned int texturecoordsize = 0;
   unsigned int normalssize = 0;
   unsigned int tangentssize = 0;
   unsigned int bitangentssize = 0;
   unsigned int colorssize = 0;
   float *vertex;
   float *texturecoords;
   float *normals;
   float *tangents;
   float *bitangents;
   unsigned int sunverticessize = 0;
   unsigned int suntexturecoordsize = 0;
   unsigned int sunnormalssize = 0;
   float *sunvertex;
   float *suntexturecoords;
   float *sunnormals;
   int i;
   if(load_obj("flat-plane1.obj", &vertex, &texturecoords, &normals, &tangents, &bitangents, &verticessize, &texturecoordsize, &normalssize, &tangentssize, &bitangentssize) != 0){
      printf("Something went wrong!\n");
      return 1;
   }else{
      printf("Model loaded successfully from file!\n");
   }
   if(load_obj("sphere.obj", &sunvertex, &suntexturecoords, &sunnormals, NULL, NULL, &sunverticessize, &suntexturecoordsize, &sunnormalssize, NULL, NULL) != 0){
      printf("Something went wrong!\n");
      return 1;
   }else{
      printf("Model loaded successfully from file!\n");
   }
   float *colors;
   for(i = 0; i < verticessize / sizeof(float) / 3; i++){
      insertarrayfloat(&colors, i * 3, 1.0f);
      insertarrayfloat(&colors, i * 3 + 1, 1.0f);
      insertarrayfloat(&colors, i * 3 + 2, 1.0f);
      colorssize += sizeof(colors[i * 3]) * 3;
   }
   for(i = 0; i < argc; i++){
      if(strcmp(argv[i], "hello") == 0){
         printf("Hi!\n");
      }
   }
   if(!glfwInit()){
      printf("Failed to initialize glfw!\n");
      return 1;
   }else{
      printf("Successfully initialized glfw!\n");
   }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_SAMPLES, 8);
   //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
   GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   if(!window){
      printf("Window creation failed!\n");
      return 1;
   }else{
      printf("Window created successfully!\n");
   }
   glfwMakeContextCurrent(window);
   glfwSetKeyCallback(window, key_callback);
   glfwSetCursorPosCallback(window, mouse_callback);
   glewExperimental = GL_TRUE;
   if(glewInit() != GLEW_OK){
      printf("Failed to initialize glew!\n");
      return 1;
   }else{
      printf("Successfully initialized glew!\n");
   } 
   glfwSwapInterval(1);
   float Vertices[] = {
    //Front
     -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f, //0
     -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, //1
      0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, //2
      0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f, //3
     
    //Back
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, //4
      0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f, //5
     -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f, //6
     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, //7

    //Right side
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, //8
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, //9
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, //10
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, //11

    //Left side
     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, //12
     -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, //13
     -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, //14
     -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, //15

    //Top
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f, //16
     -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f, //17
      0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, //18
      0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, //19

    //Bottom
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, //20
     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, //21
     -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f, //22
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f, //23
   };

   float floor[] = {
     -2.0f, -0.5f,  2.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f, //0
     -2.0f, -0.5f, -2.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f, //1
      2.0f, -0.5f, -2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f, 0.0f, //2
      2.0f, -0.5f,  2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f, 0.0f //3
   };

   unsigned int indices[] = {
    //Front
      2, 1, 0,
      3, 2, 0,
    //Back
      6, 5, 4,
      7, 6, 4,
    //Right side
      10, 9, 8,
      11, 10, 8,
    //Left side
      14, 13, 12,
      15, 14, 12,
    //Top
      18, 17, 16,
      19, 18, 16,
    //Bottom
      22, 21, 20,
      23, 22, 20
   };

   unsigned int floorindices[] = {
      2, 1, 0,
      3, 2, 0
   };

   printf("GLfloat variable called successfully\n"); 
   GLuint ShaderProgram = glCreateProgram();
   GLuint ShaderObjv = glCreateShader(GL_VERTEX_SHADER);
   GLuint ShaderObjf = glCreateShader(GL_FRAGMENT_SHADER);
   const GLchar* pvs[1];
   const GLchar* pfs[1];
   GLchar InfoLog1[1024];
   GLchar InfoLog2[1024];
   GLchar InfoLog3[1024];
   GLchar InfoLog4[1024];
   pvs[0] = pVertexShaderText;
   pfs[0] = pFragmentShaderText;
   GLint LenghtsVertex[1];
   GLint LenghtsFragment[1];
   LenghtsVertex[0] = strlen(pVertexShaderText);
   LenghtsFragment[0] = strlen(pFragmentShaderText);
   glShaderSource(ShaderObjv, 1, pvs, LenghtsVertex);
   glShaderSource(ShaderObjf, 1, pfs, LenghtsFragment);
   glCompileShader(ShaderObjv);
   glCompileShader(ShaderObjf);
   GLint successvs;
   glGetShaderiv(ShaderObjv, GL_COMPILE_STATUS, &successvs);
   if(!successvs){
       glGetShaderInfoLog(ShaderObjv, sizeof(InfoLog1), NULL, InfoLog1);
       fprintf(stderr, "Error compiling shader type vertex: '%s'\n", InfoLog1);
       return 1;
   }
   GLint successfs;
   glGetShaderiv(ShaderObjf, GL_COMPILE_STATUS, &successfs);
   if(!successfs){
       glGetShaderInfoLog(ShaderObjf, sizeof(InfoLog2), NULL, InfoLog2);
       fprintf(stderr, "Error compiling shader type fragment: '%s'\n", InfoLog2);
       return 1;
   }
   glAttachShader(ShaderProgram, ShaderObjv);
   glAttachShader(ShaderProgram, ShaderObjf);
   glLinkProgram(ShaderProgram);
   GLint success;
   glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
   if(!success){
      glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog3), NULL, InfoLog3);
      fprintf(stderr, "Error linking shader program: '%s'\n", InfoLog3);
      return 1;
   }
   glValidateProgram(ShaderProgram);
   GLint successl;
   glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &successl);
   if(!successl){
      glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog4), NULL, InfoLog4);
      fprintf(stderr, "Error validating shader program: '%s'\n", InfoLog4);
      return 1;
   }
   glUseProgram(ShaderProgram);
   GLuint DepthProgram = glCreateProgram();
   GLuint DepthObjv = glCreateShader(GL_VERTEX_SHADER);
   GLuint DepthObjf = glCreateShader(GL_FRAGMENT_SHADER);
   const GLchar* depthpvs[1];
   const GLchar* depthpfs[1];
   depthpvs[0] = pDepthVertexShaderText;
   depthpfs[0] = pDepthFragmentShaderText;
   GLint DepthLenghtsVertex[1];
   GLint DepthLenghtsFragment[1];
   DepthLenghtsVertex[0] = strlen(pDepthVertexShaderText);
   DepthLenghtsFragment[0] = strlen(pDepthFragmentShaderText);
   glShaderSource(DepthObjv, 1, depthpvs, DepthLenghtsVertex);
   glShaderSource(DepthObjf, 1, depthpfs, DepthLenghtsFragment);
   glCompileShader(DepthObjv);
   glCompileShader(DepthObjf);
   glGetShaderiv(DepthObjv, GL_COMPILE_STATUS, &successvs);
   if(!successvs){
       glGetShaderInfoLog(DepthObjv, sizeof(InfoLog1), NULL, InfoLog1);
       fprintf(stderr, "Error compiling depth shader type vertex: '%s'\n", InfoLog1);
       return 1;
   }
   glGetShaderiv(DepthObjf, GL_COMPILE_STATUS, &successfs);
   if(!successfs){
       glGetShaderInfoLog(DepthObjf, sizeof(InfoLog2), NULL, InfoLog2);
       fprintf(stderr, "Error compiling depth shader type fragment: '%s'\n", InfoLog2);
       return 1;
   }
   glAttachShader(DepthProgram, DepthObjv);
   glAttachShader(DepthProgram, DepthObjf);
   glLinkProgram(DepthProgram);
   glGetProgramiv(DepthProgram, GL_LINK_STATUS, &success);
   if(!success){
      glGetProgramInfoLog(DepthProgram, sizeof(InfoLog3), NULL, InfoLog3);
      fprintf(stderr, "Error linking depth shader program: '%s'\n", InfoLog3);
      return 1;
   }
   GLuint LightProgram = glCreateProgram();
   GLuint LightObjv = glCreateShader(GL_VERTEX_SHADER);
   GLuint LightObjf = glCreateShader(GL_FRAGMENT_SHADER);
   const GLchar* lightpvs[1];
   const GLchar* lightpfs[1];
   lightpvs[0] = pLightVertexShaderText;
   lightpfs[0] = pLightFragmentShaderText;
   GLint LightLenghtsVertex[1];
   GLint LightLenghtsFragment[1];
   LightLenghtsVertex[0] = strlen(pLightVertexShaderText);
   LightLenghtsFragment[0] = strlen(pLightFragmentShaderText);
   glShaderSource(LightObjv, 1, lightpvs, LightLenghtsVertex);
   glShaderSource(LightObjf, 1, lightpfs, LightLenghtsFragment);
   glCompileShader(LightObjv);
   glCompileShader(LightObjf);
   glGetShaderiv(LightObjv, GL_COMPILE_STATUS, &successvs);
   if(!successvs){
       glGetShaderInfoLog(LightObjv, sizeof(InfoLog1), NULL, InfoLog1);
       fprintf(stderr, "Error compiling light shader type vertex: '%s'\n", InfoLog1);
       return 1;
   }
   glGetShaderiv(LightObjf, GL_COMPILE_STATUS, &successfs);
   if(!successfs){
       glGetShaderInfoLog(LightObjf, sizeof(InfoLog2), NULL, InfoLog2);
       fprintf(stderr, "Error compiling light shader type fragment: '%s'\n", InfoLog2);
       return 1;
   }
   glAttachShader(LightProgram, LightObjv);
   glAttachShader(LightProgram, LightObjf);
   glLinkProgram(LightProgram);
   glGetProgramiv(LightProgram, GL_LINK_STATUS, &success);
   if(!success){
      glGetProgramInfoLog(LightProgram, sizeof(InfoLog3), NULL, InfoLog3);
      fprintf(stderr, "Error linking light shader program: '%s'\n", InfoLog3);
      return 1;
   }
   vec3 camerapos = {0.0f, 0.0f, 0.0f};
   vec3 center = {0.0f, 0.0f, 0.0f};
   vec3 target = {0.0f, 0.0f, 0.0f};
   vec3 front = {0.0f, 0.0f, -1.0f};
   vec3 yup = {0.0f, 1.0f, 0.0f};
   vec3 speed = {2.5f, 2.5f, 2.5f};
   vec3 lightdir = {4.0f, -7.0f, -7.0f};
   vec3 location;
   vec3 output;
   vec3 output2;
   vec3 output3; 
   vec3 camspeed;
   vec3 direction;
   vec3 directionfront;
   vec3 scaledlightdir;
   vec3 invlightdir;
   glm_vec3_negate_to(lightdir, invlightdir);
   float lightvert[] = {
      //Front
      invlightdir[0] - 0.3f, invlightdir[1] - 0.3f, invlightdir[2] + 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] + 0.3f, invlightdir[2] + 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] + 0.3f, invlightdir[2] + 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] - 0.3f, invlightdir[2] + 0.3f,

      //Back
      invlightdir[0] + 0.3f, invlightdir[1] - 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] + 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] + 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] - 0.3f, invlightdir[2] - 0.3f,

      //Left
      invlightdir[0] - 0.3f, invlightdir[1] - 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] + 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] + 0.3f, invlightdir[2] + 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] - 0.3f, invlightdir[2] + 0.3f,

      //Right
      invlightdir[0] + 0.3f, invlightdir[1] - 0.3f, invlightdir[2] + 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] + 0.3f, invlightdir[2] + 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] + 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] - 0.3f, invlightdir[2] - 0.3f,

      //Top
      invlightdir[0] - 0.3f, invlightdir[1] + 0.3f, invlightdir[2] + 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] + 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] + 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] + 0.3f, invlightdir[2] + 0.3f,

      //Bottom
      invlightdir[0] + 0.3f, invlightdir[1] - 0.3f, invlightdir[2] + 0.3f,
      invlightdir[0] + 0.3f, invlightdir[1] - 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] - 0.3f, invlightdir[2] - 0.3f,
      invlightdir[0] - 0.3f, invlightdir[1] - 0.3f, invlightdir[2] + 0.3f,
   };
   float deltatime = 0.0f;
   float currentframe = glfwGetTime();
   float lastframe = currentframe;
   direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
   direction[1] = sin(glm_rad(pitch));
   direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
   directionfront[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
   directionfront[1] = 0.0f;
   directionfront[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
   glm_normalize(direction);
   glm_normalize(directionfront);
   glm_vec3_copy(direction, front);
   glm_vec3_add(camerapos, front, location);
   GLenum err;
   mat4 model = GLM_MAT4_IDENTITY_INIT;
   mat4 view = GLM_MAT4_IDENTITY_INIT;
   mat4 proj = GLM_MAT4_IDENTITY_INIT;
   mat4 floormodel = GLM_MAT4_IDENTITY_INIT;
   mat4 sunmodel = GLM_MAT4_IDENTITY_INIT;
   mat4 identity = GLM_MAT4_IDENTITY_INIT;
   glm_lookat(camerapos, center, yup, view);
   glm_perspective(glm_rad(90.0f), (float)width/(float)height, 0.001f, 100.0f, proj);
   //glm_ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.001f, 100.0f, proj);
   glm_scale(model, (vec3){10.0f, 1.0f, 10.0f});
   glm_vec3_scale(invlightdir, 2.0f, scaledlightdir);
   glm_translate(sunmodel, scaledlightdir);
   glm_translate(model, (vec3){0.0f, -1.0f, 0.0f});
   glEnable(GL_MULTISAMPLE);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glFrontFace(GL_CCW);
   GLint uniColor = glGetUniformLocation(ShaderProgram, "color");
   GLint tex0uni = glGetUniformLocation(ShaderProgram, "tex0");
   GLint shadowuni = glGetUniformLocation(ShaderProgram, "shadowmap");
   GLint modelloc = glGetUniformLocation(ShaderProgram, "model");
   GLint depthmodelloc = glGetUniformLocation(DepthProgram, "model");
   GLint viewloc = glGetUniformLocation(ShaderProgram, "view");
   GLint projloc = glGetUniformLocation(ShaderProgram, "proj");
   GLint lightmodelloc = glGetUniformLocation(LightProgram, "model");
   GLint lightviewloc = glGetUniformLocation(LightProgram, "view");
   GLint lightprojloc = glGetUniformLocation(LightProgram, "proj");
   GLint lightDir = glGetUniformLocation(ShaderProgram, "inlightdir");
   GLint lightloc = glGetUniformLocation(ShaderProgram, "light");
   GLint depthlightloc = glGetUniformLocation(DepthProgram, "light");
   GLint textureorcolor = glGetUniformLocation(ShaderProgram, "textureorcolor");
   GLint shadowloc = glGetUniformLocation(ShaderProgram, "shadows");
   GLint lightingloc = glGetUniformLocation(ShaderProgram, "lighting");
   GLint normalmaploc = glGetUniformLocation(ShaderProgram, "normalmap");
   GLint normalmapuni = glGetUniformLocation(ShaderProgram, "normalmap0");
   glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
   glUniform1i(tex0uni, 1);  
   glUniform1i(normalmapuni, 2);
   glUniform3f(lightDir, lightdir[0], lightdir[1], lightdir[2]);
   glUniform1f(textureorcolor, 1.0f);
   glUniform1f(shadowloc, 1.0f);
   glUniform1f(lightingloc, 1.0f);
   glUniform1f(normalmaploc, 0.0f);
   glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
   glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
   glUniformMatrix4fv(projloc, 1, GL_FALSE, &proj[0][0]);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   GLuint VBO[3];
   GLuint VAO[3];
   GLuint EBO[3];
   GLuint positionbuffer[2];
   GLuint texturebuffer[2];
   GLuint normalbuffer[2];
   GLuint tangentbuffer;
   GLuint bitangentbuffer;
   GLuint colorbuffer[2];
   glGenBuffers(3, VBO);
   glGenBuffers(2, positionbuffer);
   glGenBuffers(2, texturebuffer);
   glGenBuffers(2, normalbuffer);
   glGenBuffers(1, &tangentbuffer);
   glGenBuffers(1, &bitangentbuffer);
   glGenBuffers(2, colorbuffer);
   glGenVertexArrays(3, VAO);
   glGenBuffers(3, EBO);
   glBindVertexArray(VAO[0]);
   //glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
   printf("Buffer created successfully\n");
   /*
   glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   */
   glBindBuffer(GL_ARRAY_BUFFER, positionbuffer[0]);
   glBufferData(GL_ARRAY_BUFFER, verticessize, vertex, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, texturebuffer[0]);
   glBufferData(GL_ARRAY_BUFFER, texturecoordsize, texturecoords, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[0]);
   glBufferData(GL_ARRAY_BUFFER, normalssize, normals, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
   glBufferData(GL_ARRAY_BUFFER, tangentssize, tangents, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
   glBufferData(GL_ARRAY_BUFFER, bitangentssize, bitangents, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[0]);
   glBufferData(GL_ARRAY_BUFFER, colorssize, colors, GL_STATIC_DRAW);
   printf("Buffer data set up successfully\n");
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   glEnableVertexAttribArray(3);
   glEnableVertexAttribArray(4);
   glEnableVertexAttribArray(5);
   printf("Enabling buffer successfull\n");
   GLint positionattriblocation = glGetAttribLocation(ShaderProgram, "Position");
   GLint colorattriblocation = glGetAttribLocation(ShaderProgram, "incolor");
   GLint textureattriblocation = glGetAttribLocation(ShaderProgram, "Tex");
   GLint normalattriblocation = glGetAttribLocation(ShaderProgram, "innormal");
   GLint tangentattriblocation = glGetAttribLocation(ShaderProgram, "tangent");
   GLint bitangentattriblocation = glGetAttribLocation(ShaderProgram, "bitangent");
   GLint lightpositionattriblocation = glGetAttribLocation(LightProgram, "Position");
   printf("Texture set successfully\n");
   glBindBuffer(GL_ARRAY_BUFFER, positionbuffer[0]);
   glVertexAttribPointer(positionattriblocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   printf("First pointer succeeded\n");
   glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[0]);
   glVertexAttribPointer(colorattriblocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   printf("Second pointer succeeded\n");
   glBindBuffer(GL_ARRAY_BUFFER, texturebuffer[0]);
   glVertexAttribPointer(textureattriblocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
   printf("Third pointer succeeded\n");
   glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[0]);
   glVertexAttribPointer(normalattriblocation, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
   printf("Forth pointer succeeded\n");
   //glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
   //glVertexAttribPointer(tangentattriblocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   printf("Fifth pointer succeeded\n");
   glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
   glVertexAttribPointer(bitangentattriblocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   printf("Sixth pointer succeeded\n");
   printf("Setting up VertexAttribPointer successfull\n");
   glBindVertexArray(VAO[1]);
   glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorindices), floorindices, GL_STATIC_DRAW);
   printf("Buffer data set up successfully\n");
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   glEnableVertexAttribArray(3);
   glEnableVertexAttribArray(4);
   glEnableVertexAttribArray(5);
   printf("Enabling buffer successfull\n");
   glVertexAttribPointer(positionattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
   printf("First pointer succeeded\n");
   glVertexAttribPointer(colorattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
   printf("Second pointer succeeded\n");
   glVertexAttribPointer(textureattriblocation, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
   printf("Third pointer succeeded\n");
   glVertexAttribPointer(normalattriblocation, 3, GL_FLOAT, GL_TRUE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
   printf("Forth pointer succeeded\n");
   //glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
   //glVertexAttribPointer(tangentattriblocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   printf("Fifth pointer succeeded\n");
   glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
   glVertexAttribPointer(bitangentattriblocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   printf("Sixth pointer succeeded\n");
   printf("Setting up VectexAttribPointer successfull\n");
   glBindVertexArray(VAO[2]);
   glBindBuffer(GL_ARRAY_BUFFER, positionbuffer[1]);
   glBufferData(GL_ARRAY_BUFFER, sunverticessize, sunvertex, GL_STATIC_DRAW);
   //printf("Buffer data set up successfully\n");
   glEnableVertexAttribArray(0);
   //printf("Enabling buffer successfull\n");
   glBindBuffer(GL_ARRAY_BUFFER, positionbuffer[1]);
   glVertexAttribPointer(lightpositionattriblocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   /*
   printf("First pointer succeeded\n");
   //glVertexAttribPointer(colorattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
   printf("Second pointer succeeded\n");
   //glVertexAttribPointer(textureattriblocation, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
   printf("Third pointer succeeded\n");
   //glVertexAttribPointer(normalattriblocation, 3, GL_FLOAT, GL_TRUE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
   printf("Forth pointer succeeded\n");
   printf("Setting up VectexAttribPointer successfull\n");
   */
   GLuint FBO;
   glGenFramebuffers(1, &FBO);
   GLuint depthTexture;
   glGenTextures(1, &depthTexture);
   glBindTexture(GL_TEXTURE_2D, depthTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 8192, 8192, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   float clampcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampcolor);
   glBindFramebuffer(GL_FRAMEBUFFER, FBO);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   mat4 lightproj = GLM_MAT4_IDENTITY_INIT;
   mat4 lightview = GLM_MAT4_IDENTITY_INIT;
   mat4 light = GLM_MAT4_IDENTITY_INIT;
   glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f, lightproj);
   //glm_perspective(glm_rad(45.0f), 1.0f, 0.1f, 50.0f, lightproj);
   //glm_vec3_scale(lightdir, 0.1f, scaledlightdir);
   glm_lookat(invlightdir, center, yup, lightview);
   glm_mat4_mul(lightproj, lightview, light);
   glUseProgram(DepthProgram);
   glUniformMatrix4fv(depthlightloc, 1, GL_FALSE, &light[0][0]);
   glUniformMatrix4fv(depthmodelloc, 1, GL_FALSE, &model[0][0]);
   glUseProgram(ShaderProgram);
   glUniformMatrix4fv(lightloc, 1, GL_FALSE, &light[0][0]);
   glUniform1i(shadowuni, 0); 

   int texwidth, texheight, texnum;
   stbi_set_flip_vertically_on_load(true);
   unsigned char* bytes = stbi_load("wood-floor.jpg", &texwidth, &texheight, &texnum, 0);
   if(bytes == NULL){
      printf("Failed to load the texture!\n");
      //return 1;
   }
   GLuint texture;
   glGenTextures(1, &texture);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   float flatcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatcolor);
   printf("Set parameters successfully\n");
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
   glGenerateMipmap(GL_TEXTURE_2D);
   stbi_image_free(bytes);
   printf("Texture set successfully\n");
   stbi_set_flip_vertically_on_load(true);
   unsigned char* normalbytes = stbi_load("wood-floor_normal.jpg", &texwidth, &texheight, &texnum, 0);
   if(normalbytes == NULL){
      printf("Failed to load the normal texture!\n");
   }
   GLuint normaltexture;
   glGenTextures(1, &normaltexture);
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, normaltexture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   float normalflatcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, normalflatcolor);
   printf("Set parameters successfully\n");
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, normalbytes);
   glGenerateMipmap(GL_TEXTURE_2D);
   stbi_image_free(normalbytes);
   printf("Texture set successfully\n");

   while(!glfwWindowShouldClose(window)){
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
      if((err = glGetError()) != GL_NO_ERROR){
         printf("OpenGL error: %d\n", err);
      }
      currentframe = glfwGetTime();
      deltatime = currentframe - lastframe;
      lastframe = currentframe;
      //printf("FPS: %f\n", 1.0f / deltatime);
      //printf("direction: %f, %f, %f; directionfront: %f, %f, %f\n", direction[0], direction[1], direction[2], directionfront[0], directionfront[1], directionfront[2]);
      //glm_vec3_scale(speed, deltatime, camspeed);
      //glm_rotate(model, glm_rad(degreesx), (vec3){0, 1, 0});
      //glm_rotate(model, glm_rad(degreesy), (vec3){1, 0, 0});
      //glm_rotate(floormodel, glm_rad(-degreesy * deltatime * 20), (vec3){0, 1, 0});
      //glm_rotate(model, glm_rad(degreesy), (vec3){0, 1, 0});
      //glm_rotate(floormodel, glm_rad(degreesy), (vec3){0, 1, 0});
      //glm_translate_x(model, -1.0f * deltatime);
      if(speedincrease == 1 && slow == 0){
         speed[0] = 8.0f;
	 speed[1] = 8.0f;
	 speed[2] = 8.0f;
      }else if(speedincrease == 0 && slow == 1){
         speed[0] = 0.1f;
         speed[1] = 0.1f;
         speed[2] = 0.1f;
      }else if(speedincrease == 1 && slow == 1 || speedincrease == 0 && slow == 0){
         speed[0] = 2.5f;
	 speed[1] = 2.5f;
	 speed[2] = 2.5f;
      }
      glm_vec3_scale(speed, deltatime, camspeed);
      if(zoom == 1 && zoomedin == 0){
         glm_perspective(glm_rad(30.0f), (float)width/(float)height, 0.001f, 100.0f, proj);
	 zoomedin = 1;
      }else if(zoom == 0 && zoomedin == 1){
         glm_perspective(glm_rad(90.0f), (float)width/(float)height, 0.001f, 100.0f, proj);
	 zoomedin = 0;
      }
      if(checkfront == 1){
         glm_vec3_mul(camspeed, directionfront, output);
         glm_vec3_add(camerapos, output, camerapos);
	 //glm_vec3_add(camerapos, directionfront, location);
      }
      if(checkback == 1){
         glm_vec3_mul(camspeed, directionfront, output);
         glm_vec3_sub(camerapos, output, camerapos);
	 //glm_vec3_add(camerapos, directionfront, location);
      }
      if(checkleft == 1){
         glm_vec3_crossn(directionfront, yup, output2);
	 glm_vec3_mul(output2, camspeed, output3);
	 glm_vec3_sub(camerapos, output3, camerapos);
	 //glm_vec3_add(camerapos, directionfront, location);
      }
      if(checkright == 1){
         glm_vec3_crossn(directionfront, yup, output2);
	 glm_vec3_mul(output2, camspeed, output3);
	 glm_vec3_add(camerapos, output3, camerapos);
	 //glm_vec3_add(camerapos, directionfront, location);
      }
      //jumppressed = 1;
      if(jumppressed == 1 && jump == 0){
         jump = 1;
      }
      /*
      if(jump == 1){
         glm_translate_y(model, jumpacceleration * deltatime);
	 jumpacceleration += gravity;
	 if(model[3][1] <= 0.0f){
            glm_translate_y(model, -model[3][1]);
	    jumpacceleration = 3.0f;
	    jump = 0;
	 }
      }
      */
      if(jump == 1 && falling == 0){
         camerapos[1] += jumpacceleration * deltatime;
         jumpacceleration += gravity;
	 if(camerapos[1] <= 0.0f){
            camerapos[1] = 0.0f;
	    jumpacceleration = 5.0f;
	    jump = 0;
	 }
      }
      if((camerapos[0] - 0.5f > 10.0f || camerapos[0] + 0.5f < -10.0f || camerapos[2] - 0.5f > 10.0f || camerapos[2] + 0.5f < -10.0f) && jump == 0){
         //falling = 1;
      }
      if(falling == 1 && camerapos[1] < -5.0f){
         camerapos[0] = 0.0f;
         camerapos[1] = 0.0f;
	 camerapos[2] = 0.0f;
	 yaw = -90.0f;
	 pitch = 0.0f;
	 falling = 0;
	 jump = 0;
      }
      if(falling == 1){
         camerapos[1] += fallinggravity * deltatime;
      }
      if(kill == 1){
         break;
      }
      //glm_rotate(model, glm_rad(0.1f), (vec3){0, 1, 0});
      direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
      direction[1] = sin(glm_rad(pitch));
      direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
      directionfront[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
      directionfront[1] = 0.0f;
      directionfront[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
      glm_normalize(direction);
      glm_normalize(directionfront);
      glm_vec3_copy(direction, front);
      glm_vec3_add(camerapos, front, location);
      glm_lookat(camerapos, location, yup, view);
      //printf("Yaw: %f, Pitch: %f\n", yaw, pitch);
      //glm_lookat(camerapos, center, yup, view);
      //printf("%d/%d/%d/%d\n", positionattriblocation, colorattriblocation, textureattriblocation, normalattriblocation);
      glViewport(0, 0, 8192, 8192);
      glUseProgram(DepthProgram);
      glEnable(GL_CULL_FACE);
      glUniformMatrix4fv(depthmodelloc, 1, GL_FALSE, &model[0][0]);
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      glClear(GL_DEPTH_BUFFER_BIT);
      glCullFace(GL_FRONT);
      glBindVertexArray(VAO[0]);
      //glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      glDrawArrays(GL_TRIANGLES, 0, verticessize / sizeof(float) / 3);
      glUniformMatrix4fv(depthmodelloc, 1, GL_FALSE, &floormodel[0][0]);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
      /*
      glBindVertexArray(VAO[1]);
      glDrawElements(GL_TRIANGLES, sizeof(floorindices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      */
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, width, height);
      glCullFace(GL_BACK);
      glUseProgram(ShaderProgram);
      glDisable(GL_CULL_FACE);
      glUniformMatrix4fv(projloc, 1, GL_FALSE, &proj[0][0]);
      glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, depthTexture);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture);
      if(texturechange == 1 && texturestate == 1){
         glUniform1f(textureorcolor, 0.0f);
	 texturechange = 0;
	 texturestate = 0;
      }else if(texturechange == 1 && texturestate == 0){
         glUniform1f(textureorcolor, 1.0f);
	 texturechange = 0;
	 texturestate = 1;
      }
      if(shadowchange == 1 && shadowstate == 1){
         glUniform1f(shadowloc, 0.0f);
         shadowchange = 0;
         shadowstate = 0;
      }else if(shadowchange == 1 && shadowstate == 0){
         glUniform1f(shadowloc, 1.0f);
         shadowchange = 0;
         shadowstate = 1;
      }
      if(lightingchange == 1 && lightingstate == 1){
         glUniform1f(lightingloc, 0.0f);
         lightingchange = 0;
         lightingstate = 0;
      }else if(lightingchange == 1 && lightingstate == 0){
         glUniform1f(lightingloc, 1.0f);
         lightingchange = 0;
         lightingstate = 1;
      }
      if(normalmapchange == 1 && normalmapstate == 1){
         glUniform1f(normalmaploc, 0.0f);
	 normalmapchange = 0;
	 normalmapstate = 0;
      }else if(normalmapchange == 1 && normalmapstate == 0){
         glUniform1f(normalmaploc, 1.0f);
	 normalmapchange = 0;
	 normalmapstate = 1;
      }else if(normalmapchange == 0 && normalmapstate == 1){
         glUniform1f(normalmaploc, 1.0f);
      }else if(normalmapchange == 0 && normalmapstate == 0){
         glUniform1f(normalmaploc, 0.0f);
      }
      //printf("%d, %d\n", texturechange, texturestate);
      glBindVertexArray(VAO[0]);
      //glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      glDrawArrays(GL_TRIANGLES, 0, verticessize / sizeof(float) / 3);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &floormodel[0][0]);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
      //glUniform1f(textureorcolor, 0.0f);
      /*
      glBindVertexArray(VAO[1]);
      glDrawElements(GL_TRIANGLES, sizeof(floorindices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      */
      glUseProgram(LightProgram);
      glUniformMatrix4fv(lightprojloc, 1, GL_FALSE, &proj[0][0]);
      glUniformMatrix4fv(lightviewloc, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(lightmodelloc, 1, GL_FALSE, &sunmodel[0][0]);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
      glBindVertexArray(VAO[2]);
      //glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      glDrawArrays(GL_TRIANGLES, 0, sunverticessize / sizeof(float) / 3);
      glfwSwapBuffers(window);
   } 
   free(vertex);
   free(texturecoords);
   free(normals);
   return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if(key == GLFW_KEY_R && action == GLFW_PRESS){
      glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
   }else if(key == GLFW_KEY_G && action == GLFW_PRESS){
      glUniform3f(uniColor, 0.0f, 1.0f, 0.0f);
   }else if(key == GLFW_KEY_B && action == GLFW_PRESS){
      glUniform3f(uniColor, 0.0f, 0.0f, 1.0f);
   }else if(key == GLFW_KEY_W && action == GLFW_PRESS){
      //degreesx = 1.0f;
      //degreesy = 0.5f;
      checkfront = 1;
   }else if(key == GLFW_KEY_S && action == GLFW_PRESS){
      //degreesx = -1.0f;
      //degreesy = -0.5f;
      checkback = 1;
   }else if(key == GLFW_KEY_W && action == GLFW_RELEASE){
      checkfront = 0;
   }else if(key == GLFW_KEY_S && action == GLFW_RELEASE){
      checkback = 0;
   }else if(key == GLFW_KEY_A && action == GLFW_PRESS){
      checkleft = 1;
   }else if(key == GLFW_KEY_A && action == GLFW_RELEASE){
      checkleft = 0;
   }else if(key == GLFW_KEY_D && action == GLFW_PRESS){
      checkright = 1;
   }else if(key == GLFW_KEY_D && action == GLFW_RELEASE){
      checkright = 0;
   }else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
      jumppressed = 1;
   }else if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
      jumppressed = 0;
   }else if(key == GLFW_KEY_Q && action == GLFW_PRESS){
      kill = 1;
   }else if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
      speedincrease = 1;
   }else if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
      speedincrease = 0;
   }else if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS){
      slow = 1;
   }else if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE){
      slow = 0;
   }else if(key == GLFW_KEY_C && action == GLFW_PRESS){
      zoom = 1;
   }else if(key == GLFW_KEY_C && action == GLFW_RELEASE){
      zoom = 0;
   }else if(key == GLFW_KEY_T && action == GLFW_PRESS){
      texturechange = 1;
   }else if(key == GLFW_KEY_V && action == GLFW_PRESS){
      shadowchange = 1;
   }else if(key == GLFW_KEY_L && action == GLFW_PRESS){
      lightingchange = 1;
   }else if(key == GLFW_KEY_N && action == GLFW_PRESS){
      normalmapchange = 1;
   }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
   if(firstclick == 1){
      lastx = xpos;
      lasty = ypos;
      firstclick = 0;
   }
   xoffset = xpos - lastx;
   yoffset = lasty - ypos;
   lastx = xpos;
   lasty = ypos;
   xoffset *= sensitivity;
   yoffset *= sensitivity;
   yaw += xoffset;
   pitch += yoffset;
   if(pitch > 89.0f){
      pitch = 89.0f;
   }
   if(pitch < -89.0f){
      pitch = -89.0f;
   }
}
