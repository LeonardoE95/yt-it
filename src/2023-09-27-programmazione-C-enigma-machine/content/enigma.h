#ifndef ENIGMA_H_
#define ENIGMA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

// ------------------------------------------------
// DATA STRUCTURES

#define ALPHABET_SIZE 26
#define PLUGBOARD_SIZE 10

typedef uint8_t Wiring[ALPHABET_SIZE];

typedef enum {
  RO_FORWARD,
  RO_BACKWARD
} RotorOrder;

typedef struct {
  Wiring forward_wiring;
  Wiring backward_wiring;  
  uint8_t position;
  uint8_t ring;
  uint8_t notch;
  char *name;
} Rotor;

typedef struct {
  Wiring wiring;
  char *name;
} Reflector;

typedef struct {
  uint8_t (*board)[2];
  uint8_t board_size;
} Plugboard;

// To properly configure an Enigma machine you need the following:
//
// - The rotors used
// - Rotor order (from left to right)
// - Ring setting for each rotor
// - Starting position for each rotor
// - Plugboard connection
// - Type of reflector
// 
typedef struct {
  Plugboard plugboard;
  Reflector reflector;
  Rotor rotors[3];
} Enigma;

// ------------------------------------------------
// FUNCTIONS DECLARATIONS

Enigma *init_enigma(const char *rotors_names[3], const char *reflector_name, Plugboard plugboard);
void init_wiring(Wiring wiring, const char *alphabet, size_t alphabet_len);
void reverse_wiring(Wiring new_wiring, Wiring old_wiring, size_t wiring_len);
void init_rotor(Rotor *r, const char *rotor_name);
void init_reflector(Reflector *ref, const char *reflector_name);
void destroy_enigma(Enigma *e);

void move_rotors(Enigma *e);
uint8_t apply_rotor(Rotor *r, uint8_t char_code, RotorOrder order);
uint8_t apply_rotors(Enigma *e, uint8_t char_code, RotorOrder order);
uint8_t apply_plugboard(Enigma *e, const uint8_t char_code);
uint8_t apply_reflector(Enigma *e, const uint8_t char_code);
void apply_enigma(Enigma *e, const uint8_t *input, size_t input_len, uint8_t *output);

void enigma_encrypt(Enigma *e, const uint8_t *plaintext, size_t plaintext_len, uint8_t *ciphertext);
void enigma_decrypt(Enigma *e, const uint8_t *ciphertext, size_t ciphertext_len, uint8_t *plaintext);

#define CHAR2CODE(c) ((c) - 'A')
#define CODE2CHAR(c) ('A' + (c))

#endif // ENIGMA_H_

// ------------------------------------------------

#ifdef ENIGMA_IMPLEMENTATION

// -----------------------------
// ENIGMA MODELS

void init_wiring(Wiring wiring, const char *alphabet, size_t alphabet_len) {
  for (size_t i = 0; i < alphabet_len; i++) {
    wiring[i] = (uint8_t) (alphabet[i] - 'A');
  }
}

// Used to change the direction of old_wiring into new_wiring
//
// new_wiring[x] = y if and only if old_wiring[y] = x
// 
void reverse_wiring(Wiring new_wiring, Wiring old_wiring, size_t wiring_len) {
  for (size_t i = 0; i < wiring_len; i++) {
    new_wiring[old_wiring[i]] = (uint8_t) i;
  }
}

