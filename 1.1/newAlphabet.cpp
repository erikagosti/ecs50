// This program creates an alternative representation for letters of the alphabet
// and replaces letters with its binary exponent component in numerical representation.

#include <iostream>
#include <cmath>
using namespace std;

int main(int argc, char* argv[]){

  /*
  1. make array for all input of n
  2. malloc another array of same size
  for each elemetn in array:
  2. if (n % 2**26) == 1:
    a. bool variable == true
    b. n %= 2**26
  3. n = log base 2 (n)
  4. if bool variable:
      char_array[index] = n + 65
     else:
      char_array[index] = n + 97
  char* word
  5. for each index in char_array 
    word strcat() char_array[index]
  return word
  */
  int size = argc - 1; // number of values entered in command line
  int *nums = (int *)malloc(sizeof(int) * size); // array of values to be replaced by its log2 value
  for (int i = 0; i < size; i++){
    sscanf(argv[i + 1], "%d", &(nums)[i]);
  }

  char *word_arr = (char *)malloc(sizeof(char) * size); // empty array to be filled with chars of the final word

  int capital_val = pow(2, 26); // numeral value of the capital binary bit
  for (int i = 0; i < size; i++)
  {
    bool capital = false; // whether or not the letter is capital
    if (nums[i] / capital_val == 1){
      capital = true;
      nums[i] = nums[i] % capital_val;
    }

    if (capital)
    {
      //word_arr = (char *)realloc(word_arr, sizeof(char) * (i + 1));
      word_arr[i] = log2(nums[i]) + 65;
      //strcat(word_arr, log2(nums[i]) + 65);
    }
    else{
      //word_arr = (char *)realloc(word_arr, sizeof(char) * (i + 1));
      word_arr[i] = log2(nums[i]) + 97;
      //strcat(word_arr, log2(nums[i]) + 97);
    }
  }

  cout << "You entered the word: " << word_arr;

  free(nums);
  free(word_arr);

  return 0;
}