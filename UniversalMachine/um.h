#ifndef UM_H_
#define UM_H_

uint32_t registers[8];

typedef struct _coll_ {
  uint32_t** arrays;
  uint32_t size;
  uint32_t free;
} coll;

coll collection;

uint32_t pc;
uint32_t program_size;


void initialize();

int load_code(char* path);

void destroy();

void next();

void cond_move(int a, int b, int c);

void array_index(int a, int b, int c);

void array_amendment(int a, int b, int c);

void add (int a, int b, int c);

void mult (int a, int b, int c);

void div (int a, int b, int c);

void nand (int a, int b, int c);

void halt();

void alloc(int b, int c);

void abandonment(int c);

void output(int c);

void input(int c);

void load_program(int b, int c);

#endif
