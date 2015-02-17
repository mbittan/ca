#ifndef RUNTIME_H_
#define RUNTIME_H_

#define MLUNIT   1
#define MLBOOL   2
#define MLINT    3
#define MLDOUBLE 4
#define MLSTRING 5
#define MLPAIR   6
#define MLLIST   7
#define MLFUN    8

typedef struct _mlvalue_ {
  int type;
  union {
    int MLunit;
    int MLint;
    int MLbool;
    double MLdouble;
    char* MLstring;
    struct _mlpair_ {
      struct _mlvalue_ *MLfst, *MLsnd;
    } MLpair;
    struct _mllist_ {
      struct _mlvalue_ *MLcar;
      struct _mllist_  *MLcdr;
    } *MLlist;
    struct _mlfun_ {
      int counter;
      int nbparams;
      struct _mlvalue_* env;
      struct _mlvalue_ (*invoke)(struct _mlvalue_);
    } MLfun;
  };
} MLvalue;


/***********************/
/******* Runtime *******/
/***********************/

// Booleens
const MLvalue MLtrue  = {MLBOOL, {1}};
const MLvalue MLfalse = {MLBOOL, {0}};
// Unit
const MLvalue MLlrp   = {MLUNIT, {0}};
// nil
const MLvalue MLnil   = {MLLIST, {NULL}};
// integer arithmetics

#endif
