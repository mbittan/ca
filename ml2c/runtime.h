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
      struct _mlvalue_ *MLcdr;
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

// Constructors
MLvalue * new_MLint(int x);
MLvalue * new_MLdouble(double x);
MLvalue * new_MLbool(int b);
MLvalue * new_MLstring(char * s);
MLvalue * MLpair(MLvalue *x, MLvalue *y);
MLvalue * MLlist(MLvalue *x, MLvalue *y);

// Booleens
const MLvalue MLtrue  = {MLBOOL, {1}};
const MLvalue MLfalse = {MLBOOL, {0}};
// Unit
const MLvalue MLlrp   = {MLUNIT, {0}};
// nil
const MLvalue MLnil = {MLLIST, {NULL}};
// integer arithmetics
MLvalue * MLaddint(MLvalue * x, MLvalue * y);
MLvalue * MLsubint(MLvalue * x, MLvalue * y);
MLvalue * MLmulint(MLvalue * x, MLvalue * y);
MLvalue * MLdivint(MLvalue * x, MLvalue * y);
// equals
MLvalue * MLequal(MLvalue * x, MLvalue * y);
// integer comparisons
MLvalue * MLltint(MLvalue * x, MLvalue * y);
MLvalue * MLleint(MLvalue * x, MLvalue * y);
MLvalue * MLgtint(MLvalue * x, MLvalue * y);
MLvalue * MLgeint(MLvalue * x, MLvalue * y);
// string
MLvalue * MLconcat(MLvalue * x, MLvalue *  y);
// pair accessors
MLvalue * MLfst_real(MLvalue * p);
MLvalue * MLsnd_real(MLvalue * p);
// list accessors
MLvalue * MLhd_real(MLvalue * l);
MLvalue * MLtl_real(MLvalue * l);
// print
MLvalue * MLprint(MLvalue * x);

#endif
