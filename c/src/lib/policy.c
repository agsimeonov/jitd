#include "cog.h"
#include <stdlib.h>

#ifdef __ADVANCED

typedef enum {
	  LEFT, CURRENT, RIGHT
} ret_cog;



/**
 * Moves up nodes into the given number of levels so that the resulting tree has close to a
 * Zipfian distribution for the given levels based on the number of reads.
 *
 * @param cog - root of the tree
 * @param levels - given number of levels
 * @return the new root of the tree and the number of nodes that were moved
 */
struct zipcog *zipfinize(struct cog *cog, long levels) {
  // TODO: Alok

  return 0;
}

/**
 * Return the cog with the highest read
 * @param cog - root of the tree
 * @return the cog with the max read
 *
 * */
cog *read_max( struct cog *cog){
	
	long original_rdc,left_rdc,right_rdc;


	if(cog->type ==COG_BTREE)
	{
	struct cog *left = cog->data.btree.lhs;
	struct cog *right = cog->data.btree.rhs;

	if(left!=NULL && left->type == COG_BTREE)
		left_rdc = left->data.btree.rds;
	else
		left_rdc = 0;

	if(right!=NULL && right->type == COG_BTREE)
		right_rdc = right->data.btree.rds;
	else
		right_rdc = 0;

	ret_cog type;
	
	//printf("Read count is %ld\n",cog->data.btree.rds );	
	original_rdc = cog->data.btree.rds-(left_rdc +right_rdc );
	//printf("Root read count is %ld\n",original_rdc );

	if(left_rdc>=right_rdc)
		type = LEFT;
	else
		type = RIGHT;


	if((original_rdc>left_rdc) && (original_rdc>right_rdc))
	{
		return cog;

	}
	else
	{

		if(type==LEFT)
			return(read_max(left));
		else
			return(read_max(right));
	}
	}
	else 
		return NULL;

}


/**
 * Decays the tree by the given factor. For example, if the factor is x the read count for every
 * node in the tree will be reduced as such: floor(reads/x)
 *
 * @param cog - given tree
 * @param factor - decay factor
 * @return root of the given tree
 */
struct cog *decay(struct cog *cog, long factor) {

  // TODO: Archana
	struct cog *left = cog->data.btree.lhs;
	struct cog *right = cog->data.btree.rhs;

	cog->data.btree.rds/=factor;

	if (left != NULL && left->type == COG_BTREE)
		decay(left, factor);

	if (right != NULL && right->type == COG_BTREE)
		decay(right, factor);
  
  return 0;
}

/**
 * Initializes the interval for running the policy.
 *
 * @param interval - initial interval for the policy
 * @param low - threshold - number of moves signifying a need to decrease the interval
 * @param high - threshold - number of moves signifying a need to increase the interval
 */
void initPolicyInterval(long interval, long low, long high) {
  // TODO: Alok/Archana
}

/**
 * Acquires the current interval for running the policy.  This interval is self adjusting.
 * Based on the initialized thresholds it will either increase or decrease.
 * (For now we can double/half the interval however we should consider smarter ways to adjust it
 * one of the suggested approaches is looking at Huffman trees as their operations could somehow
 * help us figure out an interesting (smarter) way to do this).
 *
 * @return the next interval
 */
long getCurrentInterval() {
  // TODO: Alok/Archana
  return 0;
}

/**
 * Updates the policy interval.
 *
 * @param - the policy interval
 */
void updatePolicyInterval(long interval) {
  // TODO: Alok/Archana
}

/**
 * Updates the policy interval.
 *
 * @param - the policy interval
 */

#endif
