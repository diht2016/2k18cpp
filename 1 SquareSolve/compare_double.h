#pragma once

const double EPSILON = 1e-12;

bool isDoubleZero(double a) {
  return (-EPSILON < a && a < EPSILON);
}
