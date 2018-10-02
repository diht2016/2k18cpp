#include <cassert>
#include <stdio.h>

#include "stack.h"

int main() {
  Stack<int> stack;
  
  stack.push(233);
  stack.dump();
  printf("%d", stack.pop());
  
  return 0;
}