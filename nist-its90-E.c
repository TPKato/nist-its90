/*
 * Convert between temperature and EMF (electromotive force) of
 * thermocouple (Type E) based on ITS-90 Thermocouple Database:
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

static FLOAT refcoeff_E1[] = {
  0.000000000000E+00,
  0.586655087080E-01,
  0.454109771240E-04,
  -0.779980486860E-06,
  -0.258001608430E-07,
  -0.594525830570E-09,
  -0.932140586670E-11,
  -0.102876055340E-12,
  -0.803701236210E-15,
  -0.439794973910E-17,
  -0.164147763550E-19,
  -0.396736195160E-22,
  -0.558273287210E-25,
  -0.346578420130E-28
};

static FLOAT refcoeff_E2[] = {
  0.000000000000E+00,
  0.586655087100E-01,
  0.450322755820E-04,
  0.289084072120E-07,
  -0.330568966520E-09,
  0.650244032700E-12,
  -0.191974955040E-15,
  -0.125366004970E-17,
  0.214892175690E-20,
  -0.143880417820E-23,
  0.359608994810E-27
};

static FLOAT invcoeff_E1[] = {
  0.0000000E+00,
  1.6977288E+01,
  -4.3514970E-01,
  -1.5859697E-01,
  -9.2502871E-02,
  -2.6084314E-02,
  -4.1360199E-03,
  -3.4034030E-04,
  -1.1564890E-05,
  0.0000000E+00
};

static FLOAT invcoeff_E2[] = {
  0.0000000E+00,
  1.7057035E+01,
  -2.3301759E-01,
  6.5435585E-03,
  -7.3562749E-05,
  -1.7896001E-06,
  8.4036165E-08,
  -1.3735879E-09,
  1.0629823E-11,
  -3.2447087E-14
};

/*
 * calculates emf (thermocouple electromotive force) from temperature.
 * The argument is the temperature in °C.
 * This function returns emf in mV or NAN if the given argument is out of range.
 */
FLOAT
temp2emf_E(FLOAT T)
{
  FLOAT *coeff;
  FLOAT cor = 0;
  int size;

  if (T < -270.000) {
    return NAN;
  } else if (T <= 0.000) {
    coeff = refcoeff_E1;
    size = sizeof(refcoeff_E1) / sizeof(refcoeff_E1[0]);
  } else if (T <= 1000.000) {
    coeff = refcoeff_E2;
    size = sizeof(refcoeff_E2) / sizeof(refcoeff_E2[0]);
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
emf2temp_E(FLOAT U)
{
  FLOAT *coeff;
  int size;

  if (U < -8.825) {
    return NAN;
  } else if (U <= 0.000) {
    coeff = invcoeff_E1;
    size = sizeof(invcoeff_E1) / sizeof(invcoeff_E1[0]);
  } else if (U <= 76.373) {
    coeff = invcoeff_E2;
    size = sizeof(invcoeff_E2) / sizeof(invcoeff_E2[0]);
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
    for (T = -270; T <= 1000; T++) {
      printf("%d %f\n", T, temp2emf_E((FLOAT)T));
    }
  } else {
    /* check if emf2temp(temp2emf(T)) = T */
    FLOAT torelance = 0.1;
    for (T = -200; T < 1000; T++) {
      FLOAT t = emf2temp_E(temp2emf_E((FLOAT)T));
      if (fabs(t - T) > torelance) {
        error = 1;
	fprintf(stderr, "Error: T = %d != %f = emf2temp_E(temp2emf_E(T))\n", T, t);
      }
    }
  }

  return error;
}
#endif
