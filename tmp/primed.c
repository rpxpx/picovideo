
#include "primed.h"


int max(int a, int b){
  if (b>a)
    return b;
  return a;
}

int min(int a, int b){
  if (b<a)
    return b;
  return a;
}

int fill_boola(bool *a, ulong l, bool v){
// Fill boolean array a with value v.
// a assumed to have been malloc'd enough memory.
  do{
    l--;
    *(a+l) = v;
  }while (l>0);
  return 0;
}

ulong count_nonzero(int *a, ulong l){
  ulong c = 0;
  do{
    l--;
    if (*(a+l)>0)
      c++; 
  }while (l>0);
  return c;
}

ulong count_T(bool *a, ulong l){
  ulong c = 0;
  do{
    l--;
    if (*(a+l))
      c++; 
  }while (l>0);
  return c;
}

int print_ints(bool *a, ulong l, ulong start){
// Pass pointer to boolean array and length; print corresponding integers
  ulong i=0; 
  for (;i<l;i++)
    if (*(a+i))
      printf("%ld ",i+start);
  printf("\n");
  return 0;
}

// Expand 2 element bn (base-exponent) integer array into exp integer array: all possible divisors of b^n
int expand_bn(int *exp, int *bn){
  uint i;
  for (i=0;i<*(bn+1);i++)
    *(exp+i) = pow(*(bn+0), i+1); 
  return 0;
}


/* INTA struct integer array functions */

// Copy a to b, into mem at passed pointer; 
// assume correct size allocated, and b.l correctly set
int copy_inta(struct inta a, struct inta b){
  if (a.l<1)
    return 1;
  do{
    a.l--;
    *(b.a+a.l) = *(a.a+a.l);
  }while (a.l>0);
  return 0;
}

int prep_inta(struct inta *a, ulong l){
  if (l==0){
    (*a).a = NULL;
    (*a).l = 0;
  }else{
    (*a).a = malloc(sizeof(int)*l);
    (*a).l = l;
  }
  return 0;
}

int wipe_inta(struct inta *a){
  if ((*a).l<1){
    return 1;
  }
  free((*a).a);
  (*a).a = NULL;
  (*a).l = 0;
  return 0;
}

struct inta clone_inta(struct inta a){
// Clone passed inta into new inta, returned
  struct inta out = INTA_NULL;
  if (a.l<1)
    return out;
  prep_inta(&out,a.l);
  copy_inta(a,out);
  return out;
 }

int rndm_one(int lo, int hi){
  long s = hi-lo+1; // Range inclusive.
  if (s>RAND_MAX)
    return lo-1;
  srand(time(NULL)); // Seed with Unixtime.
  return rand()%s+lo;
}

struct inta rndm_inta(ulong l, int lo, int hi){
  struct inta out = INTA_NULL;
  long s = hi-lo+1; // Range inclusive.
  if (l<1 || s>RAND_MAX)
    return out;

  srand(time(NULL)); // Seed rand with Unixtime.
  prep_inta(&out,l);
  do{
    l--;
    *(out.a+l) = rand()%s+lo;
  }while (l>0);
  return out;
}

// Return diffs: subtract vals of one inta from another: a-b.
struct inta diffs_inta(struct inta a, struct inta b){
  struct inta out = INTA_NULL;
  if (a.l>0 && a.l==b.l){
    prep_inta(&out,a.l);
    do{
      a.l--;
      *(out.a+a.l) = *(a.a+a.l)-*(b.a+a.l);
    }while (a.l>0);
  }
  return out;
}


// Return monotonicity measure.
// Fraction of summed step rises over summed step falls
// +1 for perfect monotonicity; -1 for perfect anti-monotonicty; 0 for equal values
float monotonicity(struct inta a){
  struct inta b,c;
  b.a = a.a+1;
  b.l = --a.l;
  c = diffs_inta(b,a);
  print_inta(c);
  // Sum +ve and -ve values separately.
  float ups,dns;
  ups = dns = 0;

  do{
    c.l--;
    if (*(c.a+c.l)>0)
      ups += *(c.a+c.l);
    if (*(c.a+c.l)<0)
      dns -= *(c.a+c.l);
  }while (c.l>0);
  
  //printf("ups: %f, dns: %f\n",ups,dns);
  if (ups>dns)
    return 1-dns/ups;
  if (dns>ups)
    return ups/dns-1;
  return 0; 
}
// Can't decide whether this sort of func shld return a new inta or mod one passed;
// Copying what happens in numpy for now;


