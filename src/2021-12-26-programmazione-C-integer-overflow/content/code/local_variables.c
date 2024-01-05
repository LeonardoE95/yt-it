// To compile and run:
// ------------------------------
// gcc local_variables.c -o local_variables
// ./local_variables

#include <stdio.h>

int main(int argc, char **argv) {
  int var1 = 10;
  int var2 = 20;

  int *addr1 = &var1;
  int *addr2 = &var2;

  printf("[INFO] – Value of var1   = %d\n", var1);
  printf("[INFO] – Value of var2   = %d\n", var2);
  printf("=============================\n");
  printf("[INFO] – Address of var1 = %p\n", addr1);
  printf("[INFO] – Address of var2 = %p\n", addr2);
    
  return 0;
}
