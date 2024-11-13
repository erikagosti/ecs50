// This program calculates the product of multiplying two upper triangular matrices together in compressed form.
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main(int argv, char* argc[]) {

  /*
  Converting to compressed matrix
  row major
  1. (i * N) + j
    a. have another counter starting from 0 that indicate how many indices per row to skip (skip parsing zeros)
    b. have an int for size
  2. make 2d array of size 1, only containing first input of matrix
    // [index, value]
  3. after each iteration, add the ((i * N) + j)'s number into the increase size by one and realloc array
  
  1. take in all input and turn into 2d array with indices
  
  Multiplying the matrices
  i. make new 2d array with index *empty*
  1. sum all row x col
  2. dot product between row and column
    a. each iteration, # of column to multiply with starts at 1 and increments by one until column = total columnB = total rowsA
    b. put it into 2d array at index (i * N) + j
  */


  ifstream f(argc[1]);

  string string_dim; // a string to hold the dimension value before casting
  getline(f, string_dim);
  int dim_1 = stoi(string_dim); // the dimension of mat_1

  int arr_size = (dim_1 * (dim_1 + 1)) / 2; // the size of the matrix in compressed array form

  int *mat_1 = (int *)calloc(arr_size, sizeof(int*)); // the first matrix to be multiplied
  // give indices

  string arg; // a string to read the values of the array into
  int i = 0; // a counter for reading values into the indices of mat_1
  while (getline(f, arg))
  {
    mat_1[i] = stoi(arg);

    i++;
  }

  ifstream f2(argc[2]);

  string string_dim2; // a string to hold the dimension value before casting
  getline(f2, string_dim2);
  int dim_2 = stoi(string_dim2); // the dimension of mat_2

  arr_size = (dim_2 * (dim_2 + 1)) / 2;

  int *mat_2 = (int *)calloc(arr_size, sizeof(int*)); // the second matrix to be multiplied
  // give indices

  string arg2; // a string to read the values of the array into
  i = 0;
  while (getline(f2, arg2))
  {
    mat_2[i] = stoi(arg2);

    i++;
  }

  int *new_mat = (int *)calloc(arr_size, sizeof(int)); // an empty new array to store the resulting matrix values into

  for (int row = 0; row < dim_1; row++) { // row == 2

    for (int i = row; i < dim_1; i++) { // i == 3
      int sum = 0; // the result of each matrix multiplication iteration

      for (int j = row; j <= i; j++) { // j == 2
      int index_1 = row * dim_1 - row * (row - 1) / 2 + j - row; // the index of which to multiply from mat_1
      int index_2 = j * dim_1 - j * (j - 1) / 2 + i - j; // the index of which to multiply from mat_2
        sum += mat_1[index_1] * mat_2[index_2];
      }
      int index_3 = row * dim_1 - row * (row - 1) / 2 + i - row; // the index of new_mat to store the value into
      new_mat[index_3] = sum;
    }
  }

  for (int i = 0; i < arr_size; i++) {
    cout << new_mat[i] << " "; // delete
  }
  cout << endl;

  free(mat_1);
  free(mat_2);
  free(new_mat);

  /*
  ATTEMPTS
  // only for square matrices

  for (int row = 0; row < dimensions; row++) { // row == 2

    for (int i = row; i < dimensions; i++) { // i == 3
      int sum = 0;

      for (int j = row; j <= i; j++) { // j == 2
      int index_1 = row * dimensions - row * (row - 1) / 2 + j - row;
      int index_2 = j * dimensions - j * (j - 1) / 2 + i - j;
        sum += mat_1[index_1] * mat_2[index_2];
      }
      int index_3 = row * dimensions - row * (row - 1) / 2 + i - row;
      new_mat[row * dimensions + i] = sum;
    }
  }
  */

  return 0;
}