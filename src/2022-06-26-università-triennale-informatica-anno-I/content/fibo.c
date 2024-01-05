#include <stdio.h>

int fibo(int n) {
  int a = 1, b = 1, t = -1;

  if (n <= 2) { return 1; }

  for (int i = 2; i <= n; i++) {
    t = b;
    b = a + b;
    a = t;
  }

  return a;
}

int main() {
  printf("%d\n", fibo(15));
  return 0;
}



