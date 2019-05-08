#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

// number of elements
#define LIMIT 1000000
#define SWAP(A, B) {int T=(A);A=(B);B=T;}

typedef struct int_array
{
    size_t size;
    int *array;
} _int_array;

void init(_int_array *a, size_t array_size )
{
    int i;
    
    a->size = array_size;
    a->array = malloc(array_size*sizeof(int));
    for (i=0; i<a->size; i++)
	{
	    a->array[i] = i;
	    //printf("%4d ", i);
	}
}	

void shuffle(_int_array *a)
{
    int i, j;
    
	for (i=0; i<a->size; i++)
	{
	    j = rand() % a->size;
	    SWAP(a->array[i], a->array[j]);
	}
}

void display(_int_array *a)
{
    int i;
	for (i=0; i<a->size; i++)
	{
        printf("%5d", a->array[i]);
	}
    printf("\n----------------------------------------------------------------------------------------------------\n");
}

bool is_sorted(_int_array *a)
{
    int i;

    for (i=0; i<a->size-1; i++)
    {
        if (a->array[i] > a->array[i+1])
        {
            return false;
        }
    }
    return true;
}


//-------------------------------------------------//-------------------------------------------------
void bubble_sort(_int_array *a)
{
    int i, j;
    
	for (i=a->size; --i>=0;)
	{
	    //printf("%4.1f%%\r", (1.0-(float)i/a->size )*100.0);
        int flipped = 0;
        
	    for (j=0; j<i; j++)
	    {
    		if (a->array[j] > a->array[j+1])
    		{
    		    SWAP(a->array[j], a->array[j+1]);
    		    flipped = 1;
    		}
        }
	    if (!flipped)
	    {
	        //printf("\n");
	        return;
	    }
    }
}

//-------------------------------------------------//-------------------------------------------------
void bidirectional_bubble_sort(_int_array *a)
{
	int j;
	int limit = a->size;
	int st = -1;

	while (st < limit) {
	    int flipped = 0;
	    st++;
	    limit--;

	    //printf("%4.1f%%\r", (float)st/a->size*100.0);

	    for (j=st; j<limit; j++) 
	    {
			if (a->array[j] > a->array[j+1]) 
			{
				SWAP(a->array[j], a->array[j+1]);
				flipped = 1;
			}
	    }
	    if (!flipped) {
	        //printf("\n");
			return;
	    }
	    for (j=limit; --j >= st;)
	    {
			if (a->array[j] > a->array[j+1])
			{
				SWAP(a->array[j], a->array[j+1]);
				flipped = 1;
			}
	    }
	    if (!flipped) {
	        //printf("\n");
	    	return;
	    }
	}   // while
}

//-------------------------------------------------//-------------------------------------------------
typedef struct partition_params {
	int *a;
	int lo;
	int hi;
} _partition_params;

void partition(int a[], int lo0, int hi0) 
{
	int lo = lo0;
	int hi = hi0;

	if (lo >= hi)
	{
	    return;
	}
    //printf("[%d, %d]\r", lo0, hi0);

	int mid = a[(lo + hi) / 2];
	while (lo < hi) {
	    while (lo<hi && a[lo] < mid) {
		    lo++;
	    }
	    while (lo<hi && a[hi] > mid) {
    		hi--;
	    }
	    if (lo < hi) {
    		SWAP(a[lo], a[hi]);
	    }
	}
	if (hi < lo) {
	    SWAP(lo, hi);
	}
	partition(a, lo0, lo);
	//printf("\n");
	partition(a, lo == lo0 ? lo+1 : lo, hi0);
}

void *partition_thread(void *params)
{
	_partition_params *part_data = (_partition_params *)params;
	partition(part_data->a, part_data->lo, part_data->hi);
}

/* create temp arrays on the HEAP - not the stack */
int *L, *R;

void merge(int arr[], int l, int m, int r);

