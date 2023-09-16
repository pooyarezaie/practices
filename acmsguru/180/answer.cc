#include <iostream>

const int MAX_N = 65537 + 10;
int input_array[MAX_N];
int temp_array[MAX_N];

long long int merge(int arr[], int start, int mid, int end);
long long int merge_sort(int arr[], int start, int end);

int main() {
    int n; std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin>>input_array[i];
    }
    std::cout<<merge_sort(input_array, 0, n)<<std::endl;
    return 0;
}

long long int merge_sort(int arr[], int start, int end) {
    if (end - start <= 1) {
        return 0;
    }
    int mid = (start + end) / 2;
    long long int inversions_left = merge_sort(arr, start, mid);
    long long int inversions_right = merge_sort(arr, mid, end);
    long long int between_inversions = merge(arr, start, mid, end); 
    return inversions_left + inversions_right + between_inversions;
}

long long int merge(int arr[], int start, int mid, int end) {
    for (int i = start; i < end; ++i) {
        temp_array[i] = arr[i];
    }
    int p1 = start, p2 = mid, index = start;
    long long int inversions = 0;

    while (p1 < mid && p2 < end) {
        if (temp_array[p1] <= temp_array[p2]) {
            arr[index] = temp_array[p1];
            p1++; index++;
        } else  {
            arr[index] = temp_array[p2];
            inversions += mid - p1;
            index++; p2++;
        }
    }
    while (p1 < mid) {
        arr[index] = temp_array[p1];
        index++; p1++;
    }
    while (p2 < end) { 
        arr[index] = temp_array[p2];
        index++; p2++;
    }
    return inversions;
}
