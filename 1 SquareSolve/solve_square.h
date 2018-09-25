#pragma once

#include <cassert>
#include <cmath>

#include "compare_double.h"
#include "solve_linear.h"

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Solves a square equation ax^2 + bx + c = 0
//!
//! @param [in] a a‐coefficient
//! @param [in] b b‐coefficient
//! @param [in] c c‐coefficient
//! @param [out] x1 Pointer to the major root
//! @param [out] x2 Pointer to the minor root
//!
//! @return Number of roots
//!
//! @note In case of infinite number of roots,
//! returns EQ_INF_ROOTS,
//! which is defined in "solve_linear.cpp"
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int solveSquare(const double a, const double b, const double c, const double* x1, const double* x2) {
  assert(std::isfinite(a));
  assert(std::isfinite(b));
  assert(std::isfinite(c));
  
  assert(x1 != NULL);
  assert(x2 != NULL);
  assert(x1 != x2);
  
  if (isDoubleZero(a)) {
    return solveLinear(b, c, x1);
  }
  
  if (isDoubleZero(c)) {
    // a is not zero (was checked before),
    // so there is always one root
    double x = 0;
    solveLinear(a, b, &x);
    
    *x1 = x < 0 ? 0 : x;
    *x2 = x < 0 ? x : 0;
    return 2;
  }
  
  const double d = b * b - 4 * a * c;
  if (isDoubleZero(d)) {
    *x1 = *x2 = -b / (2 * a);
    return 1;
  }
  
  if (d > 0) {
    const double sqrt_d = sqrt(d);
    const double sign = a > 0 ? 1 : -1;
    
    *x1 = (-b + sign * sqrt_d) / (2 * a);
    *x2 = (-b - sign * sqrt_d) / (2 * a);
    return 2;
  }
  
  return 0;
}
