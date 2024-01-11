
#include <stdio.h>

#include "primed.h"



int main(int argc, char *argv[]){
  int *n,l;
  long num;

  if (argc > 1){
    //printf("As str: %s\n", argv[1]);
    num = atol(argv[1]);
    //printf("As int: %ld, %ld\n", num, 2*num);
  }else{
    printf("supply integer at invocation\n");
    return 1;
  }

  printf("int: %ld, long: %ld\n",sizeof(int),sizeof(long));
  int *ta = malloc(sizeof(int) * 100);
  printf("ta: %ld\n",sizeof ta);

  bool *ba = malloc(sizeof(bool)*10);
  fill_boola(ba, 10, 1);
  print_ints(ba,10,1);

  ulong cT = count_T(ba, 10);
  printf("T count: %ld\n", cT);


  int sz = 3;

  struct inta ri0 = rndm_inta(sz,0,9);
  print_inta(ri0);

  struct inta ri1;
  prep_inta(&ri1, ri0.l);
  copy_inta(ri0, ri1);
  print_inta(ri1);

  printf("randone: %d\n", rndm_one(0,10));


  float mono = monotonicity(ri0);
  printf("%f\n", mono);

  struct inta ri2 = insert_inta(ri0,-1,2);
  print_inta(ri2);

  struct inta prms = primez(num);
  printf("primes <=%ld: ",num); print_inta(prms);


  struct inta decomp = decompoze(num);
  printf("prime factorization of %ld: ",num); print_inta(decomp);


  struct inta divs = divizors(num);
  printf("divisors of %ld:\n",num); print_inta(divs);

  int swaps = sort_inta(divs);
  print_inta(divs);

  return 0;



  struct inta rinta = rndm_inta(10,0,99);
  printf("random inta:\n");
  print_inta(rinta);
  printf("monoton: %f\n",monotonicity(rinta));
  sort_inta(rinta);
  print_inta(rinta);
  printf("monoton: %f\n",monotonicity(rinta));

  return 0;

  //printf("bool: %ld int: %ld long: %ld longlong: %ld\n",sizeof(bool),sizeof(int),sizeof(long),sizeof(long long));

  //n = malloc(500);
  //n = NULL;
  //l = primes(&n,num);
  /*
  struct inta p = primez(num);
  if (p.l<0){
    printf("supplied integer limit for prime set out of range\n");
    return 1; 
  }
  print_inta(p);
  free(p.a);
    */

  struct inta bn = decompoze(num);
  print_inta(bn);

  // Initializing pointers...
  char *s = "constant string";

  int *a = (int[]){2,2};
  int *b = (int[]){3,1};
  int *c = (int[]){5,2};
  
  struct inta aa,bb,cc;
  aa.l = *(a+1);
  bb.l = *(b+1);
  cc.l = *(c+1);
  aa.a = malloc(sizeof(int)*aa.l);
  bb.a = malloc(sizeof(int)*bb.l);
  cc.a = malloc(sizeof(int)*cc.l);
  
  expand_bn(aa.a, a);
  expand_bn(bb.a, b);
  expand_bn(cc.a, c);

  print_inta(aa);
  print_inta(bb);
  print_inta(cc);

  /*
  int i;
  // Print exp_a
  for (i=0;i<*(a+1);i++)
    printf("%d ",*(exp_a+i));
  printf("\n");
  //
  // Print exp_b
  for (i=0;i<*(b+1);i++)
    printf("%d ",*(exp_b+i));
  printf("\n");
  */
  
  struct inta op = outer_prod(bb,cc);
  print_inta(op);
  wipe_inta(&op);

  struct inta *opm = malloc(sizeof(struct inta)*3);
  *opm = aa; *(opm+1) = bb; *(opm+2) = cc;

  op = outer_prod_many(opm,3);
//  printf("%ld\n",op.l);
  print_inta(op);

  struct inta tmp;
  tmp = insert_inta(op,1,2);
  wipe_inta(&op);
  op = tmp;
  print_inta(op);

  tmp = prepend_inta(op,3);
  wipe_inta(&op);
  op = tmp;
  print_inta(op);

//  free(op.a);
  wipe_inta(&op);
  //wipe_inta(&tmp);
  print_inta(op);
  wipe_inta(&bn);


  printf("divizors:\n");
  struct inta out = divizors(num);
  print_inta(out);
  sort_inta(out);
  print_inta(out);
  wipe_inta(&out);

  printf("%ld\n", time(NULL));
  srand(time(NULL));
  int r = rand();
  printf("random: %d; rand modulo: %d; max: %d\n",r,r%360,RAND_MAX);


  printf("rndm_one: %d\n",rndm_one(-2,2));

  /*
  struct inta tsort;
  prep_inta(&tsort,10);
  tsort.a[0] = 63;
  tsort.a[1] = 69;
  tsort.a[2] = 60;
  tsort.a[3] = 18;
  tsort.a[4] = 46;
  tsort.a[5] = 53;
  tsort.a[6] = 64;
  tsort.a[7] = 77;
  tsort.a[8] = 66;
  tsort.a[9] = 82;
  
  print_inta(tsort);
  int swps;
  swps = sort_inta(tsort);
  printf("SWAPS: %d\n",swps);
  print_inta(tsort);
  */



  return 0;
}

