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

// calculate Collatz sequence length for given seed value
int collatz_iters(int seed);

// return size of heap
int len_heap(heap* hp);

// returns internal node with largest index
//
// avoids potential double in mathematical definition:
//       floor(h.heap_size / 2);
int max_intern(heap* hp);

// return iters at a specific index in heap
unsigned int acc_iters(heap* hp, int index);

// return seed at a specific index in heap
unsigned int acc_seed(heap* hp, int index);

// swap c_pairs at two indices in heap
void swap_heap(heap* hp, int i, int j);

// standard min_heapify over a c_pair's iters member, but if equal,
// will sort by seed value in increasing order
void min_heapify(heap* hp, int i);

// construct valid min heap (with extra sorting
// by seed value of c_pairs -- see min_heapify)
void build_min_heap(heap* hp);

// extract sorted list of c_pairs from heap
c_pair* heapsort(heap* hp);
