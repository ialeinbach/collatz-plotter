#define Y_WIDTH 6

typedef struct heap heap;
typedef struct c_pair c_pair;

// holds initial value (seed) and length of
// its Collatz sequence (iters)
struct c_pair {
  int seed;
  int iters;
};

// heap of c_pairs that tracks the number
// of nodes it contains
struct heap {
  c_pair* heap_arr;
  int heap_size;
};

// plots ASCII-character graph of Collatz sequence
// length of integers [1, u_bound]
extern void print_collatz(int u_bound);
