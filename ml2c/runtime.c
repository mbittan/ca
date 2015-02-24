#include <stdlib.h>
#include <stdio.h>

#include "runtime.h"


MLvalue new_MLint(int x){
  MLvalue n;
  n.type=MLINT;
  n.MLint=x;
  return n;
}

MLvalue new_MLdouble(double x){
  MLvalue n;
  n.type=MLDOUBLE;
  n.MLdouble=x;
  return n;
}

MLvalue new_MLbool(int b){
  MLvalue m;
  m.type=MLBOOL;
  m.MLbool
}
