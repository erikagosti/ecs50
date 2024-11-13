void find_div(unsigned int dividend, unsigned int divisor); // finds the quotient of the division problem

#include <stdio.h>
#include <stdlib.h>

int main(int argv, char* argc[]) {

  unsigned int dividend; // the dividend of the division
  unsigned int divisor; // the divisor of the division
  char *endptr;

  dividend = strtoul(argc[1], &endptr, 10);
  divisor = strtoul(argc[2], &endptr, 10);

  find_div(dividend, divisor);

  return 0;
}

void find_div(unsigned int dividend, unsigned int divisor) {

  unsigned int quotient = 0; // the quotient of the division
  unsigned int remainder = 0; // the remainder of the division

  for (int i = 31; i >= 0; i--) {
    remainder = (remainder << 1) | ((dividend >> i) & 1);
    if (remainder >= divisor) {
      quotient |= (1 << i);
      remainder -= divisor;
    }
  }

    printf("%u / %u = %u R %u\n", dividend, divisor, quotient, remainder);
}