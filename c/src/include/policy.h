#ifndef POLICY_LIB_H_SHIELD
#define POLICY_LIB_H_SHIELD

#include "cog.h"

#ifdef __ADVANCED
/** Contains the root of the tree after zipfinize as well as the number of splayed nodes. */
typedef struct zipcog {
  struct cog *cog; // root of the tree after zipfinize
  long count; // number of nodes that were splayed during zipfinizing
} zipcog;

long splay_count;
/**
 * Return the cog with the highest read
 * @param cog - root of the tree
 * @return the cog with the max read
 *
 * */
cog *read_max( struct cog *cog);


/**
 * Moves up nodes into the given number of levels so that the resulting tree has close to a
 * Zipfian distribution for the given levels based on the number of reads.
 *
 * @param cog - root of the tree
 * @param levels - given number of levels
 * @return the new root of the tree and the number of nodes that were moved
 */
struct zipcog *zipfinize(struct cog *cog, long levels);

/**
 * Decays the tree by the given factor. For example, if the factor is x the read count for every
 * node in the tree will be reduced as such: floor(reads/x)
 *
 * @param cog - given tree
 * @param factor - decay factor
 * @return root of the given tree
 */
struct cog *decay(struct cog *cog, long factor);

/**
 * Initializes the interval for running the policy.
 *
 * @param interval - initial interval for the policy
 * @param low - threshold - number of moves signifying a need to decrease the interval
 * @param high - threshold - number of moves signifying a need to increase the interval
 */
void initPolicyInterval(long interval, long low, long high);

/**
 * Acquires the current interval for running the policy.  This interval is self adjusting.
 * Based on the initialized thresholds it will either increase or decrease.
 * (For now we can double/half the interval however we should consider smarter ways to adjust it
 * one of the suggested approaches is looking at Huffman trees as their operations could somehow
 * help us figure out an interesting (smarter) way to do this).
 *
 * @return the next interval
 */
long getCurrentInterval();

/**
 * Updates the policy interval.
 *
 * @param - the policy interval
 */
void updatePolicyInterval(long interval);


/**
 *Do an inorder traversal and print the cogs line by line
 *
 *
 **/

 void inOrderTraversal(struct cog *cog);

#endif

#endif
