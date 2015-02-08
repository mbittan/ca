#define _XOPEN_SOURCE 777

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "um.h"


int main(int argc, char** argv) {
  

  return 0;
}


void initialize() {
  int i;

  collection.size = 16;
  collection.arrays = malloc(collection.size*sizeof(uint32_t*));

  for(i=0; i<collection.size; i++)
    collection.arrays[i] = NULL;

  collection.free = 1;

  for(i=0; i<8; i++)
    registers[i] = 0;
}

int load_code(char* path) {
  int fd;
  struct stat st;

  pc = 0;
  
  if(stat(path, &st) == -1) {
    perror("load_code : stat");
    return -1;
  }

  if((fd = open(path, O_RDONLY)) == -1) {
    perror("load_code : open");
    return -1;
  }

  collection.arrays[0] = malloc(st.st_size);
  program_size = st.st_size/sizeof(uint32_t);

  if(read(fd, collection.arrays[0], st.st_size) != st.st_size) {
    perror("load_code : read");
    return -1;
  }

  if(close(fd) == -1) {
    perror("load_code : close");
    return -1;
  }

  return 0;
}

void destroy() {
  for(;collection.size>0; collection.size--) {
    if(collection.arrays[collection.size-1] != NULL)
      free(collection.arrays[collection.size-1]);
  }
  free(collection.arrays);

  exit(EXIT_SUCCESS);
}

void next() {
  uint32_t instr = collection.arrays[0][pc];
  uint32_t opnb = instr >> 28;
  uint32_t a, b, c, value;

  if(opnb == 13) {
    a = (instr >> 25) & 7;
    value = (instr << 7) >> 7;
  }
  else {
    a = instr & 7;
    b = (instr >> 3) & 7;
    c = (instr >> 6) & 7;
  }

  switch(opnb) {
  case 0:
    cond_move(a,b,c);
    break;
  case 1:
    array_index(a,b,c);
    break;
  case 2:
    array_amendment(a,b,c);
    break;
  case 3:
    add(a,b,c);
    break;
  case 4:
    mult(a,b,c);
    break;
  case 5:
    div(a,b,c);
    break;
  case 6:
    nand(a,b,c);
    break;
  case 7:
    halt();
    break;
  }
   
}

void cond_move(int a, int b, int c) {
  if(registers[c] != 0)
    registers[a] = registers[b];
}


void array_index(int a, int b, int c) {
  registers[a] = collection.arrays[b][registers[c]];
}

void array_amendment(int a, int b, int c) {
  collection.arrays[registers[a]][registers[b]] = registers[c];
}

void add (int a, int b, int c) {
  registers[a] = registers[b] + registers[c];
}

void mult(int a, int b, int c) {
  registers[a] = registers[b] * registers[c];
}

void div(int a, int b, int c) {
  registers[a] = registers[b] / registers[c];
}

void nand(int a, int b, int c) {
  registers[a] = ~(registers[b] & registers[c]);
}

void halt() {
  destroy();
}

void alloc(int b, int c) {
  collection.arrays[collection.free] = malloc(registers[c]*sizeof(uint32_t));
  memset(collection.arrays[collection.free], 0, registers[c]*sizeof(uint32_t));
  registers[b] = collection.free;

  for(;collection.free<collection.size; collection.free++) {
    if(collection.arrays[collection.free] == NULL)
      break;
  }
  if(collection.free == collection.size) {
    collection.arrays = realloc(collection.arrays, collection.size*2);
    collection.size *= 2;
  }
}

void abandonment(int c) {
  free(collection.arrays[c]);
  if(c < collection.free)
    collection.free = c;
}

void output(int c) {
  printf("%c", (c & 255));
}

void input(int c) {
  registers[c] = (uint32_t) getchar();
}

void load_program(int b, int c) {
  
}

