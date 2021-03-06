/*
 * Convert between temperature and EMF (electromotive force) of
 * thermocouple (Type B) based on ITS-90 Thermocouple Database:
 * https://srdata.nist.gov/its90/main/
 *
 * This is an auto-generated file.
 * (created by: nistcoeff.pl all.tab)
 *
 * !!! DO NOT EDIT THIS FILE. !!!
 *
 */

#include <math.h>
#include "nist-its90.h"

static FLOAT refcoeff_B1[] = {
  0.000000000000E+00,
  -0.246508183460E-03,
  0.590404211710E-05,
  -0.132579316360E-08,
  0.156682919010E-11,
  -0.169445292400E-14,
  0.629903470940E-18
};

static FLOAT refcoeff_B2[] = {
  -0.389381686210E+01,
  0.285717474700E-01,
  -0.848851047850E-04,
  0.157852801640E-06,
  -0.168353448640E-09,
  0.111097940130E-12,
  -0.445154310330E-16,
  0.989756408210E-20,
  -0.937913302890E-24
};

static FLOAT invcoeff_B1[] = {
  9.8423321E+01,
  6.9971500E+02,
  -8.4765304E+02,
  1.0052644E+03,
  -8.3345952E+02,
  4.5508542E+02,
  -1.5523037E+02,
  2.9886750E+01,
  -2.4742860E+00
};

static FLOAT invcoeff_B2[] = {
  2.1315071E+02,
  2.8510504E+02,
  -5.2742887E+01,
  9.9160804E+00,
  -1.2965303E+00,
  1.1195870E-01,
  -6.0625199E-03,
  1.8661696E-04,
  -2.4878585E-06
};

/*
 * calculates emf (thermocouple electromotive force) from temperature.
 * The argument is the temperature in °C.
 * This function returns emf in mV or NAN if the given argument is out of range.
 */
FLOAT
temp2emf_B(FLOAT T)
{
  FLOAT *coeff;
  FLOAT cor = 0;
  int size;

  if (T < 0.000) {
    return NAN;
  } else if (T <= 630.615) {
    coeff = refcoeff_B1;
    size = sizeof(refcoeff_B1) / sizeof(refcoeff_B1[0]);
  } else if (T <= 1820.000) {
    coeff = refcoeff_B2;
    size = sizeof(refcoeff_B2) / sizeof(refcoeff_B2[0]);
  } else {
    return NAN;
  }

  cor += polynomial(T, coeff, size);
  return cor;
}

/*
 * calculates temperature from emf (thermocouple electromotive force).
 * The argument is the emf in mV.
 * This function returns a temperature in °C or NAN if the given argument is out of range.
 */
FLOAT
emf2temp_B(FLOAT U)
{
  FLOAT *coeff;
  int size;

  if (U < 0.291) {
    return NAN;
  } else if (U <= 2.431) {
    coeff = invcoeff_B1;
    size = sizeof(invcoeff_B1) / sizeof(invcoeff_B1[0]);
  } else if (U <= 13.820) {
    coeff = invcoeff_B2;
    size = sizeof(invcoeff_B2) / sizeof(invcoeff_B2[0]);
  } else {
    return NAN;
  }

  return polynomial(U, coeff, size);
}

#ifdef TEST
#include <stdio.h>

int
main(int argc, char *argv[])
{
  int T;
  int error = 0;

  if ((argc > 1) && (argv[1][0] == '-') && (argv[1][1] == 'c')) {
    /* output table of values */
    for (T = 0; T <= 1820; T++) {
      printf("%d %f\n", T, temp2emf_B((FLOAT)T));
    }
  } else {
    /* check if emf2temp(temp2emf(T)) = T */
    FLOAT torelance = 0.1;
    for (T = 250; T < 1820; T++) {
      FLOAT t = emf2temp_B(temp2emf_B((FLOAT)T));
      if (fabs(t - T) > torelance) {
        error = 1;
	fprintf(stderr, "Error: T = %d != %f = emf2temp_B(temp2emf_B(T))\n", T, t);
      }
    }
  }

  return error;
}
#endif
