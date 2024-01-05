#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>

#ifdef DEBUG
#define DEBUG_PRINT if (1)
#else
#define DEBUG_PRINT if (0)
#endif

#define MAX_CHUNK_SIZE_ALLOWED 1024 * 1024 * 10 // 10 MB cap
#define MAX_FILE_LENGTH 1024 * 1024 * 30 // 30 MB cap

#define PNG_SIG_CAP 8
const uint8_t PNG_SIG[PNG_SIG_CAP] = { 137, 80, 78, 71, 13, 10, 26, 10};

void write_bytes_or_panic(FILE *file, void *buf, size_t buf_cap) {
  size_t n = fwrite(buf, buf_cap, 1, file);

  if (n != 1) {
    if (ferror(file)) {
      fprintf(stderr, "[ERROR]: Could not write %zu bytes to file, %s\n",
	      buf_cap, strerror(errno));
      exit(1);
    } else {
      assert(0 && "unreacheable");
    }
  }
  
}

void read_bytes_or_panic(FILE *file, void *buf, size_t buf_cap) {
  size_t n = fread(buf, buf_cap, 1, file);

  if (n != 1) {
    if (ferror(file)) {
      fprintf(stderr, "[ERROR]: Could not read %zu bytes from file, %s\n",
	      buf_cap, strerror(errno));
      exit(1);
    } else if(feof(file)) {
      fprintf(stderr, "[ERROR]: Could not read %zu bytes from file, reached end of file\n",
	      buf_cap);
      exit(1);      
    } else {
      fprintf(stderr, "[ERROR]: Unreacheable\n");
      assert(0);
    }
  }

  return;
}


void print_bytes(uint8_t *buf, size_t buf_cap) {
  for (size_t i = 0; i < buf_cap; i++) {
    printf("%d ", buf[i]);
  }
  printf("\n");
}

void reverse_bytes(void *buf0, size_t buf_cap) {
  uint8_t *buf = buf0;
  
  for (size_t i = 0; i < buf_cap / 2; i++) {
    uint8_t t = buf[i];
    buf[i] = buf[buf_cap - i - 1];
    buf[buf_cap - i - 1] = t;
  }

  return;
}

void print_chunk_data(void *chunk_data, uint32_t chunk_sz) {
  printf("\tChunk data: \n");
  printf("\t");

  unsigned char *data_buffer = chunk_data;
  unsigned char byte;
  uint32_t i, j;

  for (i = 0; i < chunk_sz; i++) {
    byte = data_buffer[i];
    printf("%02X ", data_buffer[i]);
    if (((i % 16) == 15) | (i == chunk_sz - 1)) {

      // NOTE: extra whitespaces if needed
      for (j = 0; j < 15 - (i % 16); j++) {
	printf("   ");
      }

      printf("| ");

      for (j = (i - (i % 16)); j <= i; j++) {
	byte = data_buffer[j];
	if ((byte > 31) && (byte < 127)) {
	  printf("%c", byte);
	} else {
	  printf(".");
	}
      }

      printf("\n\t");
    }
  }

  printf("\n");
}


void print_chunk_metadata(uint32_t chunk_sz, uint8_t chunk_type[4], uint32_t chunk_crc) {
  printf("----------------------------------\n");
  printf("\tChunk size: %u\n", chunk_sz);
  printf("\tChunk Type: %.*s\n", (int) sizeof(uint8_t) * 4, chunk_type);
  printf("\tChunk CRC: 0x%08X\n", chunk_crc);
}

void inject_chunk(FILE *file, uint32_t chunk_sz,
		  char *chunk_type, char *chunk_data, uint32_t chunk_crc) {
  reverse_bytes(&chunk_sz, sizeof(chunk_sz));
  write_bytes_or_panic(file, &chunk_sz, sizeof(chunk_sz));
  reverse_bytes(&chunk_sz, sizeof(chunk_sz));

  write_bytes_or_panic(file, chunk_type, 4);
  write_bytes_or_panic(file, chunk_data, chunk_sz);
  write_bytes_or_panic(file, &chunk_crc, sizeof(chunk_sz));  
}

// ------------

FILE *open_file(char *filepath, char *mode) {
  FILE *file_d = fopen(filepath, mode);

  if (!file_d) {
    fprintf(stderr, "[ERROR]: could not open file `%s`: %s \n", filepath, strerror(errno));
    exit(1);
  }

  return file_d;
}


void usage(char *name) {
  printf("Usage: %s [-hvs] <image.png>\n", name);
  printf("\n");
  printf("\t -h              priting help menu \n");
  printf("\t -v              dumping all data of all sections \n");
  printf("\t -s[secName]     dumping all data of section `secName` \n");
  return;
}

