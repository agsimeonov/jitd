#include <stdio.h>

#include "cog.h"


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
