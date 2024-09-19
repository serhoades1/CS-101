#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

void Merge(vector<int> &nums, int i, int j, int k);
void MergeSort(vector<int> &nums, int i, int k);
int Partition(vector<string> &strings, int i, int k);
void Quicksort(vector<string> &strings, int i, int k);

int main(int argc, char *argv[]) {
   
    if (argc != 4) {
        cout << "Invalid input: Try again" << endl;
        return 0;
    }

    char dataType = argv[1][0];

    ifstream file1(argv[2]);
    ifstream file2(argv[3]);
    unsigned int i;
    unsigned int j;
    unsigned int k;
    bool bContinue = true;

    if (!file1.is_open() || !file2.is_open()) {
        cout << "Cannot open file" << endl;
        return 0;
    }

    vector<int> intNumbers;
    vector<int> intNumbers2;
    vector<int> intNumbersFinal;
    vector<string> stringValues;
    vector<string> stringValues2;
    vector<string> stringValuesFinal;

    if (dataType == 'i') {
        int num;

        while (file1 >> num) {
            intNumbers.push_back(num);
        }
        while (file2 >> num) {
            intNumbers2.push_back(num);
        }

        for (i = 0; i < intNumbers.size(); i++) {
            if (i != intNumbers.size() - 1 && intNumbers.at(i) == intNumbers.at(i + 1)) {
                continue;
            }
            for (j = 0; j < intNumbers2.size(); j++) {
                if (intNumbers.at(i) == intNumbers2.at(j)) {
                    for (k = 0; k < intNumbersFinal.size(); k++) {
                        if (intNumbers.at(i) == intNumbersFinal.at(k)) {
                            bContinue = false;
                        }
                    }
                    if (!bContinue) {
                        bContinue = true;
                        break;
                    }
                    intNumbersFinal.push_back(intNumbers.at(i));
                    break;
                }
            }
        }

        MergeSort(intNumbersFinal, 0, intNumbersFinal.size() - 1);

        for (i = 0; i < intNumbersFinal.size(); i++) {
            if (binary_search(intNumbersFinal.begin(), intNumbersFinal.end(), intNumbersFinal[i])) {
                cout << intNumbersFinal[i] << endl;
            }
        }
    }
    
    else if (dataType == 's') {
        string value;
        // cout << "Test 8" << endl;
        while (file1 >> value) {
            stringValues.push_back(value);
        }
        // cout << "Test 9" << endl;
        while (file2 >> value) {
            stringValues2.push_back(value);
        }

        // for (i = 0; i < stringValues.size(); i++) {
        //     for (j = 0; j < stringValues2.size(); j++) {
        //         if (stringValues.at(i) == stringValues2.at(j)) {
        //             stringValuesFinal.push_back(stringValues.at(i));
        //             break;
        //         }
        //     }
        // }

        for (i = 0; i < stringValues.size(); i++) {
            for (j = 0; j < stringValues2.size(); j++) {
                if (stringValues.at(i) == stringValues2.at(j)) {
                    for (k = 0; k < stringValuesFinal.size(); k++) {
                        if (stringValues.at(i) == stringValuesFinal.at(k)) {
                            bContinue = false;
                        }
                    }
                    if (!bContinue) {
                        bContinue = true;
                        break;
                    }
                    stringValuesFinal.push_back(stringValues.at(i));
                    break;
                }
            }
        }

        // cout << stringValuesFinal.size() - 1 << endl;
        Quicksort(stringValuesFinal, 0, stringValuesFinal.size() - 1);

        // cout << "Test 10" << endl;
        for (i = 0; i < stringValuesFinal.size(); i++) {
            if (binary_search(stringValuesFinal.begin(), stringValuesFinal.end(), stringValuesFinal.at(i))) {
                cout << stringValuesFinal.at(i) << endl;
            }
        }
        // cout << "Test 11" << endl;
    }

    file1.close();
    file2.close();

    // cout << "Test 12" << endl;
    return 0;
   
//    int numbers[] = { 10, 2, 78, 4, 45, 32, 7, 11 };
//    const int NUMBERS_SIZE = 8;
//    int i;
   
//    cout << "UNSORTED: ";
//    for(i = 0; i < NUMBERS_SIZE; ++i) {
//       cout << numbers[i] << " ";
//    }
//    cout << endl;
   
//    MergeSort(numbers, 0, NUMBERS_SIZE - 1);
   
//    cout << "SORTED: ";
//    for(i = 0; i < NUMBERS_SIZE; ++i) {
//       cout << numbers[i] << " ";
//    }
//    cout << endl;
   
//    return 0;
}

