#include <stdio.h>

// -23          -->  1111 1111 1111 1111 1111 1111 1110 1001 
// 4294967273   -->  1111 1111 1111 1111 1111 1111 1110 1001

int main(int argc, char **argv) {
  unsigned short unsigned_short = 4321;
  unsigned int   unsigned_int   = 4294967295;
  unsigned long  unsigned_long  = 18446744073709551615ul;

  printf("[INFO] – unsigned short value = %u\n", unsigned_short);
  printf("[INFO] – unsigned int value   = %u\n", unsigned_int);
  printf("[INFO] – unsigned long value  = %lu\n", unsigned_long);
  
  return 0;
}
