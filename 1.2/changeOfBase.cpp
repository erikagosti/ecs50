// This program converts a number of a certain base to the same number in a different base.
#include <iostream>
#include <string>
#include <cctype>
#include <math.h>
#include <algorithm>
using namespace std;

int main() {

  int base; // the original base of the number
  string string_num; // the original number to be converted
  int new_base; // the base to convert the original number into

  cout << "Please enter the number's base: ";
  cin >> base;

  cout << "Please enter the number: ";
  cin >> string_num;

  cout << "Please enter the new base: ";
  cin >> new_base;

  /*
  1. change to base 10
    base a with bcdef digits: b * a^4 + c * a^3 + d * a^2 + e * a^1 + e * a^0 
  2. divide the base 10 value by the new base you want
  3. keep dividng the new quotient by the new base until it becomes 0
  4. write the remainders from bottom to top, left to right to get your answer
  */

  int n = string_num.length(); // the length of the original number
  long long int number10 = 0; // the number represented in base 10
  reverse(string_num.begin(), string_num.end());

  for (int i = 0; i < n; i++) {
    if (isalpha(string_num[i]))
    {
      string_num[i] = toupper(string_num[i]);
      number10 += (static_cast<int>(string_num[i]) - 55) * pow(base, i);
    }
    else{
      number10 += (static_cast<int>(string_num[i] - '0')) * pow(base, i);
    }
  }

  int* new_number = (int*)malloc(sizeof(int)); // the value of each digit in the new base in base 10
  int index = 0; // the index of each digit in new_number
  while (number10 / new_base != 0)
  {
    new_number = (int*)realloc(new_number, sizeof(int) * (index + 1));

    new_number[index] = number10 % new_base;
    number10 /= new_base;
    index++;
  }
  new_number = (int *)realloc(new_number, sizeof(int) * (index + 1));
  new_number[index] = number10 % new_base;

  string print_num = ""; // an array of all alphanumeric characters to print representing each digit in the number
  string c; // the character of a digit in the number
  for (int i = index, j = 0; i >= 0; i--, j++)
  {
    if (new_number[i] > 9)
    {
      c = new_number[i] + 55;
    }
    else {
      c = to_string(new_number[i]);
    }
    print_num += c;
  }
  reverse(string_num.begin(), string_num.end());
  cout << string_num << " base " << base << " is " << print_num << " base " << new_base << endl;

  free(new_number);

  return 0;
}