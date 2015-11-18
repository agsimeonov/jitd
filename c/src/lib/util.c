#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cog.h"
#include "cracker.h"
#include "splay.h"
#include "zipf.h"
#include "splay.h"
#include "policy.h"


/**
 * Prints a detailed representation of a single Array or Sorted Array cog.
 *
 * @param cog - cog to print
 */
void printArrayCog(struct cog *cog) {
  printf("[");

  if (cog->type == COG_ARRAY) {
    for (int i = 0; i < cog->data.array.len; i++) {
      int offset = cog->data.array.start;
      printf("%ld", cog->data.array.records->data[i + offset].key);
      if (i + 1 < cog->data.array.len) printf(",");
    }

    printf("]");
  }

  if (cog->type == COG_SORTEDARRAY) {
    for (int i = 0; i < cog->data.sortedarray.len; i++) {
      int offset = cog->data.sortedarray.start;
      printf("%ld", cog->data.sortedarray.records->data[i + offset].key);
      if (i + 1 < cog->data.sortedarray.len) printf(",");
    }

    printf(">");
  }
}

/**
 * Prints a detailed representation of a single Concat or BTree cog.
 *
 * @param cog - cog to print
 */
void printTreeCog(struct cog *cog) {
  if (cog->type == COG_CONCAT) {
    printf("U");
  }

  if (cog->type == COG_BTREE) {
#ifndef __ADVANCED
    printf("≤ %ld", cog->data.btree.sep);
#else
    printf("≤ %ld Reads: %ld", cog->data.btree.sep, cog->data.btree.rds);
#endif
  }
}


/**
 * Prints a detailed representation of a single cog.
 *
 * @param cog - cog to print
 */
void printCog(struct cog *cog) {
  if (cog == NULL) {
    printf("NULL");
  } else {
    if (cog->type == COG_ARRAY || cog->type == COG_SORTEDARRAY) {
      printArrayCog(cog);
    }

    if (cog->type == COG_CONCAT || cog->type == COG_BTREE) {
      printTreeCog(cog);
    }
  }

  printf("\n");
}

/**
 * Prints the internal representation of the JITD providing a detailed layout
 * of the current cogs and data present within.
 *
 * @param cog - the root cog
 * @param depth - depth of the current cog in the tree - set to 0 for root
 */
void printJITD(struct cog *cog, int depth) {
  if (cog == NULL) return;

  if (cog->type == COG_CONCAT || cog->type == COG_BTREE) {
    if (cog->type == COG_CONCAT) {
      if (cog->data.concat.rhs != NULL) {
        printJITD(cog->data.concat.rhs, depth + 1);
      }
    }
    if (cog->type == COG_BTREE) {
      if (cog->data.btree.rhs != NULL) {
        printJITD(cog->data.btree.rhs, depth + 1);
      }
    }

    if (depth != 0) {
      for (int i = 0; i < depth - 1; i++) {
        printf("│   ");
      }
      printf("├───");
    }

    printCog(cog);

    if (cog->type == COG_CONCAT) {
      if (cog->data.concat.lhs != NULL) {
        printJITD(cog->data.concat.lhs, depth + 1);
      }
    }
    if (cog->type == COG_BTREE) {
      if (cog->data.btree.lhs != NULL) {
        printJITD(cog->data.btree.lhs, depth + 1);
      }
    }
  } else {
    if (depth != 0) {
      for (int i = 0; i < depth - 1; i++) {
        printf("│   ");
      }
      printf("├───");
    }

    printCog(cog);
  }
}

/** Prints the current pre-processor mode. */
void printMode() {
#ifndef __ADVANCED
  printf("Classic Mode!\n");
#else
  printf("Advanced Mode!\n");
#endif
}

/**
 * Acquires the count of BTree nodes in a tree.
 *
 * @param cog - the root BTree node in the tree
 * @return the count of BTree nodes in the tree
 */
