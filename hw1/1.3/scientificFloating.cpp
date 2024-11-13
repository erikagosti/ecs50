// This program calculates the scientific base 2 format of an inputted floating point number.

#include <iostream>
#include <string.h>
using namespace std;

int main() {
  /*
  1. make note of if num is pos or neg
  2. convert num to binary representation
  3. put it in 1.m * 2^e form and save e
  4. add 127 to e and find the binary representation of that
  5. pos/neg bit, exp field, mantissa field (without leading 1)
  */

  float num; // flaoting point nubmer the user inputs

  cout << "Please enter a float: ";
  cin >> num;

  bool negative = false; // if the number is negative, negative is true
  if (num < 0) {
    negative = true;
    num *= -1;
  }

  int left = static_cast<int>(num); // the digits to the left of the decimal point in num
  float right = num - left; // the digits to the right of the decimal point in num

  string mantissa_field = ""; // the mantissa field of num in base 2
  int exp = -1; // the exponential value of num in base 2
  while (left > 0)
  {
    mantissa_field = to_string(left % 2) + mantissa_field;
    exp++;
    left /= 2;
  }

  // mantissa_field.erase(0, 1);
  string mantissa_field_right = ""; // the mantissa field of num in base 2 to the right of the decimal point

  while ((right != 0) && (mantissa_field_right.length() <= 22)) {
    mantissa_field_right += to_string(static_cast<int>(right * 2));
    right *= 2;
    if (right >= 1)
    {
      right -= 1;
    }
  }

  mantissa_field += mantissa_field_right;

  mantissa_field.insert(1, ".");
  string binary_rep = mantissa_field + "E" + to_string(exp); // the final binary representation of num in base 2

  if (negative) {
    binary_rep = "-" + binary_rep;
  }

  cout << binary_rep << endl;

  // 10001100.000110011... cut off at 22 bits for mantissa


  return 0;
}