#include<stdlib.h>

int insertarrayuint(unsigned int **ptr, int size, int data){
   int *temp;
   if(size == 0){
      temp = malloc(sizeof(unsigned int));
      if(!temp){
         printf("Error!\n");
         return 1;
      }else{
         *ptr = temp;
      }
      **ptr = data;
   }else{
      temp = realloc(*ptr, (size+1) * sizeof(unsigned int));
      if(!temp){
         printf("Error!\n");
         return 1;
      }else{
         *ptr = temp;
      }
      (*ptr)[size] = data;
   }
}

int insertarrayfloat(float **ptr, int size, float data){
   float *temp;
   if(size == 0){
      temp = malloc(sizeof(float));
      if(!temp){
         printf("Error!\n");
         return 1;
      }else{
         *ptr = temp;
      }
      **ptr = data;
   }else{
      temp = realloc(*ptr, (size+1) * sizeof(float));
      if(!temp){
         printf("Error!\n");
         return 1;
      }else{
         *ptr = temp;
      }
      (*ptr)[size] = data;
   }
}
