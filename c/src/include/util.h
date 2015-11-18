#ifndef UTIL_LIB_H_SHIELD
#define UTIL_LIB_H_SHIELD

#include "cog.h"


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
 * @param count - the count of BTree nodes in the tree
 * @return the in-order list
 */
struct cog **inorder(struct cog *cog, long count);

/**
 * Acquires the median BTree node in a JITD.
 *
 * @param root - the root node of a JITD
 * @return the median node of a JITD
 */
struct cog *getMedian(struct cog *root);

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
struct cog *splayOnHarvest(struct cog *cog, long reads, long range, int doSplay, int steps);
#endif

#ifdef __ADVANCED
long *random_array(long number,long range);

struct cog *randomReads_array(struct cog *cog, long number, long range,long *arr) ;

struct cog *randomReads_splay_array_max_read(struct cog *cog, long number, long range,long *arr) ;

struct cog *timeRun_array(struct cog *(*function)(struct cog *, long, long, long *),
                    struct cog *cog,
                    long a,
                    long b,
                    long *arr) ;

long *zipfian_array(long number,long range);

struct cog *zipfianReads_splay_array_max_read(struct cog *cog, long number, long range,long *arr);
struct cog *zipfianReads_splay_array_median(struct cog *cog, long number, long range,long *arr);
struct cog *zipfianReads_array(struct cog *cog, long number, long range,long *arr) ;

#endif

#endif