long getBtreeNodeCount(struct cog *cog) {
  struct cog *left = cog->data.btree.lhs;
  struct cog *right = cog->data.btree.rhs;
  long count = 1;

  if (left != NULL && left->type == COG_BTREE) {
    count += getBtreeNodeCount(left);
  }

  if (right != NULL && right->type == COG_BTREE) {
    count += getBtreeNodeCount(right);
  }

  return count;
}

/**
 * Creates an in-order list of BTree Nodes for a given tree. (step called during recursion).
 *
 * @param cog - root BTree cog
 * @param list - the in-order list
 * @param index - index for next cog in the list
 * @return index for next cog in the list
 */
long inorderStep(struct cog *cog, struct cog **list, long index) {
  struct cog *left = cog->data.btree.lhs;
  struct cog *right = cog->data.btree.rhs;

  if (left != NULL && left->type == COG_BTREE)
    index = inorderStep(left, list, index);

  list[index] = cog;
  index += 1;

  if (right != NULL && right->type == COG_BTREE)
    index = inorderStep(right, list, index);

  return index;
}

/**
 * Creates an in-order list of BTree Nodes for a given tree.
 * NOTE: the in-order list is allocated with malloc, so deallocate with free when done!
 *
 * @param cog - root BTree cog
 * @param count - the count of BTree nodes in the tree
 * @return the in-order list
 */
struct cog **inorder(struct cog *cog, long count) {
  struct cog *left = cog->data.btree.lhs;
  struct cog *right = cog->data.btree.rhs;
  struct cog **list = malloc(count * sizeof(struct cog *));
  long index = 0;

  if (left != NULL && left->type == COG_BTREE)
    index = inorderStep(left, list, index);

  list[index] = cog;
  index += 1;

  if (right != NULL && right->type == COG_BTREE)
    index = inorderStep(right, list, index);

  return list;
}

/**
 * Acquires the median BTree node in a JITD.
 *
 * @param root - the root node of a JITD
 * @return the median node of a JITD
 */
struct cog *getMedian(struct cog *root) {
  long count = getBtreeNodeCount(root);
  struct cog **list = inorder(root, count);
  struct cog *median = list[count/2];
  free(list);
  return median;
}

/**
 * Executes a given function and times the execution.
 *
 * @param function - Function to run
 * @param cog - cog parameter for function
 * @param a - first long parameter for function
 * @param b - second long parameter for function
 * @return the resulting BTree
 */
struct cog *timeRun(struct cog *(*function)(struct cog *, long, long),
                    struct cog *cog,
                    long a,
                    long b) {
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  struct cog *out = (*function)(cog, a, b);
  gettimeofday(&stop, NULL);
  long long startms = start.tv_sec * 1000LL + start.tv_usec / 1000;
  long long stopms = stop.tv_sec * 1000LL + stop.tv_usec / 1000;
  printf("Took %lld milliseconds\n", stopms - startms);
  return out;
}

/**
 * Do a given number of random reads on a cog.
 *
 * @param cog - the given cog
 * @param number - number of reads to do on a cog
 * @param range - the key range for reads
 * @return the resulting BTree
 */
struct cog *randomReads(struct cog *cog, long number, long range) {
  for (long i = 0; i < number; i++) {
    long a = rand() % range;
    long b = rand() % range;
    long low = a <= b ? a : b;
    long high = a > b ? a : b;
    cog = crack(cog, low, high);
  }
  return cog;
}

#ifdef __HARVEST
/**
 * Run a test involving reads and splaying on a harvested value (last value read).
 *
 * @param cog - the root of the JITD
 * @param reads - number of reads per step
 * @param range - the key range for reads
 * @param doSplay - boolean TRUE or FALSE, if TRUE splay after every step, otherwise just read
 * @param steps - number of steps
 * @return the root of the resulting JITD
 */
struct cog *splayOnHarvest(struct cog *cog, long reads, long range, int doSplay, int steps) {
  for (int i = 0; i < steps; i++) {
    cog = timeRun(randomReads, cog, reads, range);
    if (doSplay != FALSE) {
      cog = splay(cog, getHarvest());
    }
  }
  return cog;
}
#endif