void Merge(vector<int> &numbers, int i, int j, int k) {
   int mergedSize;                            // Size of merged partition
   int mergePos;                              // Position to insert merged number
   int leftPos;                               // Position of elements in left partition
   int rightPos;                              // Position of elements in right partition

   mergePos = 0;
   mergedSize = k - i + 1;
   leftPos = i;                               // Initialize left partition position
   rightPos = j + 1;                          // Initialize right partition position
   vector<int> mergedNumbers(mergedSize);       // Dynamically allocates temporary array
                                              // for merged numbers
   
   // Add smallest element from left or right partition to merged numbers
   while (leftPos <= j && rightPos <= k) {
      if (numbers[leftPos] < numbers[rightPos]) {
         mergedNumbers[mergePos] = numbers[leftPos];
         ++leftPos;
      }
      else {
         mergedNumbers[mergePos] = numbers[rightPos];
         ++rightPos;
         
      }
      ++mergePos;
   }
   
   // If left partition is not empty, add remaining elements to merged numbers
   while (leftPos <= j) {
      mergedNumbers[mergePos] = numbers[leftPos];
      ++leftPos;
      ++mergePos;
   }
   
   // If right partition is not empty, add remaining elements to merged numbers
   while (rightPos <= k) {
      mergedNumbers[mergePos] = numbers[rightPos];
      ++rightPos;
      ++mergePos;
   }
   
   // Copy merge number back to numbers
   for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
      numbers[i + mergePos] = mergedNumbers[mergePos];
   }
}

void MergeSort(vector<int> &numbers, int i, int k) {
   int j;
   
   if (i < k) {
      j = (i + k) / 2;  // Find the midpoint in the partition
      
      // Recursively sort left and right partitions
      MergeSort(numbers, i, j);
      MergeSort(numbers, j + 1, k);
      
      // Merge left and right partition in sorted order
      Merge(numbers, i, j, k);
   }
}

int Partition(vector<string> &strings, int i, int k) {
   int l;
   int h;
   int midpoint;
   string pivot;
   string temp;
   bool done;
   
   /* Pick middle element as pivot */
   midpoint = i + (k - i) / 2;
   pivot = strings.at(midpoint);
   
   done = false;
   l = i;
   h = k;
   
   while (!done) {
      
      /* Increment l while numbers[l] < pivot */
      while (strings.at(l) < pivot) {
         ++l;
      }
    //   cout << "Test 5" << endl;
      /* Decrement h while pivot < numbers[h] */
      while (pivot < strings.at(h)) {
         --h;
      }
      
      /* If there are zero or one elements remaining,
       all numbers are partitioned. Return h */
      if (l >= h) {
         done = true;
      }
      else {
         /* Swap numbers[l] and numbers[h],
          update l and h */
         temp = strings.at(l);
         strings.at(l) = strings.at(h);
         strings.at(h) = temp;
         
         ++l;
         --h;
      }
    //   cout << "Test 6" << endl;
   }
   
   return h;
}

void Quicksort(vector<string> &strings, int i, int k) {
    int j;
    // cout << "Test 13" << endl;
    /* Base case: If there are 1 or zero elements to sort,
        partition is already sorted */
    if (i >= k) {
        return;
    }
    // cout << "Test 1" << endl;
    /* Partition the data within the array. Value j returned
        from partitioning is location of last element in low partition. */
    j = Partition(strings, i, k);
    // cout << "Test 2" << endl;
    /* Recursively sort low partition (i to j) and
        high partition (j + 1 to k) */
    Quicksort(strings, i, j);
    // cout << "Test 3" << endl;
    Quicksort(strings, j + 1, k);
    // cout << "Test 4" << endl;
}
