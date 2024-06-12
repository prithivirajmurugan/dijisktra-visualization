#include "Round.h"

int maps::math::RoundUp(int num, int multiple) {
  if (multiple == 0) {
    return num;
  }

  int remainder = num % multiple;
  if (remainder == 0) {
    return num;
  }
  return num + multiple - remainder;
}

int maps::math::RoundDown(int num, int multiple) {
  if (multiple == 0) {
    return num;
  }

  int remainder = num % multiple;

  if (remainder == 0) {
    return num;
  }
  return num - remainder;
}