#include <stdio.h>
#include <stdlib.h>
#include "combs.h"

void print_mat(int** mat, int num_rows, int num_cols);
void free_mat(int** mat, int num_rows, int num_cols);

void generate_combinations(int* items, int len, int k, int start, int* temp, int temp_index, int** result, int* result_index) {
    // Base case: When a combination of size k is formed
    if (temp_index == k) {
        for (int i = 0; i < k; i++) {
            result[*result_index][i] = temp[i];
        }
        (*result_index)++;
        return;
    }

    // Iterate over the items array and recurse
    for (int i = start; i < len; i++) {
        temp[temp_index] = items[i];
        generate_combinations(items, len, k, i + 1, temp, temp_index + 1, result, result_index);
    }
}

int** get_combs(int* items, int k, int len) {
    if (k > len || k <= 0 || len <= 0) {
        return NULL;  // Invalid input
    }

    // Calculate the number of combinations
    int num_combs = 1;
    for (int i = 0; i < k; i++) {
        num_combs *= (len - i);
        num_combs /= (i + 1);
    }

    // Allocate memory for the 2-D result array
    int** result = (int**)malloc(num_combs * sizeof(int*));
    for (int i = 0; i < num_combs; i++) {
        result[i] = (int*)malloc(k * sizeof(int));
    }

    // Temporary array to hold a single combination
    int* temp = (int*)malloc(k * sizeof(int));
    int result_index = 0;

    // Generate all combinations
    generate_combinations(items, len, k, 0, temp, 0, result, &result_index);

    // Free the temporary array
    free(temp);

    return result;
}

int max(int a, int b){
  return a > b ? a : b;
}

int min(int a, int b){
  return a < b ? a : b;
}

int num_combs(int n, int k){
  int combs = 1;
  int i;
  
  for(i = n; i > max(k, n-k); i--){
    combs *= i;
  }

  for(i = 2; i <= min(k, n - k); i++){
    combs /= i;
  }
  
  return combs;

}

void print_mat(int** mat, int num_rows, int num_cols){
  int i,j;
  
  for(i = 0; i < num_rows; i++){
    for( j = 0; j < num_cols; j++){
      printf("%d ", mat[i][j]); 
    }
    printf("\n");
  }
}

void free_mat(int** mat, int num_rows, int num_cols){
  int i;
  
  for(i = 0; i < num_rows; i++){
    free(mat[i]);
  }
  free(mat);
}


int main(){
  int num_items;
  int* items; 
  int i,k;
  int** combs;
  printf("How many items do you have: ");
  scanf("%d", &num_items);
  
  items = (int*) malloc(num_items * sizeof(int));
  
  printf("Enter your items: ");
  for(i = 0; i < num_items; i++){
    scanf("%d", &items[i]);
  } 
  
  printf("Enter k: ");
  scanf("%d", &k);
  
  combs = get_combs(items, k, num_items);
  print_mat(combs,num_combs(num_items, k) ,k);
  free(items);
  free_mat(combs,num_combs(num_items, k), k);
  
  return 0;
}

