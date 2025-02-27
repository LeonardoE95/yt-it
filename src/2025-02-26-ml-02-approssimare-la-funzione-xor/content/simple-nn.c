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

struct Model {
  float w11;
  float w12;
  float w21;
  float w22;
  float w31;
  float w32;
  float b1;
  float b2;
  float b3;
};

float rand_float(void) {
  return (float) rand() / (float) RAND_MAX;
}

struct Model init_model() {
  struct Model m;
  
  m.w11 = rand_float();
  m.w12 = rand_float();
  m.w21 = rand_float();
  m.w22 = rand_float();
  m.w31 = rand_float();
  m.w32 = rand_float();
  m.b1 = rand_float();
  m.b2 = rand_float();
  m.b3 = rand_float();

  return m;
}

void print_model(struct Model m) {
  fprintf(stdout, "-----------------------\n");
  fprintf(stdout, "w11 =  %f, w21 = %f, b1 = %f\n", m.w11, m.w21, m.b1);
  fprintf(stdout, "w12 =  %f, w22 = %f, b2 = %f\n", m.w12, m.w22, m.b2);
  fprintf(stdout, "w31 =  %f, w32 = %f, b3 = %f\n", m.w31, m.w32, m.b3);  
  fprintf(stdout, "-----------------------\n");  
}

float sigmoid(float x) {
  return 1.f / (1.f + expf(-x));
}

float feed_forward(struct Model m, float x1, float x2) {
  float y1 = sigmoid(x1 * m.w11 + x2 * m.w21 + m.b1);
  float y2 = sigmoid(x1 * m.w12 + x2 * m.w22 + m.b2);
  float y3 = sigmoid(y1 * m.w31 + y2 * m.w32 + m.b3);
  return y3;  
}

float model_cost(struct Model m) {
  float cost = 0.0f;

  for (size_t i = 0; i < TRAIN_COUNT; i++) {
    float x1 = TRAIN_DATA[i][0];
    float x2 = TRAIN_DATA[i][1];
    float y_expected = TRAIN_DATA[i][2];
    float y_obtained = feed_forward(m, x1, x2);

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

struct Model model_learn(struct Model m, float learning_rate, float eps) {
  struct Model final_m;    
  float c = model_cost(m);
  float saved;

  saved = m.w11;
  m.w11 += eps;
  float dw11 = ((model_cost(m) - c) / eps);
  final_m.w11 = saved - learning_rate * dw11;    
  m.w11 = saved;

  saved = m.w12;
  m.w12 += eps;
  float dw12 = ((model_cost(m) - c) / eps);
  final_m.w12 = saved - learning_rate * dw12;
  m.w12 = saved;

  saved = m.w21;
  m.w21 += eps;
  float dw21 = ((model_cost(m) - c) / eps);
  final_m.w21 = saved - learning_rate * dw21;
  m.w21 = saved;

  saved = m.w22;
  m.w22 += eps;
  float dw22 = ((model_cost(m) - c) / eps);
  final_m.w22 = saved - learning_rate * dw22;
  m.w22 = saved;

  saved = m.w31;
  m.w31 += eps;
  float dw31 = ((model_cost(m) - c) / eps);
  final_m.w31 = saved - learning_rate * dw31;
  m.w31 = saved;  
  
  saved = m.w32;
  m.w32 += eps;
  float dw32 = ((model_cost(m) - c) / eps);
  final_m.w32 = saved - learning_rate * dw32;
  m.w32 = saved;

  saved = m.b1;
  m.b1 += eps;
  float b1 = ((model_cost(m) - c) / eps);
  final_m.b1 = saved - learning_rate * b1;
  m.b1 = saved;

  saved = m.b2;
  m.b2 += eps;
  float b2 = ((model_cost(m) - c) / eps);
  final_m.b2 = saved - learning_rate * b2;
  m.b2 = saved;

  saved = m.b3;
  m.b3 += eps;
  float b3 = ((model_cost(m) - c) / eps);
  final_m.b3 = saved - learning_rate * b3;
  m.b3 = saved;  

  return final_m;  
}

int main(void) {
  printf("Hello nn!\n");
  srand(1337);
  
  struct Model m = init_model();

  float learning_rate = 1e-2;
  float eps = 1e-3;
  int epochs = 500 * 1000;
  
  for (size_t i = 0; i < epochs; i++) {
    m = model_learn(m, learning_rate, eps);
  }
  
  printf("cost = %f\n", model_cost(m));      

  printf("--------------------------------------------\n");
  
  for (int i = 0; i < TRAIN_COUNT; i++) {
    float x1 = TRAIN_DATA[i][0];
    float x2 = TRAIN_DATA[i][1];
    printf("%f ⊕ %f = %f\n", x1, x2, feed_forward(m, x1, x2));
  }  
  
  return 0;
}