#ifdef __ADVANCED
/**
 * Run a test involving reads and splaying on a harvested value (last value read).
 *
 * @param number - count of the number of random reads
 * @param range - the key range for reads
 * @param doSplay - boolean TRUE or FALSE, if TRUE splay after every step, otherwise just read
 * @param steps - number of steps
 * @return the array for the random reads
 */
long *random_array(long number,long range) {
  long *arr = malloc(number * sizeof(long));
  for (long i = 0; i < number; i++) {
    long a = rand() % range;    
    //cog = crack(cog, low, high);
    *(arr+i)=a;
    //printf("Value is %ld\n",a );
  }
  return arr;
}

long *zipfian_array(long number,long range) {
  float alpha = 0.99;
  long *arr = malloc(number * sizeof(long));

  int n = range;
  int zipf_rv;
  rand_val(1400);
  struct cog *cog_median;

  for (int i=0; i<number; i++) {
    zipf_rv = zipf(alpha, n);

    //cog = crack(cog, low, high);
    *(arr+i)=zipf_rv;
    //printf("Value is %ld\n",zipf_rv );
  }
  return arr;
}

struct cog *zipfianReads_splay_array_max_read(struct cog *cog, long number, long range,long *arr) {
  struct cog *cog_max_read;
  long low;  
  for (int i=0; i<number; i++) {
    low = arr[i];
    crack_scan(cog,low,low+range);
    if(i%1000==0) 
      {
        cog_max_read = read_max(cog);
        splay(cog, cog_max_read);
      }
//crack_scan(cog, zipf_rv, zipf_rv + range);
//    if(i > 100) splay(cog, cog_median);
//    printf("%d \n", zipf_rv);
  }

  return cog;
}

struct cog *zipfianReads_splay_array_median(struct cog *cog, long number, long range,long *arr) {
  struct cog *cog_max_read;
  long low;  
  for (int i=0; i<number; i++) {
    low = arr[i];
    crack_scan(cog,low,low+range);
    if(i%200==0) 
      {

        //cog_max_read = read_max(cog);
        splay(cog, getMedian(cog));
      }
//crack_scan(cog, zipf_rv, zipf_rv + range);
//    if(i > 100) splay(cog, cog_median);
//    printf("%d \n", zipf_rv);
  }

  return cog;
}



struct cog *zipfianReads_array(struct cog *cog, long number, long range,long *arr) {
  struct cog *cog_max_read;
  long low;  
  for (int i=0; i<number; i++) {
    low = arr[i];
    crack_scan(cog,low,low+range);    
//crack_scan(cog, zipf_rv, zipf_rv + range);
//    if(i > 100) splay(cog, cog_median);
//    printf("%d \n", zipf_rv);
  }

  return cog;
}



struct cog *randomReads_array(struct cog *cog, long number, long range,long *arr) {
  long low;
  printf("Read count is %ld\n",number);
  for (long i = 0; i < number; i++) {
    
    low = arr[i];
    crack_scan(cog,low,low+range);

  }
  //printJITD(cog, 0);
  return cog;
}

struct cog *randomReads_splay_array_max_read(struct cog *cog, long number, long range,long *arr) {
  struct cog *cog_max_read;
  long low;  
  for (int i=0; i<number; i++) {
    low = arr[i];
    crack_scan(cog,low,low+range);
    if(i%2000==0) 
      {
        cog_max_read = read_max(cog);
        splay(cog, cog_max_read);
      }
//crack_scan(cog, zipf_rv, zipf_rv + range);
//    if(i > 100) splay(cog, cog_median);
//    printf("%d \n", zipf_rv);
  }

  return cog;
}

struct cog *timeRun_array(struct cog *(*function)(struct cog *, long, long, long *),
                    struct cog *cog,
                    long a,
                    long b,
                    long *arr) {
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  struct cog *out = (*function)(cog, a, b,arr);
  gettimeofday(&stop, NULL);
  long long startms = start.tv_sec * 1000LL + start.tv_usec / 1000;
  long long stopms = stop.tv_sec * 1000LL + stop.tv_usec / 1000;
  printf("Took %lld milliseconds\n", stopms - startms);
  return out;
}

#endif