void quick_sort(_int_array *a)
{
	// double-threaded!
	pthread_t th1, th2;
	_partition_params part_A, part_B;
	
	part_A.a = a->array;
	part_A.lo = 0;
	part_A.hi = a->size/2-1;
	
	part_B.a = a->array;
	part_B.lo = part_A.hi+1;
	part_B.hi = a->size-1;
	
	pthread_create(&th1, NULL, partition_thread, &part_A);
	pthread_create(&th2, NULL, partition_thread, &part_B);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	
    // Don't forget to allocate memory for the backing arrays!
    L = malloc(a->size/2*sizeof(int));
    R = malloc(a->size/2*sizeof(int));

	// now merge the 2 sorted partitions part_A and part_B
	merge(a->array, 0, part_A.hi, a->size-1);
	//partition(a, 0, a->size-1);

    free(L);
    free(R);
}

//-------------------------------------------------//-------------------------------------------------
void downheap(int a[], int k, int N)
{
	int T = a[k - 1];
	while (k <= N/2)
	{
        int j = k + k;
        if ( (j < N) && (a[j - 1] < a[j]) )
            {
            j++;
		}
		if (T >= a[j - 1])
		{
    		break;
    	}
		else {
			a[k - 1] = a[j - 1];
            k = j;
        }
	}
    a[k - 1] = T;
}

void heap_sort(_int_array *a) 
{
	int N = a->size, k;

	for (k = N/2; k > 0; k--)
	{
	    downheap(a->array, k, N);
    }
	
	do
	{
        SWAP(a->array[0], a->array[N-1]);
	    N -= 1;
        downheap(a->array, 1, N);
	} while (N > 1);
}

//-------------------------------------------------//-------------------------------------------------
void bsort(int a[], int lo, int hi) 
{
    int i, j;
	for (j=hi; j > lo; j--) 
	{
	    for (i=lo; i < j; i++) 
	    {
            if (a[i] > a[i+1]) 
            {
        	    SWAP(a[i], a[i+1]);
            }
        }
    }
} 

void qb_sort(int a[], int lo0, int hi0) 
{
	int lo = lo0;
	int hi = hi0;

	if ((hi-lo) <= 6) {
	    bsort(a, lo, hi);
	    return;
	}
	int mid = a[(lo + hi) / 2];
	while (lo < hi) {
	    while (lo<hi && a[lo] < mid) {
	    	lo++;
	    }
	    while (lo<hi && a[hi] > mid) {
    		hi--;
	    }
	    if (lo < hi) {
			SWAP(a[lo], a[hi]);
	    }
	}
	if (hi < lo) {
		SWAP(a[lo], a[hi]);
	}
	qb_sort(a, lo0, lo);
	qb_sort(a, lo == lo0 ? lo+1 : lo, hi0);
}

void quick_bubble_sort(_int_array *a) 
{
	qb_sort(a->array, 0, a->size-1);
}

//-------------------------------------------------//-------------------------------------------------
void brute(int a[], int lo, int hi)
{
    if ((hi-lo) == 1) 			// compare 2 elements
    {
	    if (a[hi] < a[lo])
	    {
			SWAP(a[lo], a[hi]);
	    }
	}
	if ((hi-lo) == 2)			// compare 3 elements
    {
	    int pmin = a[lo] < a[lo+1] ? lo : lo+1;
	    	pmin = a[pmin] < a[lo+2] ? pmin : lo+2;
	    if (pmin != lo)
        {
	        SWAP(a[lo], a[pmin]);
	    }
	    brute(a, lo+1, hi);
	}
	if ((hi-lo) == 3)			// compare 4 elements
    {
	    int pmin = a[lo] < a[lo+1] ? lo : lo+1;
	    	pmin = a[pmin] < a[lo+2] ? pmin : lo+2;
	    	pmin = a[pmin] < a[lo+3] ? pmin : lo+3;
	    if (pmin != lo)
	    {
	        SWAP(a[lo], a[pmin]);
        }
	    int pmax = a[hi] > a[hi-1] ? hi : hi-1;
            pmax = a[pmax] > a[hi-2] ? pmax : hi-2;
	    if (pmax != hi)
        {
            SWAP(a[hi], a[pmax]);
	    }
	    brute(a, lo+1, hi-1);
    }
}

