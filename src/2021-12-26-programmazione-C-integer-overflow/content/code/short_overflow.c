#include <stdio.h>

int main(int argc, char **argv) {
  
  short normal_value = 20;
  short overflowing_value = (short) (65536 + 10);

  printf("[INFO] – Normal short value       = %hd\n", normal_value);
  printf("[INFO] – Overflowing short value  = %hd\n", overflowing_value);

  return 0;
}
