#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cog.h"
#include "cracker.h"


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
    printf("≤ %ld", cog->data.btree.sep);
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
long inorderStep(struct cog *cog, struct cog **list, int index) {
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
 * @return the in-order list
 */
struct cog **inorder(struct cog *cog) {
  struct cog *left = cog->data.btree.lhs;
  struct cog *right = cog->data.btree.rhs;
  struct cog **list = malloc(getBtreeNodeCount(cog) * sizeof(struct cog *));
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
