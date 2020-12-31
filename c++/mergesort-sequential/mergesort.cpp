#include <assert.h>
#include <cstring>
#include <iostream>
#include <limits>
#include <random>

typedef unsigned long long ull;

const ull MAX_ARRAY_SIZE_FOR_PRINTS = 21;

const int CODE_WRONG_NUM_ARGUMENTS_ERROR = 1;
const int CODE_UNABLE_TO_PARSE_ERROR = 2;
const int CODE_INPUT_EXCEEDS_MAX_ERROR = 3;

/* 
 * randomizes the array 'array'
 */
void randomize_array(float * array, ull n, unsigned seed) {
  std::default_random_engine rand_generator (seed);

  if(n < rand_generator.max()) {
    for (ull i = 0; i < n; i++) {
      array[i] = rand_generator() % n;
    }
  }
  else {
    for (ull i = 0; i < n; i++) {
      array[i] = rand_generator();
    }
  }
}

void print_array(float * array, ull n) {
  std::cout << "{";
  for(ull i = 0; i < n; i++){
    std::cout << array[i];

    if (i != n-1) {
      std::cout << ", ";
    }
  }
  std::cout << "}" << std::endl;
}

/*
 * for the purposes of this toy program, we only care about ascending order
 */
bool array_is_ordered(float * array, ull n) {
  for(ull i = 0; i < n-1; i++) {
    if(array[i] > array[i+1]) {
      return false;
    }
  }

  return true;
}

/*
 * assumes a and b are contiguous in memory, with 'a' first
 */
void merge(float * a, ull a_n, float * b, ull b_n) {
  ull a_i = 0; // index of array a
  ull b_i = 0; // index of array b

  // merging in place requires a lot of shifting, so is very memory-intensive in
  // worst case.

  // instead, let's just create another temporary array 'merged'

  float * merged = new float[a_n+b_n];
  ull merged_i = 0;

  while(a_i < a_n && b_i < b_n) {
    if(a[a_i] > b[b_i]) {
      // then b should come first
      merged[merged_i] = b[b_i];
      b_i++;
    }
    else {
      // else a is <= b so we put b next
      merged[merged_i] = a[a_i];
      a_i++;
    }

    merged_i++;
  }

  // if either 'a' or 'b' has items left, copy them to 'merged'
  if(a_i < a_n) {
    memcpy(merged + merged_i, a + a_i, sizeof(float) * (a_n - a_i));
  }
  if(b_i < b_n) {
    memcpy(merged + merged_i, b + b_i, sizeof(float) * (b_n - b_i));
  }

  // then move stuff from 'merged' back into original array
  memcpy(a, merged, sizeof(float) * (a_n+b_n));

  delete[] merged;
}

/*
 * recursively merge-sorts an array in-place.
 */
void mergesort(float * array, ull n) {
  if(n == 1)
    return;
  
  ull left_n = n/2;
  float * left_array = array;

  ull right_n = n - n/2;
  float * right_array = array + left_n;

  mergesort(left_array, left_n);
  mergesort(right_array, right_n);

  merge(left_array, left_n, right_array, right_n);
}

int main(int argc, char **argv) {
  if(argc < 2) {
    std::cerr << "ERROR: Must specify array length as first and only parameter"
      << std::endl
      << "Usage: " << argv[0] << " n" << std::endl;
    return CODE_WRONG_NUM_ARGUMENTS_ERROR;
  }


  ull array_size;

  try {
    array_size = std::stoull(argv[1]);
  } 
  
  catch (const std::invalid_argument& e) {
    std::cerr << "ERROR: could not parse argument to integer." << std::endl;
    return CODE_UNABLE_TO_PARSE_ERROR;
  } 
  
  catch (const std::out_of_range& e) {
    std::cerr << "ERROR: number specified is too large. Please specify a "
      << "number smaller than " 
      << std::numeric_limits<ull>::max() << std::endl;
    return CODE_INPUT_EXCEEDS_MAX_ERROR;
  }


  // TODO: make seed random once results are consistent
  unsigned seed = 1;
  float * array = new float[array_size];
  randomize_array(array, array_size, seed);

  while(array_is_ordered(array, array_size)) {
    std::cout << "Wow! You must be the luckiest person alive, because we just "
      << "generated an ordered array" << std::endl
      << " of length " << array_size << "." << std::endl;

    if(array_size < MAX_ARRAY_SIZE_FOR_PRINTS) {
      std::cout << "Array we generated: ";
      print_array(array, array_size);
    }

    std::cout << "Re-generating array..." << std::endl;
    
    std::cout << "old seed: " << seed << std::endl;
    seed += 11;
    std::cout << "new seed: " << seed << std::endl;

    randomize_array(array, array_size, seed);

    if(array_size < MAX_ARRAY_SIZE_FOR_PRINTS) {
      std::cout << "New array: ";
      print_array(array, array_size);
    }
  }

  if(array_size < MAX_ARRAY_SIZE_FOR_PRINTS) {
    std::cout << "Array before starting: ";
    print_array(array, array_size);
  }


  /* do mergesort */
  // float * sorted = new float[array_size];
  mergesort(array, array_size);


  /* check for successful sort! */
  if(array_is_ordered(array, array_size)) {
    std::cout << "SUCCESS: array is sorted!" << std::endl;
  }
  else {
    std::cerr << "FAILURE: array is not sorted!" << std::endl;
  }

  if(array_size < MAX_ARRAY_SIZE_FOR_PRINTS) {
    std::cout << "Array after sorting: ";
    print_array(array, array_size);
  }


  /* clean up */
  delete[] array;
  // delete[] sorted;

  return 0;
}