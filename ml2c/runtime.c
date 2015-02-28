#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
  m->MLint = (x->MLint)+(y->MLint);
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
    m->MLbool=(x->MLint)==(y->MLint);
  }else if(x->type==MLBOOL){
    m->MLbool=(x->MLbool)==(y->MLbool);
  }else if(x->type==MLDOUBLE){
    m->MLbool=(x->MLdouble)==(y->MLdouble);
  }else if(x->type==MLSTRING){
    m->MLbool=(strcmp(x->MLstring,y->MLstring)==0);
  }else if(x->type==MLPAIR){
    MLvalue * v1=MLequal(x->MLpair.MLfst,y->MLpair.MLfst);
    MLvalue * v2=MLequal(x->MLpair.MLsnd,y->MLpair.MLsnd);
    m->MLbool = v1->MLbool && v2->MLbool;
    free(v1);
    free(v2); 
  }else if (x->type==MLLIST){
    if((x->MLlist == NULL) && (y->MLlist==NULL)){
      m->MLbool=1;
      return m;
    }else if ((x->MLlist == NULL) || (y->MLlist==NULL)){
      m->MLbool=0;
      return m;
    }else{
      MLvalue * v1=MLequal(x->MLlist->MLcar, y->MLlist->MLcar);
      MLvalue * v2=MLequal(x->MLlist->MLcdr, y->MLlist->MLcdr);
      m->MLbool = v1->MLbool && v2->MLbool;
      free(v1);
      free(v2); 
    }
  } else {
    m->MLbool=(x->type==MLUNIT) && (y->type==MLUNIT);
  }
  return m;
}

MLvalue * MLltint(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLBOOL;
  m->MLbool = (x->MLint)<(y->MLint);
  return m;
}

MLvalue * MLleint(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLBOOL;
  m->MLbool = (x->MLint)<=(y->MLint);
  return m;

}

MLvalue * MLgtint(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLBOOL;
  m->MLbool = (x->MLint)>(y->MLint);
  return m;
}

MLvalue * MLgeint(MLvalue *x, MLvalue *y){
  MLvalue * m = malloc(sizeof(MLvalue));
  m->type = MLBOOL;
  m->MLbool = (x->MLint)>=(y->MLint);
  return m;
}

MLvalue * MLconcat(MLvalue * x, MLvalue * y){
  MLvalue * m = malloc(sizeof(MLvalue));
  char * s = malloc((strlen(x->MLstring)+strlen(y->MLstring)+1)*sizeof(char));
  int i, len = strlen(x->MLstring);
  for(i=0;x->MLstring[i]!='\0';i++){
    s[i]=x->MLstring[i];
  }
  for(i=0;y->MLstring[i]!='\0';i++){
    s[i+len]=y->MLstring[i];
  }
  m->type=MLSTRING;
  m->MLstring=s;
  return m;
}

MLvalue * MLfst_real(MLvalue * p){
  return p->MLpair.MLfst;
}

MLvalue * MLsnd_real(MLvalue * p){
  return p->MLpair.MLsnd;
}

MLvalue * MLhd_real(MLvalue * l){
  return l->MLlist->MLcar;
}

MLvalue * MLtl_real(MLvalue * l){
  return l->MLlist->MLcdr;
}

MLvalue * MLprint(MLvalue * x){
  int i;
  if(x->type==MLUNIT){
    printf("()");
  }else if(x->type==MLBOOL){
    printf("%s", x->MLbool ? "true" : "false");
  }else if(x->type==MLINT){
    printf("%d",x->MLint);
  }else if(x->type==MLDOUBLE){
    printf("%f",x->MLdouble);
  }else if(x->type==MLSTRING){
    printf("\"%s\"",x->MLstring);
  }else if(x->type==MLPAIR){
    printf("(");
    MLprint(x->MLpair.MLfst);
    printf(",");
    MLprint(x->MLpair.MLsnd);
    printf(")");
  }else if(x->type==MLLIST){
    if(x->MLlist){
      MLprint(x->MLlist->MLcar);
      printf("::");
      MLprint(x->MLlist->MLcdr);
    }else{
      printf("[]");
    }
  }else{
    printf("<fun> [");
    for(i=0;i<(x->MLfun.counter);i++){
      MLprint(&(x->MLfun.env[i]));
    }
    printf("]");
  }
  return MLlrp;
}

void init(){
  MLtrue=malloc(sizeof(MLvalue));
  MLtrue->type=MLBOOL;
  MLtrue->MLbool=1;

  MLfalse=malloc(sizeof(MLvalue));
  MLfalse->type=MLBOOL;
  MLfalse->MLbool=0;

  MLlrp=malloc(sizeof(MLvalue));
  MLlrp->type=MLUNIT;

  MLnil=malloc(sizeof(MLvalue));
  MLnil->type=MLLIST;
  MLnil->MLlist=NULL;
}
