#ifndef ZIPFIAN_DISTRIBUTION_H
#define ZIPFIAN_DISTRIBUTION_H 
#define  FALSE          0       // Boolean false
#define  TRUE           1       // Boolean true

//----- Function prototypes -------------------------------------------------
int      zipf(double alpha, int n);  // Returns a Zipf random variable
double   rand_val(int seed);         // Jain's RNG

#endif


