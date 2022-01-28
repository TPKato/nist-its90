#include "nist-its90.h"

/* finds the polynomial using the Horner method.
   This function returns f(x) where
   f(x) = coeff[0] + coeff[1] * x + coeff[2] * x^2 + ... + coeff[size - 1] * x^(size - 1).
 */
FLOAT
polynomial(FLOAT x, FLOAT *coeff, int size)
{
  int i;
  FLOAT val = coeff[--size];
  for (i = --size; i >= 0; i--) {
    val = val * x + coeff[i];
  }
  return val;
}
