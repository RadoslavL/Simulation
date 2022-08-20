#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cglm/cglm.h>

int load_obj(const char* filename, float* vertices, float* texcoords, float* normals, unsigned int* vertexindexsize, unsigned int* texcoordindexsize, unsigned int* normalindexsize /*int* indices, int* textureindices, int* normalsindices, unsigned int* vertexindexsize, unsigned int* texcoordindexsize, unsigned int* normalindexsize*/){
   FILE* file = fopen(filename, "r");
   char lineheader[128];
   int res;
   int i = 0;
   int f = 0;
   int d = 0;
   int g = 0;
   unsigned int vertexsize = 0;
   unsigned int texcoordsize = 0;
   unsigned int normalsize = 0;
   unsigned int verticessize = 0;
   unsigned int texturecoordsize = 0;
   unsigned int normalssize = 0;
   /*
   float verticesout[100000];
   float texcoordsout[100000];
   float normalsout[100000];
   float verticesind[100000];
   float texturecoordsind[100000];
   float normalsind[100000];
   */
   float *verticesout = malloc(sizeof(float) * 1000000);
   float *texcoordsout = malloc(sizeof(float) * 1000000);
   float *normalsout = malloc(sizeof(float) * 1000000);
   float *verticesind = malloc(sizeof(float) * 1000000);
   float *texturecoordsind = malloc(sizeof(float) * 1000000);
   float *normalsind = malloc(sizeof(float) * 1000000);
   unsigned int vertexindex[3];
   unsigned int texindex[3];
   unsigned int normalindex[3];
   float vertex[3];
   float texcoord[2];
   float normal[3];
   /*
   unsigned int vertexindices[100000];
   unsigned int texindices[100000];
   unsigned int normalindices[100000];
   */
   unsigned int *vertexindices = malloc(sizeof(unsigned int) * 1000000);
   unsigned int *texindices = malloc(sizeof(unsigned int) * 1000000);
   unsigned int *normalindices = malloc(sizeof(unsigned int) * 1000000);
   if(verticesout == NULL || texcoordsout == NULL || normalsout == NULL || verticesind == NULL || texturecoordsind == NULL || normalsind == NULL || vertexindices == NULL || texindices == NULL || normalindices == NULL){
      printf("Not enough memory! Allocation failed!\n");
      return 1;
   }
   if(file == NULL){
      printf("Failed to open file!\n");
      return 1;
   }
   while(1){
      res = fscanf(file, "%s", lineheader);
      if(res == EOF){
         break;
      }
      if(strcmp(lineheader, "v") == 0){
         fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
         verticesout[i] = vertex[0];
         verticesout[i + 1] = vertex[1];
         verticesout[i + 2] = vertex[2];
         i += 3;
      }else if(strcmp(lineheader, "vt") == 0){
         fscanf(file, "%f %f\n", &texcoord[0], &texcoord[1]);
         texcoordsout[f] = texcoord[0];
         texcoordsout[f + 1] = texcoord[1];
         f += 2;
      }else if(strcmp(lineheader, "vn") == 0){
         fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
         normalsout[d] = normal[0];
         normalsout[d + 1] = normal[1];
         normalsout[d + 2] = normal[2];
         d += 3;
      }else if(strcmp(lineheader, "f") == 0){
         fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexindex[0], &texindex[0], &normalindex[0], &vertexindex[1], &texindex[1], &normalindex[1], &vertexindex[2], &texindex[2], &normalindex[2]);
         vertexindices[g] = vertexindex[0];
         vertexindices[g + 1] = vertexindex[1];
         vertexindices[g + 2] = vertexindex[2];
	 vertexsize += 3;
         texindices[g] = texindex[0];
         texindices[g + 1] = texindex[1];
         texindices[g + 2] = texindex[2];
	 texcoordsize += 3;
         normalindices[g] = normalindex[0];
         normalindices[g + 1] = normalindex[1];
         normalindices[g + 2] = normalindex[2];
	 normalsize += 3;
         g += 3;
      }
   }
   /*
   if(vertexsize > sizeof(vertexindices)/sizeof(unsigned int)){
      printf("Vertex size exceeded!\n");
      return 1;
   }
   if(texcoordsize > sizeof(texindices)/sizeof(unsigned int)){
      printf("UV size exceeded!\n");
      return 1;
   }
   if(normalsize > sizeof(normalindices)/sizeof(unsigned int)){
      printf("Normal size exceeded!\n");
      return 1;
   }
   */
   for(i = 0; i < vertexsize; i++){
      verticesind[i * 3] = verticesout[(vertexindices[i] - 1) * 3];
      verticesind[i * 3 + 1] = verticesout[(vertexindices[i] - 1) * 3 + 1];
      verticesind[i * 3 + 2] = verticesout[(vertexindices[i] - 1) * 3 + 2];
      verticessize += sizeof(verticesind[i]) * 3;
   }
   for(i = 0; i < texcoordsize; i++){
      texturecoordsind[i * 2] = texcoordsout[(texindices[i] - 1) * 2];
      texturecoordsind[i * 2 + 1] = texcoordsout[(texindices[i] - 1) * 2 + 1];
      texturecoordsind[i * 2 + 2] = texcoordsout[(texindices[i] - 1) * 2 + 2];
      texturecoordsize += sizeof(texturecoordsind[i]) * 3;
   }
   for(i = 0; i < normalsize; i++){
      normalsind[i * 3] = normalsout[(normalindices[i] - 1) * 3];
      normalsind[i * 3 + 1] = normalsout[(normalindices[i] - 1) * 3 + 1];
      normalsind[i * 3 + 2] = normalsout[(normalindices[i] - 1) * 3 + 2];
      normalssize += sizeof(normalsind[i]) * 3;
   }
   memcpy(vertices, verticesind, verticessize);
   memcpy(texcoords, texturecoordsind, texturecoordsize);
   memcpy(normals, normalsind, normalssize);
   /*
   memcpy(indices, vertexindices, sizeof(vertexindices));
   memcpy(textureindices, texindices, sizeof(texindices));
   memcpy(normalsindices, normalindices, sizeof(normalindices));
   */
   *vertexindexsize = verticessize;
   *texcoordindexsize = texturecoordsize;
   *normalindexsize = normalssize;
   free(verticesout);
   free(texcoordsout);
   free(normalsout);
   free(verticesind);
   free(texturecoordsind);
   free(normalsind);
   return 0;
}
