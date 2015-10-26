#include "cog.h"

#ifndef UTIL_LIB_H_SHIELD
#define UTIL_LIB_H_SHIELD


/**
 * Prints the internal representation of the JITD providing a detailed layout
 * of the current cogs and data present within.
 *
 * @param cog - the root cog
 * @param depth - depth of the current cog in the tree - set to 0 for root
 */
void printJITD(struct cog *c, int depth);

/** Prints the current pre-processor mode. */
void printMode();

/**
 * Acquires the count of BTree nodes in a tree.
 *
 * @param cog - the root BTree node in the tree
 * @return the count of BTree nodes in the tree
 */
long getBtreeNodeCount(struct cog *cog);

/**
 * Creates an in-order list of BTree Nodes for a given tree.
 * NOTE: the in-order list is allocated with malloc, so deallocate with free when done!
 *
 * @param cog - root BTree cog
 * @return the in-order list
 */
struct cog **inorder(struct cog *cog);

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
                    long b);

/**
 * Do a given number of random reads on a cog.
 *
 * @param cog - the given cog
 * @param number - number of reads to do on a cog
 * @param range - the key range for reads
 * @return the resulting BTree
 */
struct cog *randomReads(struct cog *cog, long number, long range);

#endif