void init_rotor(Rotor *r, const char *rotor_name) {
  if (strcmp(rotor_name, "I") == 0) {
    init_wiring(r->forward_wiring, "EKMFLGDQVZNTOWYHXUSPAIBRCJ", ALPHABET_SIZE);
    r->notch = (uint8_t) 'Q' - 'A';
  } else if (strcmp(rotor_name, "II") == 0) {
    init_wiring(r->forward_wiring, "AJDKSIRUXBLHWTMCQGZNPYFVOE", ALPHABET_SIZE);
    r->notch = (uint8_t) 'E' - 'A';
  } else if (strcmp(rotor_name, "III") == 0) {
    init_wiring(r->forward_wiring, "BDFHJLCPRTXVZNYEIWGAKMUSQO", ALPHABET_SIZE);
    r->notch = (uint8_t) 'V' - 'A';
  } else if (strcmp(rotor_name, "IV") == 0) {
    init_wiring(r->forward_wiring, "ESOVPZJAYQUIRHXLNFTGKDCMWB", ALPHABET_SIZE);
    r->notch = (uint8_t) 'J' - 'A';
  } else if (strcmp(rotor_name, "V") == 0) {
    init_wiring(r->forward_wiring, "VZBRGITYUPSDNHLXAWMJQOFECK", ALPHABET_SIZE);
    r->notch = (uint8_t) 'Z' - 'A';
  }

  // TODO: make these value dynamic by accepting them from the user
  // during Enigma creation
  r->position = 0;
  r->ring = 0;
  reverse_wiring(r->backward_wiring, r->forward_wiring, ALPHABET_SIZE);

  size_t len = strlen(rotor_name);
  r->name = malloc(sizeof(char) * len);
  strncpy(r->name, rotor_name, len);  
}

void init_reflector(Reflector *ref, const char *reflector_name) {
  if (strncmp(reflector_name, "A", 1) == 0) {
    init_wiring(ref->wiring, "EJMZALYXVBWFCRQUONTSPIKHGD", ALPHABET_SIZE);
  } else if (strncmp(reflector_name, "B", 1) == 0) {
    init_wiring(ref->wiring, "YRUHQSLDPXNGOKMIEBFZCWVJAT", ALPHABET_SIZE);
  } else if (strncmp(reflector_name, "C", 1) == 0) {
    init_wiring(ref->wiring, "FVPJIAOYEDRZXWGCTKUQSBNMHL", ALPHABET_SIZE);
  }

  size_t len = strlen(reflector_name);
  ref->name = malloc(sizeof(char) * len);
  strncpy(ref->name, reflector_name, len);
}

Enigma *init_enigma(const char *rotor_names[3], const char *reflector_name, Plugboard plugboard) {
  Enigma *e = malloc(sizeof(Enigma));

  init_rotor(&e->rotors[0], rotor_names[2]);
  init_rotor(&e->rotors[1], rotor_names[1]);
  init_rotor(&e->rotors[2], rotor_names[0]);

  init_reflector(&e->reflector, reflector_name);

  if (plugboard.board_size > PLUGBOARD_SIZE) {
    printf("[ERROR]: init_enigma() - supplied plugboard size (%d) greater than maximum allowed (%d)\n",
	   plugboard.board_size,
	   PLUGBOARD_SIZE);
    exit(0);
  }  
  e->plugboard = plugboard;

  return e;
}

void destroy_enigma(Enigma *e) {
  if (e) {

    if (e->rotors[0].name) free(e->rotors[0].name);
    if (e->rotors[1].name) free(e->rotors[1].name);
    if (e->rotors[2].name) free(e->rotors[2].name);

    if (e->reflector.name) free(e->reflector.name);    
    
    free(e);
  }
}

// -----------------------------
// CORE LOGIC

void move_rotors(Enigma *e) {
  // Double stepping caused by the claw mechanism used for rotating
  // the rotors makes the second rotor move twice in a row, if the
  // first movement brings it in the turnover position during the
  // first rotation.
  //
  // https://en.wikipedia.org/wiki/Enigma_machine  
  // https://www.youtube.com/watch?v=ds8HoowfewA  
  // https://www.youtube.com/watch?v=5StZlF-clPc
  // https://www.youtube.com/watch?v=hcVhQeZ5gI4
  //

  if (e->rotors[1].position == e->rotors[1].notch) {
    // double stepping
    e->rotors[2].position = (e->rotors[2].position + 1) % ALPHABET_SIZE;
    e->rotors[1].position = (e->rotors[1].position + 1) % ALPHABET_SIZE;    
  } else if (e->rotors[0].position == e->rotors[0].notch) {
    e->rotors[1].position = (e->rotors[1].position + 1) % ALPHABET_SIZE;
  }

  e->rotors[0].position = (e->rotors[0].position + 1) % ALPHABET_SIZE;  
}

