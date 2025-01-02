


/*

gcc -g -o bubble_sort bubble_sort.c
gdb bubble_sort
break bubbleSort
run
Here are some useful commands:

    next (abbreviated as n): Execute the current line and move to the next line.
    step (abbreviated as s): Execute the current line and move into any function call on that line.
    continue (abbreviated as c): Continue execution until the next breakpoint.
    print <variable> (abbreviated as p <variable>): Print the current value of a variable.
    list: Show the source code around the current execution point.

*/






#include <stdio.h>

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // Swap arr[j] and arr[j+1]
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr)/sizeof(arr[0]);
    printf("Array before sorting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    bubbleSort(arr, n);
    
    printf("Array after sorting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}




