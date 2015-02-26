#include <stdlib.h>
#include <stdio.h>

#include "runtime.h"


MLvalue * new_MLint(int x){
  MLvalue * n = malloc(sizeof(MLvalue));
  n->type=MLINT;
  n->MLint=x;
  return n;
}

MLvalue * new_MLdouble(double x){
  MLvalue * n = malloc(sizeof(MLvalue));
  n->type=MLDOUBLE;
  n->MLdouble=x;
  return n;
}

MLvalue * new_MLbool(int b){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type=MLBOOL;
  m->MLbool=b;
  return m;
}

MLvalue * new_MLstring(char *s){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type=MLSTRING;
  m->MLstring=s;
  return m;
}

MLvalue * MLpair(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type=MLPAIR;
  m->MLpair.MLfst=x;
  m->MLpair.MLsnd=y;
  return m;
}

MLvalue * MLlist(MLvalue *x, MLvalue*y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type=MLLIST;
  m->MLlist=malloc(sizeof(struct _mllist_));
  m->MLlist->MLcar=x;
  m->MLlist->MLcdr=y;
  return m;
}

MLvalue * MLaddint(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLINT;
  m->MLint = x->MLint+y->MLint;
  return m;
}

MLvalue * MLsubint(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLINT;
  m->MLint = x->MLint-y->MLint;
  return m;
}

MLvalue * MLmulint(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLINT;
  m->MLint = x->MLint*y->MLint;
  return m;
}

MLvalue * MLdivint(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLINT;
  m->MLint = x->MLint/y->MLint;
  return m;
}

MLvalue * MLequal(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLBOOL;
  if(x->type==MLINT){
    m->MLBOOL=x->MLint==y->MLint;
  }else if(x->type==MLBOOL){
    m->MLBOOL=x->MLbool==y->MLbool;
  }else if(x->type==
}
