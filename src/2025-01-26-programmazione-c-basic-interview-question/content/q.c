// Video for Esadecimale Project:
//    https://www.youtube.com/@esadecimale
//
// Compilation:
//    gcc q.c -o q

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ----------------------------------

#define QUEUE_SIZE 1024

typedef struct Node {
  struct Node *left;
  struct Node *right;
  int value;
} Node;

typedef struct Queue {
  void *data[QUEUE_SIZE];

  // index to the first element available in the queue
  int front;

  // index to the last element available in the queue
  int rear;
} Queue;

void init(Queue *q);
int not_empty(Queue *q);
void enqueue(Queue *q, void *value);
void *dequeue(Queue *q);

void solution(Node *n);

// ----------------------------------
// Basic queue implementation

void init(Queue *q) {
  // initialize memory for queue to byte 0x00.
  // We have QUEUE_SIZE slots each of size sizeof(void*).
  memset(q->data, 0, QUEUE_SIZE * sizeof(void*));

  q->front = -1;
  q->rear = -1;
}

// operation to insert a new value at the end of the queue
void enqueue(Queue *q, void *value) {
  if (q->rear == QUEUE_SIZE - 1) {
    printf("[INFO]: Trying to add value to a full queue.");
    return;
  }

  // before queue was empty, now it contains a single element
  if (q->front == -1) {
    q->front = 0;
  }

  // Since at this moment rear points to the last available elements
  // in the queue, before using it, we have to increment it by one.
  q->rear++;
  q->data[q->rear] = value;
}

// operation to extract the next value from the queue.
void *dequeue(Queue *q) {
  if (q->front == -1) {
    printf("[INFO]: Trying to extract value from an empty queue.");    
  }

  // extract value from front of queue
  void *value = q->data[q->front];

  // update front and rear values accordingly.
  // Specifically, we have two cases to consider:
  // 
  // 1. If front == rear, then this was the last element in the queue,
  // therefore both of index must be re-initialized to -1 to signal
  // that the queue is empty.
  // 
  // 2. Otherwise, there are still more elements within the queue,
  // therefore we only have to increment front to reach the next
  // available one.
  
  if (q->front == q->rear) {
    q->front = -1;
    q->rear = -1;
  } else {
    q->front++;
  }

  return value;
}

int not_empty(Queue *q) {
  // Queue is empty when there is nothing to extract, which is
  // signaled by the following condition on the front index
  return !(q->front == -1);
}

// ----------------------------------
//
// Problem: peform a BFS on a binary tree and print the value of each
// node in a separate line, starting from the root and arriving at the
// leaves.

typedef struct Data {
  Node *n;
  int depth;  
} Data;

Data *new_data(Node *n, int depth) {
  Data *d = malloc(sizeof(Data));
  d->n = n;
  d->depth = depth;
  return d;
}

void delete_data(Data *d) {
  free(d);
}

void solution(Node *n) {
  Queue queue;
  Queue *q = &queue;
  init(q);

  int last_depth = 0;
  enqueue(q, new_data(n, 0));

  while (not_empty(q)) {
    Data *d = (Data*) dequeue(q);

    // only print newlines when we reach the next level. Since we're
    // visiting the nodes linearly, we can keep track of the depth
    // reached so far with a single variable
    if (d->depth != last_depth) {
      printf("\n");
      last_depth = d->depth;
    }

    // always print the value of visited nodes
    printf("%d", d->n->value);

    // put into the queue the childs of visited nodes and update the
    // depth by one, since we're going one layer below
    if (d->n->left) {
      enqueue(q, new_data(d->n->left, d->depth + 1));
    }
    if (d->n->right) {
      enqueue(q, new_data(d->n->right, d->depth + 1));
    }

    // make sure to free up memory allocated by malloc
    delete_data(d);
  }

  printf("\n");
}

// ----------------------------------

int main(void) {
  // Create simple tree
  //
  //       1
  //    /     \
  //   2       3
  //  / \     / \
  // 4  6    5   6
  //
  
  Node n1 = {.left = NULL, .right = NULL, .value = 6};
  Node n2 = {.left = NULL, .right = NULL, .value = 5};
  Node n3 = {.left = NULL, .right = NULL, .value = 4};
  Node n4 = {.left = &n2, .right = &n1, .value = 3};
  Node n5 = {.left = &n3, .right = &n1, .value = 2};
  Node n6 = {.left = &n5, .right = &n4, .value = 1};

  solution(&n6);

  return 0;
}
