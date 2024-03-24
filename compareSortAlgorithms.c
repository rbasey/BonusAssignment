#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}
//function to swap two integers
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
//this function needed for heap sort
void heapify(int pData[], int n, int i) {
	int large = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if(right < n && pData[right] > pData[large]) {
		large = right;
	}

	if(left < n && pData[left] > pData[large]) {
		large = left;
	}

	if(large != i) {
		swap(&(pData[i]), &(pData[large]));
		heapify(pData, n, large);
	}
}
// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int pData[], int i, int n)
{
	for(i = n/2; i >=0; i--) {
		heapify(pData, n, i);
	}
	for(i = n; i >= 0; i--) {
		swap(&(pData[0]), &(pData[i]));
		heapify(pData, i, 0);
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if(l < r) {
		//m is the mid point
		int m = (l+r)/2;
		//first half
		mergeSort(pData, l, m);
		//second half
		mergeSort(pData, m+1, r);
		
		//merge two halfs
		int i, j, k;
		int n1 = m - l + 1;
		int n2 = r - m;
		//temp left array
		int *left = Alloc(sizeof(int)*n1);
		//temp right array
		int *right = Alloc(sizeof(int)*n2);
		//copy data to left
		for(i = 0; i < n1; i++) {
		left[i] = pData[l+i];
		}
		//copy data to right
		for(j = 0; j < n2; j++) {
		right[j] = pData[m+1+j];
		}
		//merge back into pData
		i = 0;
		j = 0;
		k = l;
		while(i < n1 && j < n2) {
			if(left[i] <= right[j]) {
				pData[k] = left[i];
				i++;
			} else {
				pData[k] = right[j];
				j++;
			}
			k++;
		}
		//copy remaining elements of left
		while(i < n1) {
			pData[k] = left[i];
			i++;
			k++;
		}
		//copy remaining elements of right
		while(j < n2) {
			pData[k] = right[j];
			j++;
			k++;
		}

		//free the temp arrays 
		DeAlloc(left);
		DeAlloc(right);
	}
}
// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, item, j;
	for(i = 1; i < n; i++){
		item = pData[i];
		for(j=i-1; j >= 0; j--) {
			if(pData[j] > item) {
				//shift items
				pData[j+1] = pData[j];
			} else {
				break;
			}
		}
		pData[j+1] = item;
	}
	
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int i, j;
	//compare adjacent items
	for(i = 0; i < n-1; i++) {
		for(j = 0; j < n-i-1; j++){
			if(pData[j] > pData[j+1]) {
				//swap when element to the right is larger
				swap(&(pData[j]), &(pData[j+1]));
			}
		}
	}
	
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i, j, min_idx;
	//increment i
	for(i = 0; i < n-1; i++) {
		//look for the minimum element
		min_idx = i;
		for(j=i+1; j < n; j++) {
			if(pData[j]<pData[min_idx]) {
				min_idx = j;
			}
		}
		//swap the minimum element and element at i
		if(min_idx != i) {
		swap(&(pData[i]), &(pData[min_idx]));
		}
	}
	
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	int i, n, *data;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL) {
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for(i = 0; i < dataSz; ++i) {
			fscanf(inFile, "%d", &n);
			data = *ppData + i;
			*data = n;
		}
		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
