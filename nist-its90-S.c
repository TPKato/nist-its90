/*
 * Convert between temperature and EMF (electromotive force) of
 * thermocouple (Type S) based on ITS-90 Thermocouple Database:
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

static FLOAT refcoeff_S1[] = {
  0.000000000000E+00,
  0.540313308631E-02,
  0.125934289740E-04,
  -0.232477968689E-07,
  0.322028823036E-10,
  -0.331465196389E-13,
  0.255744251786E-16,
  -0.125068871393E-19,
  0.271443176145E-23
};

static FLOAT refcoeff_S2[] = {
  0.132900444085E+01,
  0.334509311344E-02,
  0.654805192818E-05,
  -0.164856259209E-08,
  0.129989605174E-13
};

static FLOAT refcoeff_S3[] = {
  0.146628232636E+03,
  -0.258430516752E+00,
  0.163693574641E-03,
  -0.330439046987E-07,
  -0.943223690612E-14
};

static FLOAT invcoeff_S1[] = {
  0.00000000E+00,
  1.84949460E+02,
  -8.00504062E+01,
  1.02237430E+02,
  -1.52248592E+02,
  1.88821343E+02,
  -1.59085941E+02,
  8.23027880E+01,
  -2.34181944E+01,
  2.79786260E+00
};

static FLOAT invcoeff_S2[] = {
  1.291507177E+01,
  1.466298863E+02,
  -1.534713402E+01,
  3.145945973E+00,
  -4.163257839E-01,
  3.187963771E-02,
  -1.291637500E-03,
  2.183475087E-05,
  -1.447379511E-07,
  8.211272125E-09
};

static FLOAT invcoeff_S3[] = {
  -8.087801117E+01,
  1.621573104E+02,
  -8.536869453E+00,
  4.719686976E-01,
  -1.441693666E-02,
  2.081618890E-04,
  0.000000000E+00,
  0.000000000E+00,
  0.000000000E+00,
  0.000000000E+00
};

static FLOAT invcoeff_S4[] = {
  5.333875126E+04,
  -1.235892298E+04,
  1.092657613E+03,
  -4.265693686E+01,
  6.247205420E-01,
  0.000000000E+00,
  0.000000000E+00,
  0.000000000E+00,
  0.000000000E+00,
  0.000000000E+00
};

/*
 * calculates emf (thermocouple electromotive force) from temperature.
 * The argument is the temperature in °C.
 * This function returns emf in mV or NAN if the given argument is out of range.
 */
FLOAT
temp2emf_S(FLOAT T)
{
  FLOAT *coeff;
  FLOAT cor = 0;
  int size;

  if (T < -50.000) {
    return NAN;
  } else if (T <= 1064.180) {
    coeff = refcoeff_S1;
    size = sizeof(refcoeff_S1) / sizeof(refcoeff_S1[0]);
  } else if (T <= 1664.500) {
    coeff = refcoeff_S2;
    size = sizeof(refcoeff_S2) / sizeof(refcoeff_S2[0]);
  } else if (T <= 1768.100) {
    coeff = refcoeff_S3;
    size = sizeof(refcoeff_S3) / sizeof(refcoeff_S3[0]);
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
emf2temp_S(FLOAT U)
{
  FLOAT *coeff;
  int size;

  if (U < -0.235) {
    return NAN;
  } else if (U <= 1.874) {
    coeff = invcoeff_S1;
    size = sizeof(invcoeff_S1) / sizeof(invcoeff_S1[0]);
  } else if (U <= 11.950) {
    coeff = invcoeff_S2;
    size = sizeof(invcoeff_S2) / sizeof(invcoeff_S2[0]);
  } else if (U <= 17.536) {
    coeff = invcoeff_S3;
    size = sizeof(invcoeff_S3) / sizeof(invcoeff_S3[0]);
  } else if (U <= 18.693) {
    coeff = invcoeff_S4;
    size = sizeof(invcoeff_S4) / sizeof(invcoeff_S4[0]);
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
    for (T = -50; T <= 1768; T++) {
      printf("%d %f\n", T, temp2emf_S((FLOAT)T));
    }
  } else {
    /* check if emf2temp(temp2emf(T)) = T */
    FLOAT torelance = 0.1;
    for (T = -50; T < 1768; T++) {
      FLOAT t = emf2temp_S(temp2emf_S((FLOAT)T));
      if (fabs(t - T) > torelance) {
        error = 1;
	fprintf(stderr, "Error: T = %d != %f = emf2temp_S(temp2emf_S(T))\n", T, t);
      }
    }
  }

  return error;
}
#endif