void eq_sort(int a[], int lo0, int hi0)
{
	int lo = lo0;
	int hi = hi0;

    if (hi==lo)
        return;
	if ((hi-lo) <= 3) {
	    brute(a, lo, hi);
	    return;
	}
	int mid = a[(lo + hi) / 2];
	while (lo < hi) {
	    while (lo<hi && a[lo] < mid) {
    		lo++;
	    }
	    while (lo<hi && a[hi] > mid) {
	    	hi--;
	    }
	    if (lo < hi) {
			SWAP(a[lo], a[hi]);
	    }
	}
	if (hi < lo) {
		SWAP(lo, hi);
	}
	eq_sort(a, lo0, lo);
	eq_sort(a, lo == lo0 ? lo+1 : lo, hi0);
}

void enhanced_quick_sort(_int_array *a)
{
	eq_sort(a->array, 0, a->size-1);
}

//-------------------------------------------------//-------------------------------------------------
void swap(int a[], int i, int j)
{
    //int T;
	SWAP(a[i], a[j]);
}

void QuickSort(int a[], int l, int r)
{
    int M = 7;
    int i;
    int j;
    int v;
    
    if ((r-l)>M)
    {
        i = (r+l)/2;
        if (a[l] > a[i]) swap(a, l, i);     // Tri-Median Method!
        if (a[l] > a[r]) swap(a, l, r);
        if (a[i] > a[r]) swap(a, i, r);

        j = r-1;
        swap(a, i, j);
        i = l;
        v = a[j];
        for(;;)
        {
            while(a[++i] < v);
            while(a[--j] > v);
            if (j < i) break;
            swap (a, i, j);
        }
        swap(a, i, r-1);
        QuickSort(a, l, j);
        QuickSort(a, i+1, r);
    }
}

void InsertionSort(int a[], int lo0, int hi0)
{
    int i;
    int j;
    int v;

    for (i=lo0+1; i<=hi0; i++)
    {
        v = a[i];
        j=i;
        while ((j>lo0) && (a[j-1]>v))
        {
            a[j] = a[j-1];
            j--;
        }
        a[j] = v;
    }
}

void fast_quick_sort(_int_array *a)
{
    QuickSort(a->array, 0, a->size-1);
    InsertionSort(a->array, 0, a->size-1);
}

//-------------------------------------------------//-------------------------------------------------
void msort(int a[], int lo0, int hi0)
{
	int lo = lo0;
	int hi = hi0;

	if (lo >= hi)
	    return;

	int mid = (lo + hi) / 2;

    msort(a, lo, mid);
    msort(a, mid + 1, hi);

	int end_lo = mid;
    int start_hi = mid + 1;

	while ((lo <= end_lo) && (start_hi <= hi))
	{
        if (a[lo] < a[start_hi]) {
            lo++;
        } else if (a[lo] == a[start_hi]) {
            lo++;
            start_hi++;
        } else {
		    int T = a[start_hi];
		    int k;
            for (k = start_hi-1; k >= lo; k--)
                a[k+1] = a[k];

            a[lo] = T;
            lo++;
            end_lo++;
            start_hi++;
        }
    }
}

void merge_sort_ex(_int_array *a)
{
	msort(a->array, 0, a->size-1);
}

//-------------------------------------------------//-------------------------------------------------

/* Function to merge the two halves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* Copy data to temp arrays L[] and R[] */
	memcpy(L, arr+l, n1*sizeof(int));
	memcpy(R, arr+m+1, n2*sizeof(int));
    /*
    for(i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for(j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    */
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there are any */
    
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    //memmove(arr+k, L+i, (n1-i)*sizeof(int));
    //k += n1-i;
    //i = n1;
 
    /* Copy the remaining elements of R[], if there are any */
    
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    //memmove(arr+k+n1-i, R+j, (n2-j)*sizeof(int));
}
 
