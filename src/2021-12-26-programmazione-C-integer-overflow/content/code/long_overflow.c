// gcc long_overflow.c -o long_overflow
// ./long_overflow

#include <stdio.h>

int main(int argc, char **argv) {
  
  long normal_value = 4294967296;
  long overflowing_value = (long) (18446744073709551616);

  printf("[INFO] – Normal long value       = %ld\n", normal_value);
  printf("[INFO] – Overflowing long value  = %ld\n", overflowing_value);

  return 0;
}
