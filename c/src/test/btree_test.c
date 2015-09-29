#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cog.h"
#include "cracker.h"
#include "splay.h"
#include "util.h"
#include "adaptive_merge.h"


#define BUFFER_SIZE 500000
#define KEY_RANGE   1000000

buffer mk_random_buffer(int size)
{
  buffer b = buffer_alloc(size);
  int i;
  for(i = 0; i < size; i++){
    b->data[i].key = rand() % KEY_RANGE;
    b->data[i].value = rand();
  }
//  record_dump(b->data, 0, size);
  return b;
}

buffer mk_sorted_buffer(int size)
{
  buffer b = mk_random_buffer(size);
  record_sort(b->data, 0, size);
  record_dump(b->data, 0, size);
  return b;
}


cog *mk_random_array(int size)
{
  return make_array(0, size, mk_random_buffer(size));
}

cog *mk_sorted_array(int size)
{
  return make_sortedarray(0, size, mk_sorted_buffer(size));
}

void test_scan(cog *c, long low, long high)
{
  iterator iter = scan(c, low, high);
  iter_dump(iter);
  iter_cleanup(iter);
}

void test1()
{
  printf("test 1\n");
  cog *c = mk_random_array(BUFFER_SIZE);
  test_scan(c, 200, 700);
  cleanup(c);
}

void test2()
{
  printf("test 2\n");
  cog *c = mk_sorted_array(BUFFER_SIZE);
  test_scan(c, 200, 700);
  cleanup(c);
}
void test3()
{
  printf("test 3\n");
  cog *c = 
    make_concat(
      mk_random_array(BUFFER_SIZE),
      mk_random_array(BUFFER_SIZE)
    );
  test_scan(c, 100, 200);
  cleanup(c);
}

void test4() {
  printf("test 4\n");
  cog *c = make_concat(
            mk_random_array(BUFFER_SIZE),
            mk_random_array(BUFFER_SIZE)
          );
  c = crack(c, 100, 200);
  c = crack(c, 400, 700);
  c = crack(c, 800, 900);
  test_scan(c, 1, 1000);
  cleanup(c);
}

void test5() {
  printf("test 5\n");
  cog *c = make_concat(
            mk_random_array(BUFFER_SIZE),
            mk_random_array(BUFFER_SIZE)
          );
  double_struct *ret;
  printf("Scan 1\n");
  ret = amerge(c, 100, 200);
  iter_dump(ret->iter);
  iter_cleanup(ret->iter);
  c = ret->cog;
  free(ret);
  printf("Scan 2\n");
  ret = amerge(c, 300, 700);
  iter_dump(ret->iter);
  cleanup(ret->cog);
  iter_cleanup(ret->iter);
  free(ret);
}

void splayTest() {
  printf("Splaying Test:\n");
  cog *six = make_btree(NULL, NULL, 6);
  cog *eight = make_btree(NULL, NULL, 8);
  cog *seven = make_btree(six, eight, 7);
  cog *four = make_btree(NULL, NULL, 4);
  cog *five = make_btree(four, seven, 5);
  cog *two = make_btree(NULL, NULL, 2);
  cog *three = make_btree(two, five, 3);
  cog *ten = make_btree(NULL, NULL, 10);
  cog *nine = make_btree(three, ten, 9);
  cog *zero = make_btree(NULL, NULL, 0);
  cog *one = make_btree(zero, nine, 1);
  cog *twelve = make_btree(NULL, NULL, 12);
  cog *eleven = make_btree(one, twelve, 11);
  printf("Before splay:\n");
  printJITD(eleven,0);
  printf("After splay:\n");
  splay(eleven, seven);
  printJITD(seven, 0);
}

void perfTest() {
  // get current nanotime
  struct cog *c = make_array(0, 1000, mk_random_buffer(1000));
  // get current nantotime
  // subtract to get difference this is how long it took to run
  // print result
}

int main(int argc, char **argv)
{
  clock_t start;
  cog *c;
  clock_t stop;
  double elapsed;

  c = mk_random_array(10000000);
  start = clock();
  crack_one(c, 500000);
  stop = clock();
  elapsed = (double) (stop - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("crack-1 in ms: %f\n", elapsed);

  c = mk_random_array(10000000);
  start = clock();
  crack_scan(c, 333333, 666666);
  stop = clock();
  elapsed = (double) (stop - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("crack in ms: %f\n", elapsed);

  c = mk_random_array(100000);
  start = clock();
  amerge(c, 333333, 666666);
  stop = clock();
  elapsed = (double) (stop - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("amerge in ms: %f\n", elapsed);
}
