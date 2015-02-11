#define _XOPEN_SOURCE 777

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "um.h"


int main(int argc, char** argv) {
  if(argc != 2) {
    fprintf(stderr, "Usage: um file\n");
    return EXIT_FAILURE;
  }
  
  initialize();
  if(load_code(argv[1]) == -1) {
    destroy();
    return EXIT_FAILURE;
  }

  printf("Instruction number : %d\n", collection.lengths[0]);

  while(pc <= collection.lengths[0]) {
    next();
  }
  
  // print_program();
  destroy();

  return EXIT_SUCCESS;
}


void initialize() {
  int i;

  collection.size = 16;
  collection.arrays = malloc(collection.size*sizeof(uint32_t*));
  collection.lengths = malloc(collection.size*sizeof(uint32_t));

  for(i=0; i<collection.size; i++) {
    collection.arrays[i] = NULL;
    collection.lengths[i] = 0;
  }

  collection.free = 1;

  for(i=0; i<NBREG; i++)
    registers[i] = 0;
}

int load_code(char* path) {
  int fd, i;
  uint32_t inst;
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
  collection.lengths[0] = st.st_size/sizeof(uint32_t);

  /* if(read(fd, collection.arrays[0], st.st_size) != st.st_size) { */
  /*   perror("load_code : read"); */
  /*   return -1; */
  /* } */

  for(i=0; i<collection.lengths[0]; i++) {
    if(read(fd, &inst, sizeof(uint32_t)) != sizeof(uint32_t)) {
      perror("load_code : read");
      return -1;
    }
    collection.arrays[0][i] = inst << 24;
    collection.arrays[0][i] &= ((inst <<  8) | 0xFF00FFFF);
    collection.arrays[0][i] &= ((inst >>  8) | 0xFFFF00FF);
    collection.arrays[0][i] &= ((inst >> 24) | 0xFFFFFF00); 
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
  free(collection.lengths);
}

void next() {
  uint32_t instr = collection.arrays[0][pc];
  uint32_t opnb = instr >> 28;
  uint32_t a, b, c, value;

  if(opnb >= 13) {
    a = (instr >> 25) & 7;
    value = (instr << 7) >> 7;
  }
  else {
    a = instr & 7;
    b = (instr >> 3) & 7;
    c = (instr >> 6) & 7;
  }

  //printf("%d\t:", pc);
  //print_instruction(instr);

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
    division(a,b,c);
    break;
  case 6:
    nand(a,b,c);
    break;
  case 7:
    halt();
    break;
  case 8:
    alloc(b,c);
    break;
  case 9:
    abandonment(c);
    break;
  case 10:
    output(c);
    break;
  case 11:
    input(c);
    break;
  case 12:
    load_program(b,c);
    break;
  case 13:
    orthography(a, value);
    break;
  default:
    error(NOTINS);
    break;
  }
  
  pc++;
}

void cond_move(int a, int b, int c) {
  if(registers[c] != 0)
    registers[a] = registers[b];
}

void array_index(int a, int b, int c) {
  if(registers[c] < collection.lengths[registers[b]])
    registers[a] = collection.arrays[registers[b]][registers[c]];
  else
    error(SEGFLT);
}

void array_amendment(int a, int b, int c) {
  if(registers[b] < collection.lengths[registers[a]])
    collection.arrays[registers[a]][registers[b]] = registers[c];
  else
    error(SEGFLT);
}

void add (int a, int b, int c) {
  registers[a] = (uint32_t) (registers[b] + registers[c]);
}

void mult(int a, int b, int c) {
  registers[a] = (uint32_t) (registers[b] * registers[c]);
}

void division(int a, int b, int c) {
  registers[a] = (uint32_t) (registers[b] / registers[c]);
}

void nand(int a, int b, int c) {
  registers[a] = ~(registers[b] & registers[c]);
}

void halt() {
  destroy();
  exit(EXIT_SUCCESS);
}

