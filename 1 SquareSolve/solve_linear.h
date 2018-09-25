#pragma once

#include <cassert>
#include <cmath>

#include "compare_double.h"

const int EQ_INF_ROOTS = -1;

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Solves a linear equation kx + b = 0
//!
//! @param [in] k k‐coefficient
//! @param [in] b b‐coefficient
//! @param [out] x Pointer to the root
//!
//! @return Number of roots
//!
//! @note In case of infinite number of roots,
//! returns EQ_INF_ROOTS
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int solveLinear(double k, double b, double* x) {
  assert(std::isfinite(k));
  assert(std::isfinite(b));
  
  assert(x != NULL);
  
  if (isDoubleZero(k)) {
    return isDoubleZero(b) ? EQ_INF_ROOTS : 0;
  }
  
  *x = -b / k;
  return 1;
}
