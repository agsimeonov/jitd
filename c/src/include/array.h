#ifndef ARRAY_LIB_H_SHIELD
#define ARRAY_LIB_H_SHIELD

long *random_array(long number,long range);

struct cog *randomReads_array(struct cog *cog, long number, long range,long *arr,long interval) ;
struct cog *randomReads_splay_array_max_read(struct cog *cog, long number, long range,long *arr,long interval) ;

long *zipfian_array(long number,long range);
long *zipfian_scrambled_array(long number,long range);
void *splay_level(struct cog *root, long levels);

struct cog *zipfianReads_splay_array_max_read(struct cog *cog, long number, long range,long *arr,long interval);
struct cog *zipfianReads_splay_array_median(struct cog *cog, long number, long range,long *arr,long interval);
struct cog *zipfianReads_array(struct cog *cog, long number, long range,long *arr,long interval) ;

#endif
