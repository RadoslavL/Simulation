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

const unsigned int width = 1366;
const unsigned int height = 768;
const float sensitivity = 0.1f;
float degreesx = 1.0f;
float degreesy = 0.5f;
float lastx = (float)width/2;
float lasty = (float)height/2;
float xoffset;
float yoffset;
float yaw = 450.0f;
float pitch = 0.0f;
float jumpacceleration = 3.0f;
float gravity = -0.1f;
GLuint checkfront = 0;
GLuint checkback = 0;
GLuint checkleft = 0;
GLuint checkright = 0;
GLuint jumppressed = 0;
GLuint jump = 0;
GLuint kill = 0;
GLuint firstclick = 1;
GLuint speedincrease = 0;
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
   char pVertexShaderText[1000] = {'\0'};
   char pFragmentShaderText[1000] = {'\0'};
   char pDepthVertexShaderText[1000] = {'\0'};
   char pDepthFragmentShaderText[1000] = {'\0'};
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
   if(!glfwInit()){
      printf("Failed to initialize glfw!\n");
      return 1;
   }else{
      printf("Successfully initialized glfw!\n");
   }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
     -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, //0
     -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f, //1
      0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f, //2
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, //3
     
    //Back
      0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f, //4
      0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, //5
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, //6
     -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f, //7

    //Right side
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, //8
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, //9
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, //10
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, //11

    //Left side
     -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, //12
     -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, //13
     -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, //14
     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, //15

    //Top
     -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f, //16
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f, //17
      0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, //18
      0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, //19

    //Bottom
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, //20
     -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, //21
     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f, //22
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f, //23
   };

   float floor[] = {
     -2.0f, -0.5f, -2.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f, //0
     -2.0f, -0.5f,  2.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f, //1
      2.0f, -0.5f,  2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f, 0.0f, //2
      2.0f, -0.5f, -2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f, 0.0f //3
   };

   unsigned int indices[] = {
    //Front
      0, 1, 2,
      0, 2, 3,
    //Back
      4, 5, 6,
      4, 6, 7,
    //Right side
      8, 9, 10,
      8, 10, 11,
    //Left side
      12, 13, 14,
      12, 14, 15,
    //Top
      16, 17, 18,
      16, 18, 19,
    //Bottom
      20, 21, 22,
      20, 22, 23
   };

   unsigned int floorindices[] = {
      0, 1, 2,
      0, 2, 3
   };

   printf("GLfloat variable called successfully\n"); 
   GLuint ShaderProgram = glCreateProgram();
   GLuint ShaderObjv = glCreateShader(GL_VERTEX_SHADER);
   GLuint ShaderObjf = glCreateShader(GL_FRAGMENT_SHADER);
   const GLchar* pvs[1];
   const GLchar* pfs[1];
   /*
   const char* pVertexShaderText = "#version 110\n"
   "attribute vec3 Position;\n"
   "attribute vec3 incolor;\n"
   "attribute vec2 Tex;\n"
   "attribute vec3 innormal;\n"
   "varying vec3 outcolor;\n"
   "varying vec2 texcoords;\n"
   "varying vec3 outnormal;\n"
   "varying vec3 FragPos;\n"
   "uniform mat4 model;\n"
   "uniform mat4 view;\n"
   "uniform mat4 proj;\n"
   "void main(){\n"
   "   gl_Position = proj * view * model * vec4(Position, 1.0);\n"
   "   FragPos = vec3(model * vec4(Position, 1.0));\n"
   "   outcolor = incolor;\n"
   "   texcoords = Tex;\n"
   "   outnormal = (model * vec4(innormal, 0.0)).xyz;\n"
   "}";
   const char* pFragmentShaderText = "#version 110\n"
   "uniform vec3 color;\n"
   "uniform sampler2D tex0;\n"
   "uniform vec3 lightPos;\n"
   "varying vec3 outcolor;\n"
   "varying vec2 texcoords;\n"
   "varying vec3 outnormal;\n"
   "varying vec3 FragPos;\n"
   "void main(){\n"
   "   float ambientstrength = 0.1;\n"
   "   vec3 ambient = ambientstrength * vec3(1.0, 1.0, 1.0);\n"
   "   vec3 norm = normalize(outnormal);\n"
   "   vec3 lightDir = normalize(lightPos - FragPos);\n"
   "   float diff = max(dot(norm, lightDir), 0.0);\n"
   "   vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);\n"
   "   vec4 result = vec4(ambient + diffuse, 1.0) * vec4(outcolor, 1.0);\n"
   "   gl_FragColor = result;\n"
   "}";
   */
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
       glGetShaderInfoLog(ShaderObjv, sizeof(InfoLog1), NULL, InfoLog1);
       fprintf(stderr, "Error compiling depth shader type vertex: '%s'\n", InfoLog1);
       return 1;
   }
   glGetShaderiv(DepthObjf, GL_COMPILE_STATUS, &successfs);
   if(!successfs){
       glGetShaderInfoLog(ShaderObjf, sizeof(InfoLog2), NULL, InfoLog2);
       fprintf(stderr, "Error compiling depth shader type fragment: '%s'\n", InfoLog2);
       return 1;
   }
   glAttachShader(DepthProgram, DepthObjv);
   glAttachShader(DepthProgram, DepthObjf);
   glLinkProgram(DepthProgram);
   glGetProgramiv(DepthProgram, GL_LINK_STATUS, &success);
   if(!success){
      glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog3), NULL, InfoLog3);
      fprintf(stderr, "Error linking shader program: '%s'\n", InfoLog3);
      return 1;
   }
   vec3 camerapos = {0.0f, 0.0f, -7.0f};
   vec3 center = {0.0f, 0.0f, 0.0f};
   vec3 target = {0.0f, 0.0f, 0.0f};
   vec3 front = {0.0f, 0.0f, 1.0f};
   vec3 yup = {0.0f, 1.0f, 0.0f};
   vec3 speed = {2.5f, 2.5f, 2.5f};
   vec3 lightpos = {-1.0f, 1.0f, -7.0f};
   vec3 location;
   vec3 output;
   vec3 output2;
   vec3 output3; 
   vec3 camspeed;
   vec3 direction;
   vec3 directionfront;
   vec3 scaledlightpos;
   float deltatime = 0.0f;
   float currentframe = glfwGetTime();
   float lastframe = currentframe;
   direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
   direction[1] = sin(glm_rad(pitch));
   direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
   directionfront[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
   //directionfront[1] = sin(glm_rad(pitch));
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
   mat4 identity = GLM_MAT4_IDENTITY_INIT;
   glm_lookat(camerapos, center, yup, view);
   glm_perspective(glm_rad(45.0f), (float)width/(float)height, 0.1f, 100.0f, proj);
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
   GLint lightPos = glGetUniformLocation(ShaderProgram, "lightPos");
   GLint lightloc = glGetUniformLocation(ShaderProgram, "light");
   GLint depthlightloc = glGetUniformLocation(DepthProgram, "light");
   glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
   glUniform1i(tex0uni, 0);  
   glUniform3f(lightPos, lightpos[0], lightpos[1], lightpos[2]);
   glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
   glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
   glUniformMatrix4fv(projloc, 1, GL_FALSE, &proj[0][0]);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   GLuint VBO[2];
   GLuint VAO[2];
   GLuint EBO[2];
   glGenBuffers(2, VBO);
   glGenVertexArrays(2, VAO);
   glGenBuffers(2, EBO);
   glBindVertexArray(VAO[0]);
   glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
   printf("Buffer created successfully\n");
   glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   printf("Buffer data set up successfully\n");
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   glEnableVertexAttribArray(3);
   printf("Enabling buffer successfull\n");
   GLint positionattriblocation = glGetAttribLocation(ShaderProgram, "Position");
   GLint colorattriblocation = glGetAttribLocation(ShaderProgram, "incolor");
   GLint textureattriblocation = glGetAttribLocation(ShaderProgram, "Tex");
   GLint normalattriblocation = glGetAttribLocation(ShaderProgram, "innormal");
   printf("Texture set successfully\n");
   glVertexAttribPointer(positionattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
   printf("First pointer succeeded\n");
   glVertexAttribPointer(colorattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
   printf("Second pointer succeeded\n");
   //glVertexAttribPointer(textureattriblocation, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
   printf("Third pointer succeeded\n");
   glVertexAttribPointer(normalattriblocation, 3, GL_FLOAT, GL_TRUE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
   printf("Forth pointer succeeded\n");
   printf("Setting up VectexAttribPointer successfull\n");
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
   printf("Enabling buffer successfull\n");
   glVertexAttribPointer(positionattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
   printf("First pointer succeeded\n");
   glVertexAttribPointer(colorattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
   printf("Second pointer succeeded\n");
   //glVertexAttribPointer(textureattriblocation, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
   printf("Third pointer succeeded\n");
   glVertexAttribPointer(normalattriblocation, 3, GL_FLOAT, GL_TRUE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
   printf("Forth pointer succeeded\n");
   printf("Setting up VectexAttribPointer successfull\n");
   GLuint FBO;
   glGenFramebuffers(1, &FBO);
   GLuint depthTexture;
   glGenTextures(1, &depthTexture);
   glBindTexture(GL_TEXTURE_2D, depthTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1020, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
   glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 75.0f, lightproj);
   glm_vec3_scale(lightpos, 2.0f, scaledlightpos);
   glm_lookat(scaledlightpos, center, yup, lightview);
   glm_mat4_mul(lightproj, lightview, light);
   glUseProgram(DepthProgram);
   glUniformMatrix4fv(depthlightloc, 1, GL_FALSE, &light[0][0]);
   glUniformMatrix4fv(depthmodelloc, 1, GL_FALSE, &model[0][0]);
   glUseProgram(ShaderProgram);
   glUniformMatrix4fv(lightloc, 1, GL_FALSE, &light[0][0]);
   glUniform1i(shadowuni, 0); 
   int texwidth, texheight, texnum;
   stbi_set_flip_vertically_on_load(true);
   unsigned char* bytes = stbi_load("crate.jpg", &texwidth, &texheight, &texnum, 0);
   if(bytes == NULL){
      printf("Failed to load the texture!\n");
      //return 1;
   } 
   /*
   GLuint texture;
   glGenTextures(1, &texture);
   glActiveTexture(GL_TEXTURE0);
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
   printf("Texture set successfully\n");
   */

   while(!glfwWindowShouldClose(window)){
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
      if((err = glGetError()) != GL_NO_ERROR){
         printf("OpenGL error: %d\n", err);
      }
      currentframe = glfwGetTime();
      deltatime = currentframe - lastframe;
      lastframe = currentframe;
      glm_vec3_scale(speed, deltatime, camspeed);
      //glm_rotate(model, glm_rad(degreesx), (vec3){0, 1, 0});
      //glm_rotate(model, glm_rad(degreesy), (vec3){1, 0, 0});
      //glm_rotate(floormodel, glm_rad(-degreesy * deltatime * 20), (vec3){0, 1, 0});
      //glm_rotate(model, glm_rad(degreesy), (vec3){0, 1, 0});
      //glm_rotate(floormodel, glm_rad(degreesy), (vec3){0, 1, 0});
      if(speedincrease == 1){
         speed[0] = 8.0f;
	 speed[1] = 8.0f;
	 speed[2] = 8.0f;
      }else{
         speed[0] = 2.5f;
	 speed[1] = 2.5f;
	 speed[2] = 2.5f;
      }
      if(checkfront == 1){
         glm_vec3_mul(camspeed, front, output);
         glm_vec3_add(camerapos, output, camerapos);
	 glm_vec3_add(camerapos, front, location);
      }
      if(checkback == 1){
         glm_vec3_mul(camspeed, front, output);
         glm_vec3_sub(camerapos, output, camerapos);
	 glm_vec3_add(camerapos, front, location);
      }
      if(checkleft == 1){
         glm_vec3_crossn(front, yup, output2);
	 glm_vec3_mul(output2, camspeed, output3);
	 glm_vec3_sub(camerapos, output3, camerapos);
	 glm_vec3_add(camerapos, front, location);
      }
      if(checkright == 1){
         glm_vec3_crossn(front, yup, output2);
	 glm_vec3_mul(output2, camspeed, output3);
	 glm_vec3_add(camerapos, output3, camerapos);
	 glm_vec3_add(camerapos, front, location);
      }
      jumppressed = 1;
      if(jumppressed == 1 && jump == 0){
         jump = 1;
      }
      if(jump == 1){
         glm_translate_y(model, jumpacceleration * deltatime);
	 jumpacceleration += gravity;
	 if(model[3][1] <= 0.0f){
            glm_translate_y(model, -model[3][1]);
	    jumpacceleration = 3.0f;
	    jump = 0;
	 }
      }
      if(kill == 1){
         break;
      }
      direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
      direction[1] = sin(glm_rad(pitch));
      direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
      directionfront[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
      //directionfront[1] = sin(glm_rad(pitch));
      directionfront[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
      glm_normalize(direction);
      glm_vec3_copy(direction, front);
      glm_vec3_add(camerapos, front, location);
      glm_lookat(camerapos, location, yup, view);
      //printf("Yaw: %f, Pitch: %f\n", yaw, pitch);
      //glm_lookat(camerapos, center, yup, view);
      //printf("%d/%d/%d/%d\n", positionattriblocation, colorattriblocation, textureattriblocation, normalattriblocation);
      glViewport(0, 0, 1024, 1024);
      glUseProgram(DepthProgram);
      glUniformMatrix4fv(depthmodelloc, 1, GL_FALSE, &model[0][0]);
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      glClear(GL_DEPTH_BUFFER_BIT);
      glBindVertexArray(VAO[0]);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      glUniformMatrix4fv(depthmodelloc, 1, GL_FALSE, &floormodel[0][0]);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
      glBindVertexArray(VAO[1]);
      glDrawElements(GL_TRIANGLES, sizeof(floorindices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, width, height);
      glUseProgram(ShaderProgram);
      glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
      glBindVertexArray(VAO[0]);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, depthTexture);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &floormodel[0][0]);
      //glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
      glBindVertexArray(VAO[1]);
      glDrawElements(GL_TRIANGLES, sizeof(floorindices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
      glfwSwapBuffers(window);
   } 
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