/* l is for left index and r is right index of the sub-array
  of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    //printf("enter [%d, %d] ", l, r);
    if (l < r)
    {
        int m = (unsigned int)(l+r)>>1; //Same as (l+r)/2, but avoids overflow for large l and r
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
    //printf("exit [%d, %d] ", l, r);
}

void merge_sort(_int_array *a)
{
    // Don't forget to allocate memory for the backing arrays!
    L = malloc(a->size/2*sizeof(int));
    R = malloc(a->size/2*sizeof(int));

    mergeSort(a->array, 0, a->size-1);

    free(L);
    free(R);
}

//-------------------------------------------------
void do_sort(char *name, void (*sort_alg)(_int_array *), _int_array *arr)
{
	//int start, stop;
    struct timeval start, stop;
    double diff;
    
    srand(0);
	gettimeofday(&start, NULL);
	//start = clock();
    shuffle(arr);
	//stop = clock();
	gettimeofday(&stop, NULL);
	diff = (stop.tv_sec-start.tv_sec)*1000.0 + (stop.tv_usec-start.tv_usec)/1000.0;
	printf("\n             shuffle: %.0f ms\n", diff);

    //shuffle(arr);
	//display(arr);
	//printf("\n%d\t%d\t%d\n", arr[0], arr[a->size/2], arr[a->size/2-1]);

	printf("    is_sorted(array): %d\n", is_sorted(arr));
    printf("%20s: ", name);

	gettimeofday(&start, NULL);
	//start = clock();
	sort_alg(arr);
	gettimeofday(&stop, NULL);
	//stop = clock();
	diff = (stop.tv_sec-start.tv_sec)*1000.0 + (stop.tv_usec-start.tv_usec)/1000.0;
	printf("%.0f ms\n", diff);
	//display(arr);
	//printf("%d\t%d\t%d\n", arr->array[0], arr->array[arr->size/2], arr->array[arr->size-1]);
	printf("    is_sorted(array): %d\n", is_sorted(arr));
}

int main(int argc, char **argv) 
{
    _int_array *ptr_array1 = malloc(sizeof(_int_array));
    
    if (argc == 2)          // number of items?
        init(ptr_array1, atoi(argv[1]));
    else
        init(ptr_array1, LIMIT);

    if (sizeof(size_t) == 8)         // check pointer size
        printf("sizeof(ptr_array1->array): %I64d\n", ptr_array1->size*sizeof(int));
    else
        printf("sizeof(ptr_array1->array): %d\n", (int32_t)(ptr_array1->size*sizeof(int)));
    
    //srand( ( unsigned)time(NULL) );

    //do_sort("bubble sort", bubble_sort, ptr_array1);
    //do_sort("bubble sort", bubble_sort, ptr_array1);
    
    //do_sort("bidirectional bubble sort", bidirectional_bubble_sort, ptr_array1);
    //do_sort("bidirectional bubble sort", bidirectional_bubble_sort, ptr_array1);
    
    do_sort("quick sort", quick_sort, ptr_array1);
    do_sort("quick sort", quick_sort, ptr_array1);
    
    //do_sort("heap sort", heap_sort, ptr_array1);
    //do_sort("heap sort", heap_sort, ptr_array1);
    
    //do_sort("quick/bubble sort", quick_bubble_sort, ptr_array1);
    //do_sort("quick/bubble sort", quick_bubble_sort, ptr_array1);
    
    //do_sort("enhanced quick sort", enhanced_quick_sort, ptr_array1);
    //do_sort("enhanced quick sort", enhanced_quick_sort, ptr_array1);
    
    //do_sort("fast quick sort", fast_quick_sort, ptr_array1);
    //do_sort("fast quick sort", fast_quick_sort, ptr_array1);
    
    do_sort("merge sort", merge_sort, ptr_array1);
    do_sort("merge sort", merge_sort, ptr_array1);
 
    free(ptr_array1->array);
    free(ptr_array1);
    
	return 0;
}
