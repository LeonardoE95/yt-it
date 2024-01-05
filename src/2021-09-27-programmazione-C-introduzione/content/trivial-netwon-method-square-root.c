/*
  This file contains a trivial implementation of Netwon Method
  written in C for educational purposes.

  To compile:

     gcc trivial_netwon_method_square_root.c -lm -o square_root

  To execute:

     ./square_root
  
  Author: Leonardo Tamiano.
  Date: 2021-08-15.
*/

#include <stdio.h>
#include <math.h>

#define PRECISION 0.0001

// --------------------------------------

double square(double x) {
  return x * x;
}

double my_abs(double x) {
  if (x >= 0) {
    return x;
  } else {
    return -x;
  }
}

double average(double x, double y) {
  return (x + y) / 2;
}

// --------------------------------------

int good_enough(double approx, double x) {
  if (my_abs(square(approx) - x) < PRECISION) {
    return 1;
  } else {
    return 0;
  }
}

double improve_approximation(double approx, double x) {
  return average(approx, x/approx);
}

double square_root(double x) {
  double approx = 1;

  while (!good_enough(approx, x)) {
    approx = improve_approximation(approx, x);
  }

  return approx;
}

// --------------------------------------

void test() {
  int numbers_to_test[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int length = sizeof(numbers_to_test)/sizeof(int);

  for (int i = 0; i < length; i++) {
    double number = numbers_to_test[i];
    
    double our_square_root = square_root(number);
    double c_square_root = sqrt(number);

    double difference = my_abs(our_square_root - c_square_root);

    if (difference > PRECISION) {
      printf("%f\n", difference);  
    }
  }  
}

int main(int argc, char **argv) {
  // double output = square_root(5.0f);
  // printf("%0.10f\n", output);

  double x;
  printf("Calcola la radice di: ");

  if (scanf("%lf", &x) != 1) {
    printf("[ERROR] - l'input dato non è un float, riprovare!\n");
    return -1;
  }

  double root = square_root(x);
  printf("La radice di %0.3lf è %lf\n", x, root);
  
  return 0;
}
