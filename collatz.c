#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/ioctl.h>

#include "collatz.h"

#define Y_WIDTH 6

// calculate Collatz sequence length for given seed value
int collatz_iters(int seed) {
  int iters = 0;

  while(seed > 1) {
    // As defined by Collatz conjecture
    seed = (seed % 2) ? 3 * seed + 1 : seed / 2;
    iters++;
  }

  return iters;
}

// fills a c_pair heap from 1 to u_bound, inclusive
void fill_collatz(heap* hp) {
  int u_bound = hp->heap_size;

  for(int i = 0; i < u_bound; i++) {
    (hp->heap_arr + i)->seed = i + 1;
    (hp->heap_arr + i)->iters = collatz_iters(i + 1);
  }
}

// return size of heap
int len_heap(heap* hp) {
  return hp->heap_size;
}

// returns internal node with largest index
//
// avoids potential double in mathematical definition:
//       floor(h.heap_size / 2);
int max_intern(heap* hp) {
  return (hp->heap_size % 2 == 0) ? hp->heap_size / 2 : ((hp->heap_size) - 1) / 2;
}

// return iters at a specific index in heap
unsigned int acc_iters(heap* hp, int index) {
  return (hp->heap_arr + index)->iters;
}

// return seed at a specific index in heap
unsigned int acc_seed(heap* hp, int index) {
  return (hp->heap_arr + index)->seed;
}

// swap c_pairs at two indices in heap
void swap_heap(heap* hp, int i, int j) {
  c_pair temp = *(hp->heap_arr + i);
  *(hp->heap_arr + i) = *(hp->heap_arr + j);
  *(hp->heap_arr + j) = temp;
}

// standard min_heapify over a c_pair's iters member, but if equal,
// will sort by seed value in increasing order
void min_heapify(heap* hp, int i) {
  int root = i;
  int left_child = 2 * i + 1;
  int right_child = left_child + 1;

  // check if left_child should appear above root given
  // sorting method as stated at top of function
  if(left_child < len_heap(hp)) {
    if(acc_iters(hp, left_child) < acc_iters(hp, root)) {
      root = left_child;
    } else if(acc_iters(hp, left_child) == acc_iters(hp, root)
              && acc_seed(hp, left_child) > acc_seed(hp, root)) {
      root = left_child;
    }
  }

  // check if right_child should appear above root given
  // sorting method as stated at top of function
  if(right_child < len_heap(hp)) {
    if(acc_iters(hp, right_child) < acc_iters(hp, root)) {
      root = right_child;
    } else if(acc_iters(hp, right_child) == acc_iters(hp, root)
              && acc_seed(hp, right_child) > acc_seed(hp, root)) {
      root = right_child;
    }
  }

  // if either child should appear above, swap root with
  // greatest and continue
  if(root != i) {
    swap_heap(hp, root, i);
    min_heapify(hp, root);
  }
}

// construct valid min heap (with extra sorting
// by seed value of c_pairs -- see min_heapify)
void build_min_heap(heap* hp) {
  for(int i = max_intern(hp); i >= 0; i--) {
    min_heapify(hp, i);
  }
}

// extract sorted list of c_pairs from heap
c_pair* heapsort(heap* hp) {
  build_min_heap(hp);

  for(int i = hp->heap_size - 1; i > 0; i--) {
    swap_heap(hp, 0, i);
    hp->heap_size--;
    min_heapify(hp, 0);
  }

  return hp->heap_arr;
}

// print y-axis for specific iters value
void print_y(int label) {
  printf("\n %3d |", label);
}

// print x-axis for graph of given width
void print_x_axis(int width) {
  // spacing
  printf("\n");

  // align with y-axis
  for(int i = 0; i < Y_WIDTH - 1; i++) {
    printf(" ");
  }

  // print x-axis with interval markings
  for(int i = 0; i <= width; i++) {
    printf(i % 5 ? "-" : "'");
  }

  // new line to print interval labels
  printf("\n");

  // align with y-axis
  for(int i = 0; i < Y_WIDTH - 1; i++) {
    printf(" ");
  }

  // print intervals of 5 for x-axis
  for(int i = 0; i <= width; i++) {
   if(!(i % 10)) {
     printf("0");
   } else if(!(i % 5)) {
     printf("5");
   } else {
     printf(" ");
   }
  }

  // add space below graph
  printf("\n\n");
}

// print y-axis for section of iters values that
// do not have a seed
void print_gap(int u_bound, int l_bound) {
  for(int i = u_bound; i > l_bound; i--) {
    print_y(i);
  }
}

c_pair* collatz(heap* hp, int u_bound) {
  // allocate memory for heap_arr
  hp->heap_arr = (c_pair*)malloc(u_bound * sizeof(c_pair));

  // set heap_size
  hp->heap_size = u_bound;

  //fill heap_arr with collatz info
  fill_collatz(hp);

  // sort and extract c_pairs
  return heapsort(hp);
}

// calculate collatz for seeds from 1 to u_bound
// and print graph
void print_collatz(int u_bound) {
  // construct heap and fill with collatz info
  heap h;
  heap* hp = &h;
  c_pair* sorted = collatz(hp, u_bound);

  // initialize variables for loop
  int prev_iters = sorted->iters;
  int curr_x = 0, iters, seed;

  // print max y-axis interval
  print_y(prev_iters);

  // print each data point, accounting for seeds
  // that have the same iters value
  for(int i = 0; i < u_bound; i++) {
    //get next c_pair to plot
     seed = (sorted + i)->seed;
    iters = (sorted + i)->iters;

    // print y-axis intervals until at
    // correct y-value for c_pair to be printed
    // note: this will print nothing if already on
    //       correct y-value
    print_gap(prev_iters-1, iters);

    // if the same iters value as previous seed, append to
    // current line, otherwise print_gap will have printed all
    // y-axis intervals for which there are no seed values i.e. until
    // on correct y-axis interval for seed to be printed
    if(prev_iters == iters) {
      printf("%*c", seed - curr_x, 'O');
    } else {
      // because first (and therefore min) seed with
      // its iters value, also print y-axis label
      print_y(iters);
      printf("%*c", seed, 'O');

      // current iters is prev_iters for next seed
      prev_iters = iters;
    }

    // update curr_x for current seed
    curr_x = seed;
  }

  // add x-axis to bottom of graph
  print_x_axis(u_bound < 5 ? 5 : u_bound);
}

int main(int argc, char *argv[]) {
  int u_bound;

  // to hold window size info
  struct winsize w_info;

  // populate w_info with window size info
  ioctl(0, TIOCGWINSZ, &w_info);

  // calculate max u_bound given screen width
  int max_u_bound = w_info.ws_col - Y_WIDTH;

  if(argc < 2) {
    printf("usage: %s <int>\n", argv[0]);
  } else {
    u_bound = atoi(argv[1]);

    if(u_bound < 0 || u_bound > max_u_bound) {
      printf("0 <= <int> <= %d\n", max_u_bound);
    } else {
      print_collatz(u_bound);
    }
  }
}
