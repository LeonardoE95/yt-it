#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// 500 Kilobytes
#define MAX_BUFF_SIZE 1024 * 500 

typedef size_t usize;
typedef uint8_t u8;
typedef uint32_t u32;

#define IS_PRINTABLE_ASCII(c) (((c) > 31) && ((c) < 127))
#define NON_PRINTABLE_ASCII '.'

void hexdump(void *buffer, usize size) {
  u8 *data = buffer;
  usize i, j;

  for (i = 0; i < size; i++) {
    // at the start of the row, print address
    if ((i % 16) == 0) {
      printf("%08lx ", i);
    }

    // after the first 8 bytes, put a whitespace
    if ((i % 8) == 0) {
      printf(" ");
    }

    printf("%02x ", data[i]);

    // check if the row is over
    if (((i % 16) == 15) || (i == size - 1)) {

      // add extra whitespace if needed
      for (j = 0; j < 15 - (i % 16); j++) {
	printf("   ");
      }

      printf(" |");

      // print ascii
      for (j = (i - (i % 16)); j <= i; j++) {
	printf("%c",
	       IS_PRINTABLE_ASCII(data[j]) ? data[j] : NON_PRINTABLE_ASCII);
      }

      printf("|\n");
      
    }
    
  }

  printf("%08lx\n", i);

}

usize file_open_and_read(char *filepath, char *buffer, usize size) {
  // file exists
  if (access(filepath, F_OK) != 0) {
    fprintf(stderr, "[ERROR]: File `%s` does not exist!\n", filepath);
    exit(1);
  }

  // open file
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    fprintf(stderr, "[ERROR]: Could not open file `%s`!\n", filepath);
    exit(1);
  }

  // read file size
  usize file_size;
  fseek(f, 0L, SEEK_END);
  file_size = ftell(f);
  fseek(f, 0L, SEEK_SET);

  if (file_size <= 0) {
    fprintf(stderr, "[ERROR]: File has size `%zu` <= 0 bytes!\n", file_size);
    exit(1);
  }

  if (file_size > size) {
    fprintf(stderr, "[ERROR]: File size (%zu) is too big, max size allowed is `%zu`", file_size, size);
    exit(1);
  }

  // read file in memory
  fread(buffer, sizeof(char), file_size, f);
  fclose(f);
  
  return file_size;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(1);
  }

  char *filepath = argv[1];
  char buffer[MAX_BUFF_SIZE] = {0};

  usize file_size = file_open_and_read(filepath, buffer, MAX_BUFF_SIZE);
  hexdump(buffer, file_size);
  
  return 0;
}