struct inta insert_inta(struct inta a, int n, ulong p){
  struct inta out = INTA_NULL;
  if (p>a.l)
    return out;
  out.l = a.l+1;
  out.a = malloc(sizeof(int)*out.l);
  ulong i;
  for (i=0;i<p;i++)
    *(out.a+i) = *(a.a+i);
  *(out.a+i) = n;
  for (;i<a.l;i++)
    *(out.a+i+1) = *(a.a+i);
  return out;
}


struct inta append_inta(struct inta a, int n){
  return insert_inta(a,n,a.l);
}

struct inta prepend_inta(struct inta a, int n){
  return insert_inta(a,n,0);
}


struct inta concat_inta(struct inta a, struct inta b){
  struct inta out = INTA_NULL;
  return out;
}

int print_inta(struct inta n){
// Pass inta struct w int pointer and length; print contents; 
  if (n.l<1){
    printf("Null inta\n");
    return 1;
  }
  for (ulong i=0;i<n.l;i++)
    printf("%d ",*(n.a+i));
  printf("\n");
  return 0;
}


// New int array as outer product of two int arrays
struct inta outer_prod(struct inta a, struct inta b){
  struct inta out = INTA_NULL;
  if (a.l<1 || b.l<1)
    return out;
  
  out.l = a.l*b.l;
  out.a = malloc(sizeof(int)*out.l);
  for (ulong i=0;i<a.l;i++)
    for (ulong j=0;j<b.l;j++)
      *(out.a+(i*b.l + j)) = *(a.a+i) * *(b.a+j);  
  return out;
}


// outer_prod for >2 int arrays. Work from RHS->LHS
struct inta outer_prod_many(struct inta *a, ulong l){
  struct inta out = INTA_NULL;

  // Trivial cases
  if (l==0 || a[0].l==0){
    //printf("Null input\n");
    return out;
  }
  if (l==1){
    //printf("One array input\n");
    out = clone_inta(a[0]); 
    return out;
  }

  l--;
  struct inta t = clone_inta(*(a+l));
  do{
    l--;
    out = outer_prod(*(a+l), t); 
    free(t.a);
    t = out;
  }while(l>0);
  return out;
}



// Find all primes <= n.
// Sieve of Eratosthenes.
struct inta primez(ulong n){
  ulong i,j;
  bool *sieve;
  int *prms;
  struct inta out = INTA_NULL;

  if (n>MAX_PRIME){
    return out;
  }

  // Create sieve first with Boolean values.
  sieve = (bool*) malloc(sizeof(bool)*(n-1));
  fill_boola(sieve, n-1, 1);

//  printf("printing ints\n");
//  print_ints(sieve, n-1, 2);
  
  i = 0;
  for (;i<(n-1);){
    // Advance to next true element within the bool array;
    for(;sieve[i]==0 && i<(n-1);i++)
      ;
    // Set false all >2 multiples of i   
    for(j=2;((i+2)*j)<=n;j++){
      sieve[(i+2)*j-2]=0;
    } 
    i++;
  }

  //print_ints(sieve, n-1, 2);

  // Convert boolean array into integer array.
  // Length of int array == number of T values in bool array
  ulong l = count_T(sieve, n-1);
  
  prms = (int*) malloc(sizeof(int)*l);
  for (i=j=0;i<(n-1);i++)
    if (*(sieve+i)){
      *(prms+j)=i+2;
      j++;
    }

  free(sieve);
  struct inta p = {prms,l};
  return p;
}


// Prime decomposition of int n.
// Returning: integer array struct of prime/exponent pairs.
struct inta decompoze(ulong n){
  
  // First create set of all primes <=n
  struct inta p = primez(n);
  // Initialize base-power array to null.
  struct inta bn = INTA_NULL;
  if (p.l==0){
   return bn;
  }
  
  // Check for trivial solution first: n is prime: n is final integer in the prime set, p.
  if (n==*(p.a+p.l-1)){
    bn.a = (int*) malloc(sizeof(int)*2);
    *(bn.a) = n;
    *(bn.a+1) = 1;
    bn.l = 2;
    return bn;
  }

