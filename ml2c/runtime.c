#include <stdlib.h>
#include <stdio.h>

#include "runtime.h"

int main() {
  printf("%d\n", ((MLvalue){MLINT, {8 - 2}}).MLint);
  return 0;
}
