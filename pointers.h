#include<stdio.h>
#include<stdlib.h>

void insertarrayint(int **ptr, int *size, int data){
   if(*size == 0){
      printf("Allocating memory for pointer with size: %d\n", *size);
      *ptr = malloc(sizeof(int));
      if(*ptr == NULL){
         printf("Failed to allocate memory!\n");
      }
   }else{
      printf("Reallocating memory for new pointer with size: %d\n", *size);
      *ptr = realloc(*ptr, (*size + 1) * sizeof(int));
      if(*ptr == NULL){
         printf("Failed to allocate memory!\n");
      }
   }
   printf("Before setting data with size: %d\n", *size);
   (*ptr)[*size] = data;
   printf("After setting data with size: %d\n", *size);
   (*size)++;
}

void insertarrayfloat(float **ptr, int *size, float data){
   if(*size == 0){
      printf("Allocating memory for pointer with size: %d\n", *size);
      *ptr = malloc(sizeof(float));
      if(*ptr == NULL){
         printf("Failed to allocate memory!\n");
      }
   }else{
      printf("Reallocating memory for new pointer with size: %d\n", *size);
      *ptr = realloc(*ptr, (*size + 1) * sizeof(float));
      if(*ptr == NULL){
         printf("Failed to allocate memory!\n");
      }
   }
   printf("Before setting data with size: %d\n", *size);
   (*ptr)[*size] = data;
   printf("After setting data with size: %d\n", *size);
   (*size)++;
}
