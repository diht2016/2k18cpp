#include <cmath>
#include <stdio.h>

#include "solve_square.h"

#include "test_data.h"
#define print_fail_on_current printf("FAILED! a=%f, b=%f, c=%f)\n", a, b, c)

int main() {
  printf("# Square equation tester\n\n");
  
  int total = 0;
  int failed = 0;
  
  int pos = 0;
  while (std::isfinite(test_data[pos])) {
    total++;
    /* Okay, you got completely away with input processing, for the first time it is fine */
    double a = test_data[pos];
    double b = test_data[pos + 1];
    double c = test_data[pos + 2];
    
    double x1 = 0;
    double x2 = 0;
    int expected_root_count = (int) test_data[pos + 3];
    int root_count = solveSquare(a, b, c, &x1, &x2);
    
    pos += 4;
    
    if (expected_root_count != root_count) {
      failed++;
      print_fail_on_current;
      printf("Expected %d roots, got %d\n", expected_root_count, root_count);
    }
    
    if (expected_root_count <= 0) {
      continue;
    }
    
    double expected_x1 = test_data[pos++];
    if (!isDoubleZero(x1 - expected_x1)) {
      failed++;
      print_fail_on_current;
      printf("Expected major root to be %f, got %f\n", expected_x1, x1);
    }
    
    if (expected_root_count <= 1) {
      continue;
    }
    
    double expected_x2 = test_data[pos++];
    if (!isDoubleZero(x2 - expected_x2)) {
      failed++;
      print_fail_on_current;
      printf("Expected minor root to be %f, got %f\n", expected_x2, x2);
    }
  }
  
  printf("Total tests: %d, fails: %d.", total, failed);
  return 0;
}
