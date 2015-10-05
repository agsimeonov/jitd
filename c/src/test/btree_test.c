#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "cog.h"
#include "cracker.h"
#include "splay.h"
#include "util.h"
#include "adaptive_merge.h"

#define BUFFER_SIZE 10
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




/**
 * Acquires the count of BTree nodes in a tree.
 *
 * @param cog - the root BTree node in the tree
 * @return the count of BTree nodes in the tree
 */
long getBtreeNodeCount(struct cog *cog) {
  struct cog *left = cog->data.btree.lhs;
  struct cog *right = cog->data.btree.rhs;
  long count = 1;

  if (left != NULL && left->type == COG_BTREE) {
    count += getBtreeNodeCount(left);
  }

  if (right != NULL && right->type == COG_BTREE) {
    count += getBtreeNodeCount(right);
  }

  return count;
}

/**
 * Creates an in-order list of BTree Nodes for a given tree. (step called during recursion).
 *
 * @param cog - root BTree cog
 * @param list - the in-order list
 * @param index - index for next cog in the list
 * @return index for next cog in the list
 */
long inorderStep(struct cog *cog, struct cog **list, int index) {
  struct cog *left = cog->data.btree.lhs;
  struct cog *right = cog->data.btree.rhs;

  if (left != NULL && left->type == COG_BTREE)
    index = inorderStep(left, list, index);

  list[index] = cog;
  index += 1;

  if (right != NULL && right->type == COG_BTREE)
    index = inorderStep(right, list, index);

  return index;
}

/**
 * Creates an in-order list of BTree Nodes for a given tree.
 * NOTE: the in-order list is allocated with malloc, so deallocate with free when done!
 *
 * @param cog - root BTree cog
 * @return the in-order list
 */
struct cog **inorder(struct cog *cog) {
  struct cog *left = cog->data.btree.lhs;
  struct cog *right = cog->data.btree.rhs;
  struct cog **list = malloc(getBtreeNodeCount(cog) * sizeof(struct cog *));
  long index = 0;

  if (left != NULL && left->type == COG_BTREE)
    index = inorderStep(left, list, index);

  list[index] = cog;
  index += 1;

  if (right != NULL && right->type == COG_BTREE)
    index = inorderStep(right, list, index);

  return list;
}

struct cog* median(struct cog** array) {
 int size = sizeof(array)/sizeof(struct cog *);
 return array[size/2];


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
                    long b) {
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  struct cog *out = (*function)(cog, a, b);
  gettimeofday(&stop, NULL);
  long long startms = start.tv_sec * 1000LL + start.tv_usec / 1000;
  long long stopms = stop.tv_sec * 1000LL + stop.tv_usec / 1000;
  printf("Took %lld milliseconds\n", stopms - startms);
  return out;
}

/**
 * Do a given number of random reads on a cog.
 *
 * @param cog - the given cog
 * @param number - number of reads t do on a cog
 * @param range - the key range for reads
 * @return the resulting BTree
 */
struct cog *randomReads(struct cog *cog, long number, long range) {
  for (long i = 0; i < number; i++) {
    long a = rand() % range;
    long b = rand() % range;
    long low = a <= b ? a : b;
    long high = a > b ? a : b;
    cog = crack(cog, low, high);
  }
  return cog;
}

int main(int argc, char **argv) {
//  int rand_start = 42; //time(NULL)
//  srand(rand_start);
//  test1();
//  srand(rand_start);
//  test2();
//  srand(rand_start);
//  test3();
//  srand(rand_start);
//  test4();
//  srand(rand_start);
//  test5();
  splayTest();
  struct cog *cog;
  struct cog *m;
  struct cog **array;
  cog = mk_random_array(1000000);
  cog = timeRun(randomReads, cog, 1000, 1000000);
//  array = inorder(cog);
//  m = median(array);
//  free(array);
//  cog = splay(cog, m);
  cog = timeRun(randomReads, cog, 1000, 1000000);
//  array = inorder(cog);
//  m = median(array);
//  free(array);
//  cog = splay(cog, m);
  cog = timeRun(randomReads, cog, 1000, 1000000);
//  array = inorder(cog);
//  m = median(array);
//  free(array);
//  cog = splay(cog, m);
  cog = timeRun(randomReads, cog, 1000, 1000000);
//  array = inorder(cog);
//  m = median(array);
//  free(array);
//  cog = splay(cog, m);
  cog = timeRun(randomReads, cog, 1000, 1000000);
//  array = inorder(cog);
//  m = median(array);
//  free(array);
//  cog = splay(cog, m);
  cog = timeRun(randomReads, cog, 1000, 1000000);
//  array = inorder(cog);
//  m = median(array);
//  free(array);
//  cog = splay(cog, m);


}
