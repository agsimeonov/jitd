#include <stdio.h>

#include "array.h"
#include "policy.h"
#include "zipf.h"
#include "util.h"

long *zipfian_array(long number,long range) {
  float alpha = 0.99;
  long *arr = malloc(number * sizeof(long));

  int n = range;
  int zipf_rv;
  rand_val(1400);
  for (int i=0; i<number; i++) {
    zipf_rv = zipf(alpha, n);
    *(arr+i)=zipf_rv;
  }
  return arr;
}

long *zipfian_scrambled_array(long number,long range) {
  float alpha = 0.99;
  long *arr = malloc(number * sizeof(long));

  FILE *file;
  char *line = NULL;
  int len = 0;
  char read;
  file=fopen("test_scramble", "r");
  char * e;

  if (file == NULL)
    return 1;
  long i=0;
  while ((read = getline(&line, &len, file)) != -1) {    
     if(i<number) {
    *(arr+i)=strtol(line, &e, 0);
      }
      else
        break;
      i++;
    }
  //printf("The return data size is %ld\n",i);
  return arr;
}



struct cog *zipfianReads_splay_array_max_read(struct cog *cog, long number, long range,long *arr,long interval) {
  struct cog *cog_max_read;
  long low;
  double alpha = 1;
  long elements = 1000000;
  long count = getZipfCountAtCDF(elements, alpha, .5);
  long levels = getNumberOfLevels(count);
  //long levels = 4;
  printf("Number of levels is : %ld\n",levels);  
  printf("Interval is %ld\n",getInterval());
  interval = getInterval();
  //int levels =2;
  splay_count=0;
  long next_interval;
  next_interval= getInterval();
  //double alpha = 1;
  //long range =1000000;
  printf("Next interval is %li\n",next_interval);
  for (long i=0; i<number; i++) {
    low = arr[i];
    long a = arr[i];
    long b = arr[i+1];    
    long low = a <= b ? a : b;
    long high = a > b ? a : b;
    //printf("Cracking at %li - %li \n",low,high);

    cog = crack_scan(cog,low,high);
    //if(i==200)
     // jsonJITD(cog,"before.json");

    //printf("Interval is \n",next_interval);
    if(i==next_interval) 
      {      
        //printJITD(cog,0);
        cog=zipfinize(cog, levels);
        /*if(splay_count > 10 && interval>100)
          {
            interval = interval/2;
          }
        else if(splay_count <5)
        {
            interval = interval+100;
        }
        splay_count=0;*/

        //printf("Zipfinise called\n");
        next_interval = next_interval+getInterval();
        //printf("Interval to splay is %ld\n",getInterval());
        printf("Next interval to splay is %ld\n",next_interval);
        //printJITD(cog,0);
      }
      //if(i==201)
       // jsonJITD(cog,"after.json");
      //printf("The new interval is %d\n",getInterval());

      }
    //jsonJITD(cog,"before.json");
    //cog=zipfinize(cog, levels);
    //jsonJITD(cog,"after.json");

  return cog;
}

struct cog *zipfianReads_splay_array_median(struct cog *cog, long number, long range,long *arr,long interval) {
  struct cog *cog_max_read;
  long low;
  //splay_count=0;  
  long elements = 10000000;
  double alpha = 1;
  long count = getZipfCountAtCDF(elements, alpha, .5);
  long levels = getNumberOfLevels(count);
  interval = getInterval();
  long next_interval = interval;

  for (long i=0; i<number; i++) {
    long a = arr[i];
    long b = arr[i+1];    
    long low = a <= b ? a : b;
    long high = a > b ? a : b;
    cog = crack_scan(cog,low,high);
    //crack_scan(cog,low,low+range);
    //printf("Cracking at %li - %li \n",low,high);

    if(i==next_interval) 
      {
        //cog=zipfinize(cog, levels);
        cog = splay_median(cog);      
        next_interval = next_interval+getInterval();
        
      }
  }

  return cog;
}



struct cog *zipfianReads_array(struct cog *cog, long number, long range,long *arr,long interval) {
  struct cog *cog_max_read;
  long low;
  double alpha = 1;
  //long range =1000000;  
  for (long i=0; i<number; i++) {
    long a = arr[i];
    long b = arr[i+1];
    //fprintf(stderr, "%s\n", );
    //printf("Range is %li\n",range );
    
    
    long low = a <= b ? a : b;
    long high = a > b ? a : b;
    //printf("Cracking at %li - %li \n",low,high);

    cog=crack_scan(cog,low,high);   
  }

  return cog;
}



struct cog *randomReads_array(struct cog *cog, long number, long range,long *arr,long interval) {
  long low;
  printf("Read count is %ld\n",number);
  for (long i = 0; i < number; i++) {
    
    low = arr[i];
    cog = crack_scan(cog,low,low+range);

  }
  return cog;
}

struct cog *randomReads_splay_array_max_read(struct cog *cog, long number, long range,long *arr,long interval) {
  struct cog *cog_max_read;
  long low;
  splay_count=0;
  double alpha = 1;
  long elements = 1000000;
  long count = getZipfCountAtCDF(elements, alpha, .5);
  long levels = getNumberOfLevels(count);
  printf("Number of levels is : %ld\n",levels);
  //long levels=2;  
  for (long i=0; i<number; i++) {
    low = arr[i];
    crack_scan(cog,low,low+range);
    if(i%interval==0) 
      {        
        cog=zipfinize(cog, levels);
        if(splay_count > 10 && interval>100)
          {
            interval = interval/2;
          }
        else if(splay_count <5)
        {
            interval = interval+100;
        }
        splay_count=0;
      }
  }

  return cog;
}

struct cog *randomReads_splay_array_median(struct cog *cog, long number, long range,long *arr,long interval) {
  struct cog *cog_max_read;
  long low;
  splay_count=0;  
  for (long i=0; i<number; i++) {
    low = arr[i];
    crack_scan(cog,low,low+range);
    if(i%interval==0) 
      {        
        splay(cog, getMedian(cog));
        if(splay_count > 10 && interval>100)
          {
            interval = interval/2;
          }
        else if(splay_count <5)
        {
            interval = interval+100;
        }
        splay_count=0;

      }
  }

  return cog;
}