int main(int argc, char **argv) {
  // parse arguments
  char *options = NULL;
  char *image_filepath = NULL;
  char *file_to_hide_filepath = NULL;
  char *output_filepath = NULL;

  bool help_mode = false;     // -h
  bool verbose_mode = false;  // -v
  bool section_mode = false;  // -s
  bool write_mode = false;

  if (argc == 4) {
    // NOTE: assuming: ./png_hider.c <input.PNG> <SECRET> <output.PNG>
    write_mode = true;
    image_filepath = argv[1];
    file_to_hide_filepath = argv[2];
    output_filepath = argv[3];    
  } else if(argc == 3) {
    // NOTE: assuming only 1 option at a time: ./png_hider.c [-hvs] <input.PNG>
    options = argv[1];
    image_filepath = argv[2];
  } else if (argc == 2) {

    // NOTE: Handle case when only `-h` is given
    if (strlen(argv[1]) == 2 && (memcmp(argv[1], "-h", 2) == 0)) {
      image_filepath = NULL;
      options = argv[1];
    } else {
      image_filepath = argv[1];    
    }
    
  } else {
    fprintf(stderr, "Usage: %s [-hvs] <image.png> <file_to_hide> <output.png> \n", argv[0]);
    exit(1);
  }

  // parsing various modes of operation
  if (options) {
    assert((strlen(options) == 2 || strlen(options) == 6) && "Invalid length of options!");
    help_mode    = memcmp(options, "-h", 2) == 0;
    verbose_mode = memcmp(options, "-v", 2) == 0;
    section_mode = memcmp(options, "-s", 2) == 0;

    if(!help_mode && !verbose_mode && !section_mode) {
      fprintf(stderr, "[ERROR]: flags allowed are `-h`, `-v`, `-s[secName]`\n");
      exit(1);
    }
    
  }

  char section[4] = {0};
  if (section_mode) {
    assert(strlen(options) == 6 && "Invalid length of section name!");
    memcpy(section, options + 2, 5);
  }

  DEBUG_PRINT {
    printf("Arguments: \n");
    printf("\t image_filepath = `%s` \n" , image_filepath);
    printf("\t help_mode      = `%d` \n" , help_mode);
    printf("\t verbose_mode   = `%d` \n" , verbose_mode);
    printf("\t section_mode   = `%d` \n" , section_mode);
    printf("\t section        = `%.*s` \n" ,
	   (int) sizeof(char) * 4, section);    
  }

  if(help_mode) {
    usage(argv[0]);
    return 0;
  }

  // -------------------------------------------------------
  // Start tot read <input.png>
  FILE *input_file, *file_to_hide, *output_file;
  
  input_file = open_file(image_filepath, "rb");

  if (write_mode) {
    file_to_hide = open_file(file_to_hide_filepath, "rb");
    output_file = open_file(output_filepath, "wb");    
  }

  // NOTE: check for PNG signature
  uint8_t sig[PNG_SIG_CAP];
  read_bytes_or_panic(input_file, sig, PNG_SIG_CAP);

  DEBUG_PRINT {
    printf("Signature: ");
    print_bytes(sig, PNG_SIG_CAP);
  }
  
  if(memcmp(sig, PNG_SIG, PNG_SIG_CAP) != 0) {
    fprintf(stderr, "ERROR: PNG signature not valid in `%s`\n", image_filepath);  
    exit(1);
  }

  if (write_mode) {
    write_bytes_or_panic(output_file, sig, PNG_SIG_CAP);
  }

  // NOTE: read chunk
  bool quit = false;
  bool chunk_added = false;
  while (!quit) {
    // read chunk length
    uint32_t chunk_sz;
    read_bytes_or_panic(input_file, &chunk_sz, sizeof(chunk_sz));
    if (write_mode) {
      write_bytes_or_panic(output_file, &chunk_sz, sizeof(chunk_sz));
    }
    reverse_bytes(&chunk_sz, sizeof(chunk_sz));

    // read chunk type
    uint8_t chunk_type[4];
    read_bytes_or_panic(input_file, chunk_type, sizeof(chunk_type));
    if (write_mode) {
      write_bytes_or_panic(output_file, chunk_type, sizeof(chunk_type));
    }  
  
    // read chunk data
    //
    // NOTE: assert() could be removed if we read chunk_sz iteratively
    // using an array of static size.
    assert(chunk_sz < MAX_CHUNK_SIZE_ALLOWED && "Chunk size too big!");

    char *chunk_data = NULL;
    if (chunk_sz > 0) {
      chunk_data = malloc(chunk_sz);
      read_bytes_or_panic(input_file, chunk_data, chunk_sz);
      if (write_mode) {
	write_bytes_or_panic(output_file, chunk_data, chunk_sz);
      }
    }  

    // read chunk CRC
    uint32_t chunk_crc;
    read_bytes_or_panic(input_file, &chunk_crc, sizeof(chunk_crc));
    if (write_mode) {
      write_bytes_or_panic(output_file, &chunk_crc, sizeof(chunk_crc));
    }

    // --------------------------------------
    // we're ready to insert secret file

    if (write_mode && !chunk_added) {
      // compute size of file to hide
      fseek(file_to_hide, 0L, SEEK_END);
      unsigned long file_length = ftell(file_to_hide);
      fseek(file_to_hide, 0L, SEEK_SET);

      assert(file_length < MAX_FILE_LENGTH && "file to hide too big!\n");

      // prepare chunk to inject
      uint32_t injected_sz = (uint32_t) file_length;
      char *data_to_inject = malloc(file_length);
      read_bytes_or_panic(file_to_hide, data_to_inject, injected_sz);
      char *injected_type = "tEXt";
      uint32_t injected_crc = 0;

      inject_chunk(output_file, injected_sz, injected_type, data_to_inject, injected_crc);

      free(data_to_inject);
      chunk_added = true;
    }    

    // --------------------------------------
    // we're ready to print stuff

    if (!write_mode && section_mode && (memcmp(chunk_type, section, 4) == 0)) {
      if (chunk_sz > 0) {
	write_bytes_or_panic(stdout, chunk_data, chunk_sz);
	chunk_data = NULL;
	free(chunk_data);
      }
    } else if (!write_mode && !section_mode) {
      print_chunk_metadata(chunk_sz, chunk_type, chunk_crc);

      if (verbose_mode && chunk_sz > 0) {
	print_chunk_data(chunk_data, chunk_sz);
	chunk_data = NULL;
	free(chunk_data);
      }
    }

    if (write_mode && chunk_sz > 0) {
      free(chunk_data);
    }
    
    quit = *(uint32_t*) chunk_type == 0x444E4549;    
  }
  
  fclose(input_file);

  if (write_mode) {
    fclose(file_to_hide);
    fclose(output_file);
  }
  
  return 0;
}
