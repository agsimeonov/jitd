#ifndef ZIPF_LIB_H_SHIELD
#define ZIPF_LIB_H_SHIELD

#define FALSE 0 // Boolean false
#define TRUE  1 // Boolean true

/**
 * Acquire a Zipf random variable.
 *
 * @param alpha - rate of decay (exponent of the denominator must be greater than 0)
 * @param n - range (positive integers)
 * @return a Zipf random variable
 */
int zipf(double alpha, int n);

/**
 * Jain's random number generator.
 *
 * @param seed - seed
 * @return a random value between 0.0 and 1.0
 */
double rand_val(int seed);

#endif
