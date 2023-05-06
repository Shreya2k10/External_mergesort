#include <bits/stdc++.h>
#include <fstream>
#include  <iostream>
using namespace std;

int numberOfMergePasses=0;
int totalNumberOfDiskSeeks=0;
int totalNumberOfDiskTransfers=0;
int totalCost=0;
struct MinHeapNode {
    
    // The element to be stored
    int element;
  
    // index for array of that element
    int i;
};
  
// utility function protype
void swap(MinHeapNode* x, MinHeapNode* y);
  
class MinHeap {
    
    
    MinHeapNode* harr;
  
    
    int heap_size;
  
public:
    
    // Constructor: creating heap for given size
    MinHeap(MinHeapNode a[], int size);
  
    // to heapify this subtree with root at given index
    void MinHeapify(int);
  
    // to get index of left child
    
    int left(int i) { return (2 * i + 1); }
  
    // to get index of right child
    int right(int i) { return (2 * i + 2); }
  
    // to get the root
    MinHeapNode getMin() { return harr[0]; }
  
    // to replace root with new node
    
    void replaceMin(MinHeapNode x)
    {
        harr[0] = x;
        MinHeapify(0);
    }
};
// building heap for given array  
MinHeap::MinHeap(MinHeapNode a[], int size)
{
    heap_size = size;
    harr = a; // store address of array
    int i = (heap_size - 1) / 2;
    while (i >= 0) {
        MinHeapify(i);
        i--;
    }
}
  
// A recursive method assuming that the
// subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    
    if (l < heap_size && harr[l].element < harr[i].element)
        smallest = l;
    
    if (r < heap_size
        && harr[r].element < harr[smallest].element)
        smallest = r;
    
    if (smallest != i) {
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}
  
// The swap function already defined in global
void swap(MinHeapNode* x, MinHeapNode* y)
{
    MinHeapNode temp = *x;
    *x = *y;
    *y = temp;
}
  
// Merging two subarrays of arr[]. Fisrt subarray is arr[l..m] and second is arr[n+1....r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    int L[n1], R[n2];

//data till i<n1 copied in L  
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
//data after j<n2 copied in R
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
  
    /* Merge 2 temp arrays in arr[l..r]*/
    // first subarray index
    i = 0;
  
    // second subarray index
    j = 0;
  
    // merged array index
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
  
    /* Copy elements L (if any) */
    while (i < n1)
        arr[k++] = L[i++];
  
    /* Copy rest elements (if any) in R */
    while (j < n2)
        arr[k++] = R[j++];
}
  
/* l-left index & r-right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        /* Same as (l+r)/2 just avoiding overflow for
        large l and h */
        int m = l + (r - l) / 2;
  
        // Sort first and second halves
        mergeSort(arr, l, m);
        numberOfMergePasses++;
        mergeSort(arr, m + 1, r);
        numberOfMergePasses++;
        merge(arr, l, m, r);
        numberOfMergePasses++;

        //I understood total number of merge passes as number of times
        //function mergeSort called+number of times function merge called

    }
}
  
FILE* openFile(char* fileName, char* mode)
{
    FILE* fp = fopen(fileName, mode);
    if (fp == NULL) {
        perror("File pointer missing\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}
  
void mergeFiles(char* output_file, int n, int k, char* fileContaingKey)
{
    string key_array;
    int i,in[i];
    i=0;
    while(getline(fileContaingKey,key_array))
    {
        getline(fileContaingKey, key_array[i], '\n');
        in[i] = openFile(key_array[i], "r");
    }

   // OUTPUT FILE
    FILE* out = openFile(output_file, "w");
  
    
    MinHeapNode* harr = new MinHeapNode[k];
    int i;
    for (i = 0; i < k; i++) {
        // break if no output file is empty and
        // index i will be no. of input files
        if (fscanf(in[i], "%d ", &harr[i].element) != 1)
            break;
  
        // Index of scratch output file
        harr[i].i = i;
    }
    // Create the heap
    MinHeap hp(harr, i);
  
    int count = 0;
  
    while (count != i) {
        // store min element in output file
        MinHeapNode root = hp.getMin();
        fprintf(out, "%d ", root.element);
  
        // now take the next element that will become the next root
        if (fscanf(in[root.i], "%d ", &root.element) != 1) {
            root.element = INT_MAX;
            count++;
        }
  
        // Replace root by next element of input file
        hp.replaceMin(root);
    }
  
    // closing all files
    for (int i = 0; i < k; i++)
        fclose(in[i]);
  
    fclose(out);
}
  

void createInitialRuns(char* input_file, int run_size,
                       int num_ways)
{
    // For big input file
    FILE* in = openFile(input_file, "r");
  
    // output scratch files
    FILE* out[num_ways];
    char fileName[2];
    for (int i = 0; i < num_ways; i++) {
        // convert i to string
        snprintf(fileName, sizeof(fileName), "%d", i);
  
        // Open output files in write mode.
        out[i] = openFile(fileName, "w");
    }
//dynamic array allocated
    int* arr = (int*)malloc(run_size * sizeof(int));
  
    bool more_input = true;
    int next_output_file = 0;
  
    int i;
    while (more_input) {
        for (i = 0; i < run_size; i++) {
            if (fscanf(in, "%d ", &arr[i]) != 1) {
                more_input = false;
                break;
            }
        }
  
        // sort array using merge sort
        mergeSort(arr, 0, i - 1);
  
        // write the records to the
        // appropriate scratch output file
       for (int j = 0; j < i; j++)
            fprintf(out[next_output_file], "%d ", arr[j]);
  
        next_output_file++;
    }
  
    // close input and output files
    for (int i = 0; i < num_ways; i++)
        fclose(out[i]);
  
    fclose(in);
}
  
// For sorting data stored on disk
void externalSort(char* input_file, char* output_file,
                  int num_ways, int run_size)
{
    createInitialRuns(input_file, run_size, num_ways);
  
    // Making runs for merging
    mergeFiles(output_file, run_size, num_ways);
}
  
// Driver code
int main()
{   

    char* fileContaingKey, *in;
    int sizeOfMemory,sizeOfKey,totalNumberOfKeys,numberOfKeys,diskBlockSize;
    
        
    cin << fileContaingKey << sizeOfKey << totalNumberOfKeys << numberOfKeys << diskBlockSize;
    if ( fileContaingKey.is_open() ) {
    char mychar;
    while ( fileContaingKey ) {
    key_array = fileContaingKey.get();
    std::cout << key_array;
    }
    }


    while(getline(fileContaingKey,sizeOfKey))
    {   
        int i=0;
        getline(cin, sizeOfKey[i], '\n');
        in[i] = openFile(sizeOfKey[i], "r");
        i++;
    }

  
    //FILE* in = openFile(input_file, "w");
  
    //srand(time(NULL));
  
    // generate input
    //for (int i = 0; i < num_ways * run_size; i++)
        //fprintf(in, "%d ", rand());
  
    //fclose(in);
  
    externalSort(input_file, output_file, num_ways,
                 run_size);
    cout<<totalNumberOfDiskSeeks<<totalNumberOfDiskTransfers<<numberOfMergePasses<<totalCost;
    
    return 0;
}

