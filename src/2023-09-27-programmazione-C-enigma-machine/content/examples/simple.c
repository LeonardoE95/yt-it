#include <stdio.h>

#define ENIGMA_IMPLEMENTATION
#include "../enigma.h"

int main() {
  const char *rotors[3] = {"III", "II", "I"};
  
  const char *reflector_name = "B";
  
  uint8_t board[][2] = {
    {0, 1},
    {2, 3}
  };

  Plugboard plugboard = {board, 2};

  Enigma *e = init_enigma(rotors, reflector_name, plugboard);
  Enigma *e2 = init_enigma(rotors, reflector_name, plugboard);
  
  char *str = "HELLOWORLD";
  size_t length = strlen(str);

  char *plaintext = malloc(length * sizeof(char));
  char *ciphertext = malloc(length * sizeof(char));
  strncpy(plaintext, str, length);
  strncpy(ciphertext, str, length);

  enigma_encrypt(e, (uint8_t*)plaintext, length, (uint8_t*)ciphertext);
  printf("%s -> %s\n", plaintext, ciphertext);

  strncpy(plaintext, ciphertext, length);
  enigma_decrypt(e2, (uint8_t*)ciphertext, length, (uint8_t*)plaintext);
  printf("%s -> %s\n", ciphertext, plaintext);

  destroy_enigma(e);
  free(plaintext);
  free(ciphertext);
  
  return 0;
}
