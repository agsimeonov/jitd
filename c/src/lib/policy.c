#include "cog.h"
#include <stdlib.h>

#ifdef __ADVANCED

typedef enum {
	  LEFT, CURRENT, RIGHT
} ret_cog;


long Splay_interval;
long Lower_count;
long Higher_count;



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
 * Moves up nodes into the given number of levels so that the resulting tree has close to a
 * Zipfian distribution for the given levels based on the number of reads.
 *
 * @param cog - root of the tree
 * @param levels - given number of levels
 * @return the new root of the tree and the number of nodes that were moved
 */
/*struct zipcog *zipfinize(struct cog *cog, long levels) {
  // TODO: Alok

	//cog_max_read = read_max(root);
        //printf("Read max count is : %ld\n", cog_max_read->data.btree.rds);
    //splay(cog, cog_max_read);
    struct cog *cog_max_read, *cog_left, *cog_right;

    struct zipcog *zip,*left_zip, *right_zip;
    zip = malloc(sizeof(struct zipcog));
    zip->cog = malloc(sizeof(struct cog));
    left_zip = malloc(sizeof(struct zipcog));
    left_zip->cog = malloc(sizeof(struct cog));
    right_zip = malloc(sizeof(struct zipcog));
    right_zip->cog = malloc(sizeof(struct cog));
    zip->cog = cog;
    zip->count =0;
    if(levels==0)
    	return 0;
    else{

    	cog_max_read = read_max(cog);
    	if(cog_max_read!=zip.cog)
    	{
    		//No splay required
    		zip->count+=1;
    		zip->cog = splay(cog, cog_max_read);
    	}

        
        cog_left = cog->data.btree.lhs;
        cog_right = cog->data.btree.rhs;
        lef_zip = zipfinize(cog_left,levels--);
        zip->count+=left_zip->count;
        right->zip = zipfinize(cog_right,levels--);
        zip+=right_zip->count;
    }  

  return zip;
}*/

/*int zipftree(struct cog *cog, int levels)
{
	struct cog *cog_max_read, *cog_left,*cog_right,*root;
	 if(levels==0)
    	return 0;
    else{

    	cog_max_read = read_max(cog);
    	if(cog_max_read!=cog)
    	{
    		//No splay required
    		Splay_count+=1;
    		//root=splay(cog, cog_max_read);
    	}

        
        cog_left = cog->data.btree.lhs;
        cog_right = cog->data.btree.rhs;
        if(cog_left!=NULL && cog_left->type == COG_BTREE)
        	zipftree(cog_left,levels--);        
        if(cog_right!=NULL && cog_right->type == COG_BTREE)
        	zipftree(cog_right,levels--);        
    }  

    printf("Prerformed : %ld \n",Splay_count);
  return 0;

}*/


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
	Splay_interval = interval;
	Lower_count = low;
	Higher_count = high;
	
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
  return Splay_interval;
}

/**
 * Updates the policy interval.
 *
 * @param - the policy interval
 */
void updatePolicyInterval(long interval) {
  // TODO: Alok/Archana
	Splay_interval = interval;
}

/**
 * Perform the splay recursively on the subtrees
 * @param cog - root cog
 * @param levels - levels of interest
 * @return - root of the cog
 */

void *splay_level(struct cog *root, long levels){

	struct cog *int_cog,*left,*right,*cog_max_read;

	if(levels>=0)	
	{
		cog_max_read = read_max(root);
		printf("Max read cog is %p\n",cog_max_read);
		
		printf("Splaying at level :%d\n",levels);
		printf("Before splaying root address %p\n",root);
		int_cog = splay(root,cog_max_read);
		printf("After splaying root address %p\n",int_cog);
		left = int_cog->data.btree.lhs;
		right = int_cog->data.btree.rhs;
		printf("Left splaying root address %p\n",left);
		printf("Right splaying root address %p\n",right);
		


	if(left!=NULL && left->type == COG_BTREE)
		{
			printf("Going to the left subtree\n\n");
			splay_level(left,--levels);
		}
	else
		return;

	if(right!=NULL && right->type == COG_BTREE)
		{
			printf("Going to the right subtree\n\n");
			splay_level(right,--levels);	
		}
	else
		return;
	}
	else
		return;

}

#endif
