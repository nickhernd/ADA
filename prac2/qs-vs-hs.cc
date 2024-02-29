/*
JAIME HERNÁNDEZ DELGADO 48776654W
ADA. 2023-24
Práctica 2:"Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/

#include <unistd.h>
#include <iostream>
#include <math.h>

using namespace std;

double STEPS = 0;
const size_t MILIONS = pow(10, 6);

//--------------------------------------------------------------
// Middle Quick Sort
void 
middle_QuickSort(int * v, long left, long right){
    long i,j;
    int pivot;
	if (left<right){
        STEPS++;
		i=left; j=right;
		pivot=v[(i+j)/2];
		do{
			while (v[i]<pivot) {
                STEPS++;
                i++;
            }

			while (v[j]>pivot) {
                j--;
                STEPS++;
            }

			if (i<=j){ 
				swap(v[i], v[j]);
                STEPS++;
				i++; j--;
			}
		}while (i<=j);
		if (left < j){STEPS++;  middle_QuickSort(v,left,j);}
		if (i < right){STEPS++; middle_QuickSort(v,i,right);}
	}
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i)
{
    STEPS++;
    size_t largest;
    size_t l, r; //left and right child

    do{
        largest = i;  // Initialize largest as root
        l = 2*i + 1;  // left = 2*i + 1
        r = 2*i + 2;  // right = 2*i + 2

        STEPS++;
        // Is left child larger than root?
        if (l < n && v[l] > v[largest]) {
            STEPS++;
            largest = l;
        }
 
        // Is right child larger than largest so far
        if (r < n && v[r] > v[largest]) {
            STEPS++;
            largest = r;
        }
            
 
        // If largest is still root then the process is done
        if (largest == i) {
            STEPS++;
            break;
        }

	    // If not, swap new largest with current node i and repeat the process with childs. 
        swap(v[i], v[largest]);
        i=largest; 
        STEPS++; 
   } while (true);
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n)
{
    STEPS++;
    // Build a MAX-HEAP with the input array
    for (size_t i = n / 2 - 1; true; i--){
        sink(v, n, i);
        STEPS++;
        if (i==0) break; //as size_t is unsigned type
	}	

  	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--)
    {
		// Move current root to the end.
        swap(v[0], v[i]);
        STEPS++;
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0);
        STEPS++;
		// The procedure ends when HEAP has only one element, the lower of the input array

        
    }
}

int main() {
    srand(0);

    cout << "# QUICKSORT VERSUS HEAPSORT" << endl
         << "# Average processing Msteps (MILIONS of program steps)" << endl
         << "# Number of samples (arrays of integer): 30" << endl
         << "# \t\t RANDOM ARRAYS \t\t\t SORTED ARRAYS \t\t\t REVERSE SORTED ARRAYS" << endl
         << "# \t\t ------------------------ \t ------------------------ \t ------------------------" << endl
         << "# Size \t\t QuickSort \t HeapSort \t QuickSort \t HeapSort \t QuickSort \t HeapSort" << endl
         << "# =======================================================================================================" << endl;

    for (int exp = 15; exp <= 20; exp++) {
        size_t size = size_t(pow(2, exp));
        int *vec_random = new int[size];
        int *vec_sorted = new int[size];
        int *vec_invers = new int[size];

        cout << size << "\t\t";

        // Random Arrays (QuickSort)
        for (unsigned i = 0; i < 30; i++) {
            for (size_t j = 0; j < size; j++) {
                vec_random[j] = rand();
            }

            middle_QuickSort(vec_random, 0, size - 1);
        }

        cout << (STEPS / MILIONS) / 30 << "\t\t";
        STEPS = 0;

        // Random Arrays (HeapSort)
        for (unsigned i = 0; i < 30; i++) {
            for (size_t j = 0; j < size; j++)
                vec_random[j] = rand();

            heapSort(vec_random, size);
        }

        cout << (STEPS / MILIONS) / 30 << "\t\t";
        STEPS = 0;

        // Sorted Arrays (QuickSort)
        for (size_t j = 0; j < size; j++) {
            vec_sorted[j] = j;
        }

        middle_QuickSort(vec_sorted, 0, size - 1);
        cout << (STEPS / MILIONS) << "\t\t";
        STEPS = 0;

        // Sorted Arrays (HeapSort)
        heapSort(vec_sorted, size);
        cout << (STEPS / MILIONS) << "\t\t";
        STEPS = 0;

        // Reverse Sorted Arrays (QuickSort)
        for (size_t j = 0; j < size; j++) {
            vec_invers[j] = (size - 1) - j;
        }

        STEPS = 0;

        middle_QuickSort(vec_invers, 0, size - 1);
        cout << (STEPS / MILIONS) << "\t\t";
        STEPS = 0;

        // Reverse Sorted Arrays (HeapSort)
        for (size_t i = 0; i < size; i++) {
            vec_invers[i] = (size - 1) - i;
        }

        heapSort(vec_invers, size);
        cout << (STEPS / MILIONS) << endl;
        STEPS = 0;

        for (size_t i = 0; i < size; i++){
            if (vec_invers[i] < vec_invers[i-1]){ 
                cerr << "Panic, array not sorted! " << i << endl; 
                exit(EXIT_FAILURE);            
            }
        }

        delete[] vec_invers;

        for (size_t i = 0; i < size; i++){
            if (vec_random[i] < vec_random[i-1]){ 
                cerr << "Panic, array not sorted! " << i << endl; 
                exit(EXIT_FAILURE);            
            }
        }

        delete[] vec_random;

        for (size_t i = 0; i < size; i++){
            if (vec_sorted[i] < vec_sorted[i-1]){ 
                cerr << "Panic, array not sorted! " << i << endl; 
                exit(EXIT_FAILURE);            
            }
        }

        delete[] vec_sorted;
    }
    return 0;
}