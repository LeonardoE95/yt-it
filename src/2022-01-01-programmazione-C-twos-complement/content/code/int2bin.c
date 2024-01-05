#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
  Simple program to show the binary representation of integers to be
  used for educational purposes.

  Author: Leonardo Tamiano.
  Youtube: https://www.youtube.com/channel/UCqZe2cxQNCvgkQxUuqsIaig
*/

void print_binary(int value) {
  int bit_size = sizeof(int) * 8;
  int group_length = 4;
    
  int str_len = bit_size + 1 + ((bit_size / group_length) - 1);
  char bin_str[str_len];

  for (int i = 0, j = 0, c = 0; i < str_len; i++) {
    if (j > 0 && ((j % 4) == 0)) {
      bin_str[i] = ' ';
      j = 0;
      c++;
    } else {
      int bit_index = bit_size - 1 - i + c;
      bin_str[i] = (value >> bit_index) & 1 ? '1' : '0';
      j += 1;
    }
  }
  
  bin_str[str_len] = '\0';
  
  printf("%s\n", bin_str);
}

void test(void) {
  int values_to_test[] = {
    0,
    2147483647,
    2147483648, // equivalent to -2147483648
    4294967295, // equivalent to -1
  };

  for (int i = 0; i < sizeof(values_to_test)/sizeof(int); i++) {
    printf("Binary representation for: %d\n", values_to_test[i]);
    printf("\t");
    print_binary(values_to_test[i]);
    printf("\n");
  }
}

// -------------------------------------

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <number>\n", argv[0]);
    exit(1);
  }

  char *endptr, *str;
  int value;
  int base = 10;

  str = argv[1];
  value = strtol(str, &endptr, base);
  
  if (errno != 0) {
    perror("strtol: ");
    exit(1);
  }

  if (endptr == str) {
    fprintf(stderr, "No digits found\n");
    exit(1);
  }

  printf("Binary representation for: %d\n", value);
  printf("\t");
  print_binary(value);
  printf("\n");

  return 0;
}
