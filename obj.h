#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"pointers.h"

int load_obj(const char* filename, float* vertices, float* texcoords, float* normals, float* tangentsout, float* bitangentsout, unsigned int* vertexindexsize, unsigned int* texcoordindexsize, unsigned int* normalindexsize, unsigned int* tangentindexsize, unsigned int* bitangentindexsize /*int* indices, int* textureindices, int* normalsindices, unsigned int* vertexindexsize, unsigned int* texcoordindexsize, unsigned int* normalindexsize*/){
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
   unsigned int tangentssize = 0;
   unsigned int bitangentssize = 0;
   /*
   float verticesout[100000];
   float texcoordsout[100000];
   float normalsout[100000];
   float verticesind[100000];
   float texturecoordsind[100000];
   float normalsind[100000];
   */
   float *verticesout = malloc(sizeof(float) * 5000000);
   float *texcoordsout = malloc(sizeof(float) * 5000000);
   float *normalsout = malloc(sizeof(float) * 5000000);
   float *verticesind = malloc(sizeof(float) * 5000000);
   float *texturecoordsind = malloc(sizeof(float) * 5000000);
   float *normalsind = malloc(sizeof(float) * 5000000);
   float *tangents = malloc(sizeof(float) * 5000000);
   float *bitangents = malloc(sizeof(float) * 5000000);
   unsigned int vertexindex[3];
   unsigned int texindex[3];
   unsigned int normalindex[3];
   float vertex[3];
   float texcoord[2];
   float normal[3];
   float tangent[3], bitangent[3];
   float v0[3], v1[3], v2[3], uv0[2], uv1[2], uv2[2], deltapos1[3], deltapos2[3], deltauv1[2], deltauv2[2];
   float r;
   /*
   unsigned int vertexindices[100000];
   unsigned int texindices[100000];
   unsigned int normalindices[100000];
   */
   unsigned int *vertexindices = malloc(sizeof(unsigned int) * 5000000);
   unsigned int *texindices = malloc(sizeof(unsigned int) * 5000000);
   unsigned int *normalindices = malloc(sizeof(unsigned int) * 5000000);
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
   for(i = 0; i < verticessize / sizeof(float) / 9; i++){
      v0[0]  = verticesind[i * 9];
      v0[1]  = verticesind[i * 9 + 1];
      v0[2]  = verticesind[i * 9 + 2];
      v1[0]  = verticesind[i * 9 + 3];
      v1[1]  = verticesind[i * 9 + 4];
      v1[2]  = verticesind[i * 9 + 5];
      v2[0]  = verticesind[i * 9 + 6];
      v2[1]  = verticesind[i * 9 + 7];
      v2[2]  = verticesind[i * 9 + 8];
      uv0[0] = texturecoordsind[i * 6];
      uv0[1] = texturecoordsind[i * 6 + 1];
      uv1[0] = texturecoordsind[i * 6 + 2];
      uv1[1] = texturecoordsind[i * 6 + 3];
      uv2[0] = texturecoordsind[i * 6 + 4];
      uv2[1] = texturecoordsind[i * 6 + 5];
      deltapos1[0] = v1[0] - v0[0];
      deltapos1[1] = v1[1] - v0[1];
      deltapos1[2] = v1[2] - v0[2];
      deltapos2[0] = v2[0] - v0[0];
      deltapos2[1] = v2[1] - v0[1];
      deltapos2[2] = v2[2] - v0[2];
      deltauv1[0] = uv1[0] - uv0[0];
      deltauv1[1] = uv1[1] - uv0[1];
      deltauv2[0] = uv2[0] - uv0[0];
      deltauv2[1] = uv2[1] - uv0[1];
      r = 1.0f / (deltauv1[0] * deltauv2[1] - deltauv1[1] * deltauv2[0]);
      tangent[0] = r * (deltapos1[0] * deltauv2[1] - deltapos2[0] * deltauv1[1]); 
      tangent[1] = r * (deltapos1[1] * deltauv2[1] - deltapos2[1] * deltauv1[1]);
      tangent[2] = r * (deltapos1[2] * deltauv2[1] - deltapos2[2] * deltauv1[1]);
      bitangent[0] = r * (deltapos2[0] * deltauv1[0] - deltapos1[0] * deltauv2[0]);
      bitangent[1] = r * (deltapos2[1] * deltauv1[0] - deltapos1[1] * deltauv2[0]);
      bitangent[2] = r * (deltapos2[2] * deltauv1[0] - deltapos1[2] * deltauv2[0]);
      tangents[i * 3] = tangent[0];
      tangents[i * 9 + 1] = tangent[1];
      tangents[i * 9 + 2] = tangent[2];
      tangents[i * 9 + 3] = tangent[0];
      tangents[i * 9 + 4] = tangent[1];
      tangents[i * 9 + 5] = tangent[2];
      tangents[i * 9 + 6] = tangent[0];
      tangents[i * 9 + 7] = tangent[1];
      tangents[i * 9 + 8] = tangent[2];
      bitangents[i * 9] = bitangent[0];
      bitangents[i * 9 + 1] = bitangent[1];
      bitangents[i * 9 + 2] = bitangent[2];
      bitangents[i * 9 + 3] = bitangent[0];
      bitangents[i * 9 + 4] = bitangent[1];
      bitangents[i * 9 + 5] = bitangent[2];
      bitangents[i * 9 + 6] = bitangent[0];
      bitangents[i * 9 + 7] = bitangent[1];
      bitangents[i * 9 + 8] = bitangent[2];
      tangentssize += sizeof(tangents[i]) * 9;
      bitangentssize += sizeof(bitangents[i]) * 9;
   }
   memcpy(vertices, verticesind, verticessize);
   memcpy(texcoords, texturecoordsind, texturecoordsize);
   memcpy(normals, normalsind, normalssize);
   if(tangentsout && bitangentsout){
      memcpy(tangentsout, tangents, tangentssize);
      memcpy(bitangentsout, bitangents, bitangentssize);
   }
   /*
   memcpy(indices, vertexindices, sizeof(vertexindices));
   memcpy(textureindices, texindices, sizeof(texindices));
   memcpy(normalsindices, normalindices, sizeof(normalindices));
   */
   *vertexindexsize = verticessize;
   *texcoordindexsize = texturecoordsize;
   *normalindexsize = normalssize;
   if(tangentindexsize && bitangentindexsize){
      *tangentindexsize = tangentssize;
      *bitangentindexsize = bitangentssize;
   }
   free(verticesout);
   free(texcoordsout);
   free(normalsout);
   free(verticesind);
   free(texturecoordsind);
   free(normalsind);
   free(tangents);
   free(bitangents);
   return 0;
}
