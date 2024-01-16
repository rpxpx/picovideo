/* 
 * Number theory.
 * Functions for working with integers and primes.
 *
 */ 

// TO DO:
// switch longs to unsigned longs;


#ifndef _PRIMED_H_
#define _PRIMED_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

typedef unsigned long ulong;


// Integer array
struct inta{
  int *a;
  ulong l;
};

#define INTA_NULL {NULL,0}
#define MAX_PRIME 100000000


int max(int a, int b);
int min(int a, int b);
int fill_boola(bool *a, ulong l, bool v);
ulong count_nonzero(int *a, ulong l);
ulong count_T(bool *a, ulong l);
int print_ints(bool *a, ulong l, ulong start);
int expand_bn(int *exp, int *bn);

int copy_inta(struct inta a, struct inta b);
int prep_inta(struct inta *a, ulong l);
int wipe_inta(struct inta *a);
struct inta clone_inta(struct inta a);
int rndm_one(int lo, int hi);
struct inta rndm_inta(ulong l, int lo, int hi);
struct inta diffs_inta(struct inta a, struct inta b);
float monotonicity(struct inta a);
struct inta insert_inta(struct inta a, int n, ulong p);
struct inta append_inta(struct inta a, int n);
struct inta prepend_inta(struct inta a, int n);
struct inta concat_inta(struct inta a, struct inta b);
int print_inta(struct inta n);

struct inta outer_prod(struct inta a, struct inta b);
struct inta outer_prod_many(struct inta *a, ulong l);

struct inta primez(ulong n);
struct inta decompoze(ulong n);
struct inta divizors(ulong n);

int sort_inta(struct inta a);
int swap_two(int *a, int *b);
int sort_two(int *a, int *b);
int sort_three(int *a, int *b, int *c);
int quicksort_hoare(struct inta a, uint depth);

#endif
