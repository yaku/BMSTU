#include <iostream>
#include <algorithm>

using namespace std;

/*! Bubble sort.
* \param [in] items − array to print
* \param [in] size − size of array
* \return void
*/
template <class T>
void bubble_sort(T *items, int size)
{
	register int i, j;
	T tmp;

	for (i = 1; i < size; i++)
		for (j = size-1; j >= i; j--)
			if (items[j-1] > items[j]) {

				tmp = items[j-1];

				items[j-1] = items[j];
				items[j] = tmp;
			}
}

/*! Quick sort.
* \param [in] items − array to sort
* \param [in] leftarg − number of elements in left side of 
the array (or part of the array)
* \param [in] rightarg − number of elements in right side of 
the array (or part of the array)
* \return void
*/
template<class T> 
void quicksort(T *items, const int& leftarg, const int& rightarg)
{
	if (leftarg < rightarg) {

		T pivotvalue = items[leftarg];
		int left = leftarg - 1;
		int right = rightarg + 1;

		for(;;) {

			while (items[--right] > pivotvalue);
			while (items[++left] < pivotvalue);

			if (left >= right)
				break;

			T temp = items[right];
			items[right] = items[left];
			items[left] = temp;
		}

		int pivot = right;
		quicksort(items, leftarg, pivot);
		quicksort(items, pivot + 1, rightarg);
	}
}

/*! Prints out elements of array.
* \param [in] items − array to print
* \param [in] size − size of array
* \return void
*/
template <class T> 
void show_items(T *items, int size)
{
	int i;

	for(i=0; i < size; i++)
		cout << items[i] << ", ";
	cout << endl << endl;
}

/*! Merge sort.
* \param [in] items − array to print
* \param [in] size − size of array
* \return void
*/
template <typename T>
void mergesort(T * items, const int size)
{
	if(size < 2)
		return;

	int numInBottom = (int)ceil((double)size/2.0);
	int numInTop = size - numInBottom;

	mergesort<T>(items, numInBottom);
	mergesort<T>(items + numInBottom, numInTop);

	merge<T>(items, items+numInBottom, numInBottom, numInTop);
}


/*! Merge lists by pointers.
* \param [in] bottom − bottom list address
* \param [in] top − top list address
* \param [in] numInBottom − Size of the bottom list
* \param [in] numInTop − Size of the top list
* \return void
*/
template <typename T>
void merge(T * bottom, T * top, const int numInBottom, const int numInTop)
{
	int bIndex = 0, tIndex = 0;

	T * result = new T[numInBottom+numInTop];

	while((bIndex < numInBottom) && (tIndex < numInTop))
	{

		if(bottom[bIndex] < top[tIndex])
			result[(bIndex++)+tIndex] = bottom[bIndex];
		else
			result[bIndex+(tIndex++)] = top[tIndex];
	}

	if(bIndex < numInBottom)
		memcpy(result + (bIndex+tIndex), bottom + bIndex, sizeof(T)*(numInBottom - bIndex));
	else
		memcpy(result + (bIndex+tIndex), top + tIndex, sizeof(T)*(numInTop - tIndex));

	memcpy(bottom, result, sizeof(T)*(numInTop+numInBottom));

	delete [] result;
}


int main(void)
{
	int iarray[7];
	double darray[5];

	//! Randomize arrays.
	for( int i = 0; i < 7; i++ ) iarray [ i ] = -21 + (rand() % 750 );
	for( int i = 0; i < 5; i++ ) darray [ i ] = -21 + (rand() % 750 ) * 2.71828182845904523536;

	//! Then shuffle.
	random_shuffle(iarray, iarray + 6);
	random_shuffle(darray, darray + 4);

	//! Print unsorted arrays.
	cout << "Bubble sort" << endl;
	cout << "Here is unsorted integer array: " << endl;
	show_items(iarray, 7);
	cout << "Here is unsorted double array: " << endl;
	show_items(darray, 5);

	//! Bubble sort.
	bubble_sort(iarray, 7);
	bubble_sort(darray, 5);

	//! Print sorted arrays.
	cout << "Here is sorted integer array: " << endl;
	show_items(iarray, 7);
	cout << "Here is sorted double array: " << endl;
	show_items(darray, 5);

	random_shuffle(iarray, iarray + 6);
	random_shuffle(darray, darray + 4);

	//! Print unsorted arrays.
	cout << "Quick sort" << endl;
	cout << "Here is unsorted integer array: " << endl;
	show_items(iarray, 7);
	cout << "Here is unsorted double array: " << endl;
	show_items(darray, 5);

	//! Quick sort.
	quicksort(iarray, 0, 6);
	quicksort(darray, 0, 4);

	//! Print unsorted arrays.
	cout << "Here is sorted integer array: " << endl;
	show_items(iarray, 7);
	cout << "Here is sorted double array: " << endl;
	show_items(darray, 5);

	random_shuffle(iarray, iarray + 6);
	random_shuffle(darray, darray + 4);

	//! Print sorted arrays.
	cout << "Merge sort" << endl;
	cout << "Here is unsorted integer array: " << endl;
	show_items(iarray, 7);
	cout << "Here is unsorted double array: " << endl;
	show_items(darray, 5);

	//! Merge sort.
	mergesort(iarray, 7);
	mergesort(darray, 5);

	//! Print unsorted arrays.
	cout << "Here is sorted integer array: " << endl;
	show_items(iarray, 7);
	cout << "Here is sorted double array: " << endl;
	show_items(darray, 5);
}