void alloc(int b, int c) {
  int i;
  
  collection.arrays[collection.free] = malloc(registers[c]*sizeof(uint32_t));
  memset(collection.arrays[collection.free], 0, 
	 registers[c]*sizeof(uint32_t));
  collection.lengths[collection.free] = registers[c];
  registers[b] = collection.free;

  for(;collection.free<collection.size; collection.free++) {
    if(collection.arrays[collection.free] == NULL)
      break;
  }
  if(collection.free == collection.size) {
    collection.arrays = realloc(collection.arrays, collection.size*2);
    collection.lengths = realloc(collection.lengths, collection.size*2);
    for(i=collection.size; i<collection.size*2; i++) {
      collection.arrays[i] = NULL;
      collection.lengths[i] = 0;
    }
    collection.size *= 2;
  }
}

void abandonment(int c) {
  free(collection.arrays[registers[c]]);
  collection.arrays[registers[c]] = NULL;
  collection.lengths[registers[c]] = 0;
  if(registers[c] < collection.free)
    collection.free = registers[c];
}

void output(int c) {
  printf("%c", (registers[c] & 255));
}

void input(int c) {
  registers[c] = (uint32_t) getchar();
}

void load_program(int b, int c) {
  uint32_t *new_prog, *tmp;
  if(collection.lengths[registers[b]] > 0) {
    new_prog = malloc(collection.lengths[registers[b]] * sizeof(uint32_t));
    memcpy(new_prog, collection.arrays[registers[b]], collection.lengths[registers[b]]*sizeof(uint32_t));
    free(collection.arrays[0]);
    collection.arrays[0] = new_prog;
    collection.lengths[0] = collection.lengths[registers[b]];
    pc = registers[c]-1;
  }
  else
    error(SEGFLT);
}

void orthography(int a, int value) {
  registers[a] = value;
}

void error(int errno) {
  switch(errno) {
  case SEGFLT:
    printf("Segmentation fault\n");
    break;
  case NOTINS:
    printf("Instruction unknown\n");
    break;
  default:
    printf("Unknown error\n");
    break;
  }
  destroy();
  exit(EXIT_FAILURE);
}

void print_instruction(uint32_t instr) {
  uint32_t opnb = instr >> 28;
  uint32_t a, b, c, value;
  char instr_name[5];
  int i;

  switch(opnb) {
  case 0:
    sprintf(instr_name, "cdmv");
    break;
  case 1:
    sprintf(instr_name, "arin");
    break;
  case 2:
    sprintf(instr_name, "aram");
    break;
  case 3:
    sprintf(instr_name, "add ");
    break;
  case 4:
    sprintf(instr_name, "mult");
    break;
  case 5:
    sprintf(instr_name, "div ");
    break;
  case 6:
    sprintf(instr_name, "nand");
    break;
  case 7:
    sprintf(instr_name, "halt");
    break;
  case 8:
    sprintf(instr_name, "allo");
    break;
  case 9:
    sprintf(instr_name, "aban");
    break;
  case 10:
    sprintf(instr_name, "out ");
    break;
  case 11:
    sprintf(instr_name, "in  ");
    break;
  case 12:
    sprintf(instr_name, "load");
    break;
  case 13:
    sprintf(instr_name, "orth");
    break;
  default:
    sprintf(instr_name, "NOIN");
    break;
  }

  if(opnb >= 13) {
    a = (instr >> 25) & 7;
    value = (instr << 7) >> 7;
    printf("%s r%d, %6d\t", instr_name, a, value);
  }
  else {
    a = instr & 7;
    b = (instr >> 3) & 7;
    c = (instr >> 6) & 7;
    printf("%s r%d, r%d, r%d\t", instr_name, a, b, c);
  }

  // print instruction byte by byte
  for(i=31; i>=0; i--) {
    printf("%d", (instr >> i) & 1);
    if(i==28)
      printf(" ");
  }
  printf("\n");
}

void print_program() {
  int i;
  
  for(i=0; i<collection.lengths[0]; i++) {
    printf("%d\t:", i);
    print_instruction(collection.arrays[0][i]);
  }
}