  // n is not prime: n is a product of exponentiated primes.
  // Find exponents; count nonzero exponents.
  int *exp = (int*) malloc(sizeof(int)*p.l); //array for exponents of primes
  int k = 0; //counter for nonzero exponents
  int i = 0;
  for (i=0; n>1; i++){
    *(exp+i) = 0;
    while (n%*(p.a+i)==0){
      *(exp+i) += 1;
      n /= *(p.a+i);
    }
    if (*(exp+i))
      k++;
  }
  // i is now length of exponent array.
  // k is number of nonzero exponents.

  // Splice primes and exponents into a single array to return.
  bn.a = (int*) malloc(sizeof(int)*2*k);

  bn.l = 2*k;
  int j,m;
  for(j=m=0;j<i;j++){
    if (*(exp+j)){
      *(bn.a+2*m) = *(p.a+j);
      *(bn.a+(2*m+1)) = *(exp+j);
      m++;
    }
  }
 
  free(p.a);
  free(exp);
  return bn;
}


// Return integer array of all divisors of n
struct inta divizors(ulong n){
  
  // First find prime decomposition of n.
  struct inta bn = decompoze(n);
  
  // Create an array of intas, length of bn decomp/2. 
  // Expand bn decomp into this. 
  // Prepend 1s. Pass to outer_prod_many().
  // Set of all divisors is just this outer product.
  //
  // Expand bn pairs.
  struct inta expnd[bn.l/2]; //bn.l/2 is the number of unique primes.
  uint i;
  for (i=0;i<bn.l/2;i++){
    expnd[i].l = *(bn.a+2*i+1)+1; //+1 for prepended 1
    expnd[i].a = malloc(sizeof(int)*expnd[i].l); 
    *(expnd[i].a) = 1;
    expand_bn((expnd[i].a+1), (bn.a+2*i));
    //print_inta(expnd[i]);
  }
  
  struct inta out = INTA_NULL;
  out = outer_prod_many(expnd, bn.l/2);
  
  // Free up mem in expnd before exiting.
  for (i=0;i<bn.l/2;i++)
    free(expnd[i].a);

  return out;
}


/* SORT ALOGORITHMS */
// merge sort, quicksort, et al
// Return number of swaps.

int sort_inta(struct inta a){
  return quicksort_hoare(a,0);
}
//
// Trivial sorts: used in complex sorts.
int swap_two(int *a, int *b){
  int t;
  t = *a; *a = *b; *b = t;
  return 1;
}
int sort_two(int *a, int *b){
  if (*a>*b)
    return swap_two(a,b); 
  return 0;
}
int sort_three(int *a, int *b, int *c){
  int swps = 0;
  swps += sort_two(a,c);
  swps += sort_two(b,c);
  swps += sort_two(a,b);
  return swps;
}

// QUICKSORT: Median pivot, Hoare partitioning, recursive.
int quicksort_hoare(struct inta a, uint depth){
  //
  // Nothing to do
  if (a.l<2)
    return 0;
  
  // Trivial cases
  if (a.l==2){
    return sort_two(a.a,a.a+1);
  }
  if (a.l==3){
    return sort_three(a.a,a.a+1,a.a+2);  
  }
   
  // For inta length>3, find pivot and parition. 
  //
  // Select pivot value for array.
  // Median pivot: pick three values, ends and middle; sort in situ, pick middle value as pivot.

  long swps;
  long lo,hi;
  lo = 0; hi = a.l-1; 
  // Sort start,end,middpoint. Take middpoint as pivot. Push partition markers inwards.
  swps = sort_three(a.a+lo++, a.a+a.l/2, a.a+hi--);
  int piv = *(a.a+a.l/2);

  // Slide inwards, swapping vals around pivot. 
  for (;lo<hi;){
    if (*(a.a+lo)>=piv){
      if (*(a.a+hi)<piv){
        swps += swap_two(a.a+lo++,a.a+hi--);
      }else{
        hi--;
      }
    }else{
      lo++; 
    }
  }
  
  // Partition point found; recursively submit non-zero length subpartitions for sorting.
  // LH partition: 0-->hi
  if (hi>0){
    struct inta b = a;
    b.l = hi+1;
    swps += quicksort_hoare(b,depth+1);
  }
  // RH partition: lo-->a.l-1
  if (lo<a.l-1){
    struct inta c;
    c.a = a.a+lo;
    c.l = a.l-lo;
    swps += quicksort_hoare(c,depth+1);
  }

  //print_inta(a);
  return swps;
}