uint8_t apply_rotor(Rotor *r, uint8_t char_code, RotorOrder order) {
  char_code = (char_code - r->ring + r->position + ALPHABET_SIZE) % ALPHABET_SIZE;
  switch(order) {
  case RO_FORWARD: {
    char_code = r->forward_wiring[char_code];
  } break;
  case RO_BACKWARD: {
    char_code = r->backward_wiring[char_code];
  } break;
  default: assert(0 && "apply_rotor(): Unreachable!\n");
  }
  char_code = (char_code + r->ring - r->position + ALPHABET_SIZE) % ALPHABET_SIZE;
  return char_code;
}

uint8_t apply_rotors(Enigma *e, uint8_t char_code, RotorOrder order) {
  switch(order) {
  case RO_FORWARD: {
    char_code = apply_rotor(&e->rotors[0], char_code, RO_FORWARD);
    char_code = apply_rotor(&e->rotors[1], char_code, RO_FORWARD);
    char_code = apply_rotor(&e->rotors[2], char_code, RO_FORWARD);    
  } break;

  case RO_BACKWARD: {
    char_code = apply_rotor(&e->rotors[2], char_code, RO_BACKWARD);
    char_code = apply_rotor(&e->rotors[1], char_code, RO_BACKWARD);
    char_code = apply_rotor(&e->rotors[0], char_code, RO_BACKWARD);
  } break;

  default: assert(0 && "apply_rotors(): Unreachable!\n");
  }

  return char_code;
}

uint8_t apply_plugboard(Enigma *e, const uint8_t char_code) {
  for (size_t i = 0; (i < e->plugboard.board_size && i < PLUGBOARD_SIZE); i++) {
    if (char_code == e->plugboard.board[i][0]) {
      return e->plugboard.board[i][1];
    } else if (char_code == e->plugboard.board[i][1]) {
      return e->plugboard.board[i][0];
    }
  }
  return char_code;
}

uint8_t apply_reflector(Enigma *e, const uint8_t char_code) {
  return e->reflector.wiring[char_code];
}

void apply_enigma(Enigma *e, const uint8_t *input, size_t input_len, uint8_t *output) {
  // NOTE: assume output has been already allocated such that strlen(output) == input_len
  
  for(size_t i = 0; i < input_len; i++) {
    // transform character into char_code
    uint8_t char_code = CHAR2CODE(input[i]);

    // Main enigma loop:
    // - move rotors
    move_rotors(e);
    // - apply plugboard
    char_code = apply_plugboard(e, char_code);
    // - apply rotors from RIGHT TO LEFT (FORWARD direction)
    char_code = apply_rotors(e, char_code, RO_FORWARD);
    // - apply reflector
    char_code = apply_reflector(e, char_code);
    // - apply rotors from LEFT TO RIGHT (BACKWARD direction)
    char_code = apply_rotors(e, char_code, RO_BACKWARD);
    // - apply plugboard
    char_code = apply_plugboard(e, char_code);
    
    uint8_t char_output = CODE2CHAR(char_code);
    output[i] = char_output;
  }
}

void enigma_encrypt(Enigma *e, const uint8_t *plaintext, size_t plaintext_len, uint8_t *ciphertext) {
  assert(plaintext_len == strlen((const char*)ciphertext) && "enigma_encrypt(): strlen(ciphertext) != plaintext_len");
  apply_enigma(e, plaintext, plaintext_len, ciphertext);
  #ifdef ENIGMA_DEBUG
  printf("[INFO] enigma_encrypt(): '%s' -> '%s'\n", plaintext, ciphertext);
  #endif
}

void enigma_decrypt(Enigma *e, const uint8_t *ciphertext, size_t ciphertext_len, uint8_t *plaintext) {
  assert(ciphertext_len == strlen((const char*)plaintext) && "enigma_decrypt(): strlen(plaintext) != ciphertext_len");
  apply_enigma(e, ciphertext, ciphertext_len, plaintext);
  #ifdef ENIGMA_DEBUG
  printf("[INFO] enigma_decrypt(): '%s' -> '%s'\n", ciphertext, plaintext);
  #endif  
}

#endif // ENIGMA_IMPLEMENTATION
