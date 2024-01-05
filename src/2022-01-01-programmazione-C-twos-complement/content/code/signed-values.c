#include <stdio.h>

int main(int argc, char **argv) {
  signed short signed_short = -4321;
  signed int signed_int   = -2147483644;
  signed long signed_long = -9223372036854775807ul;

  printf("[INFO] – unsigned short value = %d\n", signed_short);
  printf("[INFO] – unsigned int value   = %d\n", signed_int);
  printf("[INFO] – unsigned long value  = %ld\n", signed_long);
  
  return 0;
}
