#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 64
#define CANARY_VALUE 0xDEADBEEF
#define POISON_BYTE_VALUE 111

#define ERR_CANARY_BEFORE 0x1
#define ERR_CANARY_AFTER 0x2
#define ERR_LENGTH 0x4
#define ERR_CHECKSUM 0x8
#define ERR_NULL 0x80

template <class T>
class Stack {
 public:
  Stack() {
    canary_before = CANARY_VALUE;
    canary_after = CANARY_VALUE;
    memset(buffer, POISON_BYTE_VALUE, STACK_SIZE * sizeof(T));
    length = 0;
    checksum = computeChecksum();
    dumpIfBroken();
  }
  
  void push(const T item) {
    dumpIfBroken();
    
    assert(length < STACK_SIZE);
    buffer[length] = item;
    length++;
    
    checksum = computeChecksum();
    dumpIfBroken();
  }
  
  T pop() {
    dumpIfBroken();
    
    assert(length > 0);
    length--;
    T item = buffer[length];
    
    memset(&(buffer[length]), POISON_BYTE_VALUE, sizeof(T));
    checksum = computeChecksum();
    dumpIfBroken();
    
    return item;
  }
  
  size_t getSize() const {
    dumpIfBroken();
    return length;
  }
  
  int checkErrors() const {
    if (this == nullptr) {
      return ERR_NULL;
    }
    
    int err_bits = 0;
    if (canary_before != CANARY_VALUE) {
      err_bits |= ERR_CANARY_BEFORE;
    }
    if (canary_after != CANARY_VALUE) {
      err_bits |= ERR_CANARY_AFTER;
    }
    
    if (length > STACK_SIZE) {
      err_bits |= ERR_LENGTH;
    }
    if (checksum != computeChecksum()) {
      err_bits |= ERR_CHECKSUM;
    }
    
    return err_bits;
  }
  
  void dumpIfBroken() const {
    if (checkErrors()) {
      dump();
    }
  }
  
  void dump() const {
    size_t err_bits = checkErrors();
    
    printf("=== Dump stack [at %#X] ", this);
    printf("(%s) ===\n", err_bits ? "ERROR!" : "ok");
    
    if (err_bits & ERR_NULL) {
      printf("ERROR: null pointer!\n");
      return;
    }
    
    printf("Canary before: [%#X] ", canary_before);
    printf("(%s)\n", err_bits & ERR_CANARY_BEFORE ? "ERROR!" : "ok");
    
    printf("Canary after:  [%#X] ", canary_after);
    printf("(%s)\n", err_bits & ERR_CANARY_AFTER ? "ERROR!" : "ok");
    
    printf("Length: [%d] ", length);
    printf("(%s)\n", err_bits & ERR_LENGTH ? "ERROR!" : "ok");
    
    printf("Checksum: [%#X] ", computeChecksum());
    printf("(%s)\n", err_bits & ERR_CHECKSUM ? "ERROR!" : "ok");
    
    unsigned char* char_buffer = (unsigned char*) buffer;
    
    printf("\n=== Data dump ===\n");
    for (size_t i = 0; i < STACK_SIZE; i++) {
      if (length == i) {
        printf("<--- End of used data according to length\n");
      }
      printf("%4d: [", i);
      
      bool poison = true;
      for (size_t j = 0; j < sizeof(T); j++) {
        size_t pos = sizeof(T) * i + j;
        
        if (j != 0) {
          printf(" ");
        }
        printf("%02X", char_buffer[pos]);
        
        poison = poison && (char_buffer[pos] == POISON_BYTE_VALUE);
      }
      printf("]");
      
      if (poison) {
        printf(" (poison)");
      }
      if (!poison && i >= length) {
        printf(" (SHOULD BE POISONED!)");
      }
      
      printf("\n");
    }
    printf("=== Dump end ===\n");
  }
  
 private:
  size_t computeChecksum() const {
    char* char_buffer = (char*) buffer;
    size_t inner_size = STACK_SIZE * sizeof(T) + sizeof(size_t);
    
    size_t sum = 0;
    for (size_t i = 0; i < inner_size; i++) {
      sum = sum * 173 + char_buffer[i];
    }
    
    return sum;
  }
  
 private:
  int canary_before;
  T buffer[STACK_SIZE];
  size_t length;
  size_t checksum;
  int canary_after;
};
