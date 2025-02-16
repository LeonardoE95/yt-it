#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float TRAIN_DATA[][3] = {
  {0, 0, 0},
  {0, 1, 1},
  {1, 0, 1},
  {1, 1, 0}
};
#define TRAIN_COUNT sizeof(TRAIN_DATA) / sizeof(TRAIN_DATA[0])

float sigmoid(float x) {
  return 1.f / (1.f + expf(-x));
}

float rand_float(void) {
  return (float) rand() / (float) RAND_MAX;
}

float model_cost(float w1, float w2, float b) {
  float cost = 0.0f;

  for (size_t i = 0; i < TRAIN_COUNT; i++) {
    float x1 = TRAIN_DATA[i][0];
    float x2 = TRAIN_DATA[i][1];
    float y_expected = TRAIN_DATA[i][2];
    float y_obtained = sigmoid(x1 * w1 + x2 * w2 + b);

    float d = y_obtained - y_expected;
    // square to make sure that the cost function will have a
    // continuous partial derivative. This is needed to implement
    // gradient descent as a learning algorithm.
    cost += d*d;
  }

  // return the average cost
  cost /= TRAIN_COUNT;
  return cost;
}

int main(void) {
  printf("Hello nn!\n");

  srand(1337);

  float w1 = rand_float() * 5.f;
  float w2 = rand_float() * 10.f;  
  float b = rand_float() * 5.f;

  for (int i = 0; i < TRAIN_COUNT; i++) {
    float x1 = TRAIN_DATA[i][0];
    float x2 = TRAIN_DATA[i][1];
    float y_expected = TRAIN_DATA[i][2];
    float y_computed = sigmoid(w1 * x1 + w2 * x2 + b);
    printf("cost = %f | %f ^ %f = %f | expected = %f\n", model_cost(w1, w2, b), x1, x2, y_computed, y_expected);
  }

  printf("---------\n");

  float learning_rate = 1e-3;
  float eps = 1e-3;
  int epochs = 800*1000;  

  for (size_t i = 0; i < epochs; i++) {
    float c = model_cost(w1, w2, b);

    float dw1 = (model_cost(w1 + eps, w2, b) - c) / eps;
    float dw2 = (model_cost(w1, w2 + eps, b) - c) / eps;
    float db = (model_cost(w1, w2, b + eps) - c) / eps;
    
    w1 -= learning_rate * dw1;
    w2 -= learning_rate * dw2;
    b  -= learning_rate * db;

    // printf("cost = %f | w1 = %f, w2 = %f, b = %f\n", model_cost(w1, w2, b), w1, w2, b);
  }

  for (int i = 0; i < TRAIN_COUNT; i++) {
    float x1 = TRAIN_DATA[i][0];
    float x2 = TRAIN_DATA[i][1];
    float y_expected = TRAIN_DATA[i][2];
    float y_computed = sigmoid(w1 * x1 + w2 * x2 + b);
    printf("cost = %f | %f ^ %f = %f | expected = %f\n", model_cost(w1, w2, b), x1, x2, y_computed, y_expected);
  }  
  
  return 0;
}
