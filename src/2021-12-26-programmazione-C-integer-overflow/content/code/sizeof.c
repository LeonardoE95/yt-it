#include <stdio.h>

// To compile and run:
// ------------------------------
// gcc sizeof.c -o sizeof
// ./sizeof    

int main(int argc, char **argv) {
  printf("[INFO] – Size of char      = %d\n", sizeof(char));
  printf("===============================\n");
  printf("[INFO] – Size of short     = %d\n", sizeof(short));
  printf("[INFO] – Size of int       = %d\n", sizeof(int));
  printf("[INFO] – Size of long      = %d\n", sizeof(long));
  printf("[INFO] – Size of long long = %d\n", sizeof(long long));
  printf("===============================\n");
  printf("[INFO] – Size of float     = %d\n", sizeof(float));
  printf("[INFO] – Size of double    = %d\n", sizeof(double));
  
  return 0;
}
