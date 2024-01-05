#include <stdio.h>

int main(int argc, char **argv) {
  
  int normal_value = 1337;
  int overflowing_value = (int) (4294967296);

  printf("[INFO] – Normal integer value       = %d\n", normal_value);
  printf("[INFO] – Overflowing integer value  = %d\n", overflowing_value);

  return 0;
}
