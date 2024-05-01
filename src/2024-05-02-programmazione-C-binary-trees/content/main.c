#include <stdio.h>
#include <stdlib.h>

struct Node {
  int value;
  struct Node *left;
  struct Node *right;
};

typedef struct Node Node;

// --------------------------

Node *new_node(int value) {
  Node *n = malloc(sizeof(Node));

  // always initialize
  n->value = value;
  n->left = NULL;
  n->right = NULL;  
  
  return n;
}

void del_node(Node *n) {
  free(n);
}

// --------------------------

int compute_depth(Node *n) {
  if (!n) return 0;

  int right_depth = compute_depth(n->right);
  int left_depth = compute_depth(n->left);
  
  return 1 + (right_depth >= left_depth ? right_depth : left_depth);
}

int compute_sum(Node *n) {
  if (!n) return 0;
  
  int result = n->value;
  result += compute_sum(n->left);
  result += compute_sum(n->right);  

  return result;
}

void print_node_recursive(FILE *f, Node *n) {
  if (!n) { return; }

  fprintf(f, "A%p[label=%d]\n", (void*)n, n->value);

  if (n->left) {
    fprintf(f, "A%p -> A%p\n", (void*)n, (void*)n->left);
  }

  if (n->right) {
    fprintf(f, "A%p -> A%p\n", (void*)n, (void*)n->right);
  }
  
  print_node_recursive(f, n->left);
  print_node_recursive(f, n->right);  
}

void print_tree(Node *r) {
  FILE *f;

  // write file using graphviz syntax
  f = fopen("./tree.dot", "w");
  if (!f) {
    fprintf(stderr, "[ERROR] - Could not open 'tree.dot'\n");
    exit(1);
  }

  fprintf(f, "digraph G{\n");
  print_node_recursive(f, r);
  fprintf(f, "}\n");

  fclose(f);

  system("/usr/bin/dot -Tsvg tree.dot > out.svg");
}

// --------------------------

int main(void) {

  Node *n1 = new_node(1);
  Node *n2 = new_node(2);
  Node *n3 = new_node(3);
  Node *n4 = new_node(4);
  Node *n5 = new_node(5);  

  n1->left = n2;
  n1->right = n3;
  n3->left = n4;
  n3->right = n5;  

  printf("Depth = %d\n", compute_depth(n1));
  printf("Sum = %d\n", compute_sum(n1));
  print_tree(n1);
  
  del_node(n1);
  del_node(n2);
  del_node(n3);
  del_node(n4);
  del_node(n5);    

  return 0;
}
