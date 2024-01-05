// ------------------------------------------------------------

// This file contains a handmade implementation of the strings
// utility. The program itself is much simpler than the official
// strings, and it was just implemented for educational purposes.
//
// WARNING: This program might be buggy, so use it at your own risk.
//
// Usage: ./strings <filename>
//
// If during compilation the flag -DDEBUG is used, additional debug
// statements are printed to stdout
//
// NOTE 1: sometimes files in /proc have 0 size but they can be
// read. An example of such file is /proc/self/maps. The official
// strings is able to read all the strings of that file, but we simply
// do not handle such case. This means that our binary differs from
// the original one in the handling of cases such as /proc/self/maps.
//
// NOTE 2: our script is able to capture strings up to a certain
// length, which if we want to be pedantic is WINDOW_SIZE * 2 - 1. I
// believe the official binary strings does not have this limitation.
// 
// Made by Leonardo Tamiano, 2021-11-05.

// ------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MIN_ASCII 32
#define MAX_ASCII 126
#define ISPRINT(c) (c >= MIN_ASCII && c <= MAX_ASCII)

#define PRINTABLE(c) \
  ( (c) >= 0	     \
    && (c) <= 255    \
    && ((c) == '\t' || ISPRINT(c))		\
    )

#ifdef DEBUG
#define DEBUG_PRINT if (1)
#else
#define DEBUG_PRINT if (0)
#endif

#define WINDOW_SIZE 1024
#define MIN_LENGTH 4

// -----------------------------------------
// GLOBAL VARIABLES
unsigned char buffer[WINDOW_SIZE];
long offset;

unsigned char tmp[WINDOW_SIZE];
int tmp_size;

unsigned char full[WINDOW_SIZE * 2];
int full_size;

// -----------------------------------------

// Computes file length given a filename that is assume to exist on
// the OS level. 
long get_file_length(char *filename) {
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}

void print_sequence(unsigned char *buff, int start, int end, int offset) {
  DEBUG_PRINT {
    printf("begin printable seq (%d, %d | %d)\n", start, end, offset);
    printf("\t");
  }

  for (int j = start; j <= end; j++) {
    printf("%c", buff[j]);
  }
  printf("\n");

  DEBUG_PRINT {
    printf("end printable seq\n");
    printf("\n");
  }  
}


// Finds and prints all strings in file pointed by ptr.
void print_strings(FILE *ptr, long file_length) {
  int read_success = 1;
  int byte_read;

  while(read_success) {
    // reset buffer contents for next read
    memset(buffer, 0, WINDOW_SIZE);
    
    read_success = fread(buffer, WINDOW_SIZE, 1, ptr);

    DEBUG_PRINT {
      printf("[INFO] - About to read \n");
      for (int i = 0; i < WINDOW_SIZE; i++) {
	printf("%c", buffer[i]);
      }
      printf("\n");
    }
    
    if(!read_success) {
      if(ferror(ptr)) {
	// -- error case
	fprintf(stderr, "[ERROR] - Problems when reading file!\n");
	exit(1);
      } else if(feof(ptr)) {
	// -- EOF case
	// check how much space left with respect to EOF
	byte_read = file_length - offset;
	DEBUG_PRINT {
	  printf("[INFO] - Still have to read %d bytes = (%d - %d)!\n", byte_read, file_length, offset);
	}
      }
    } else {
      byte_read = WINDOW_SIZE;
    }

    // make sure byte_read is in a valid range
    if (byte_read < 0 || byte_read > WINDOW_SIZE) {
      fprintf(stderr, "[ERROR] - byte_read is not in a valid range (byte_read=%d, offset=%ld)\n", byte_read, offset);
      exit(1);
    }

    // now we have byte_read valid bytes in buffer
    int start = -1;
    int end = -1;

    for(int i = 0; i < byte_read; i++) {
      char c = buffer[i];
      offset++;

      // begin new printable sequence
      if (start == -1 && PRINTABLE(c)) {
	start = i;

	if(tmp_size > 0 && start != 0) {
	  // in this case tmp has to be considered on its own
	  if(tmp_size >= MIN_LENGTH) {
	    print_sequence(tmp, 0, tmp_size - 1, offset);
	  }

	  tmp_size = 0;
	  memset(tmp, 0, WINDOW_SIZE);
	}	
      }

      // end printable sequence
      if (start != -1 && !PRINTABLE(c)) {
	end = i - 1;

	if (tmp_size > 0) {
	  // we know start = 0
	  full_size = tmp_size + end + 1;

	  if (full_size >= MIN_LENGTH) {
	    memcpy(full, tmp, tmp_size);
	    memcpy(full + tmp_size, buffer, end + 1);

	    print_sequence(full, 0, full_size - 1, offset);

	    full_size = 0;
	    memset(full, 0, WINDOW_SIZE);
	  }

	  tmp_size = 0;
	  memset(tmp, 0, WINDOW_SIZE);
	} else {
	  full_size = end - start + 1;
	  if (full_size >= MIN_LENGTH) {
	    print_sequence(buffer, start, end, offset);
	  }
	}
	
	// reset for next sequence
	start = -1;
	end = -1;
      }
    }

    // current window is over
    if (start != -1) {

      // copy partial substring into tmp
      tmp_size = WINDOW_SIZE - start;
      memcpy(tmp, buffer + start, tmp_size);

      DEBUG_PRINT {
	printf("[INFO] - tmp_string size: %d\n", tmp_size);
	printf("[INFO] - tmp string: %s\n", tmp);
      }
    }
  }
}

int main(int argc, char **argv) {
  char *filename = NULL;
  FILE *ptr;

  // get file from args
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(1);
  } else {
    filename = argv[1];
  }

  // check if file exists
  if(access(filename, F_OK) != 0) {
    fprintf(stderr, "[ERROR] - File %s does not exist!\n", filename);
    exit(1);
  }

  // check if file is readable
  if(access(filename, R_OK) != 0) {
    fprintf(stderr, "[ERROR] - File %s is not readable!\n", filename);
    exit(1);
  }

  // compute and validate file length
  long file_length = get_file_length(filename);
  if (file_length <= 0) {
    fprintf(stderr, "[INFO] - Either file %s is a device driver or it has 0 size, both cases which we do not handle!\n", filename);
    exit(1);
  }
  
  // 'r' -> read mode
  // 'b' -> binary mode
  ptr = fopen(filename, "rb");
  print_strings(ptr, file_length);
  fclose(ptr);

  return 0;
}
