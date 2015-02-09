#ifndef UM_H_
#define UM_H_

#define NBREG 8

uint32_t registers[NBREG];

struct coll {
  uint32_t** arrays;
  uint32_t* lengths;
  uint32_t size;
  uint32_t free;
} collection;

uint32_t pc;

void initialize();

int load_code(char* path);

void destroy();

void next();

void cond_move(int a, int b, int c);

void array_index(int a, int b, int c);

void array_amendment(int a, int b, int c);

void add(int a, int b, int c);

void mult(int a, int b, int c);

void division(int a, int b, int c);

void nand(int a, int b, int c);

void halt();

void alloc(int b, int c);

void abandonment(int c);

void output(int c);

void input(int c);

void load_program(int b, int c);

void orthography(int a, int value);

void error(int errno);

void print_instruction(uint32_t instr);

#define SEGFLT 1
#define NOTINS 2

#endif
