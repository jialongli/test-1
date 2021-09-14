//https://rtoax.blog.csdn.net/article/details/108333441
//快速排序又是一种分而治之思想在排序算法上的典型应用。

//快速排序是由东尼·霍尔所发展的一种排序算法。在平均状况下，排序 n 个项目要 Ο(nlogn) 次比较。
//在最坏状况下则需要 Ο(n2) 次比较，但这种状况并不常见。
//事实上，快速排序通常明显比其他 Ο(nlogn) 算法更快，因为它的内部循环（inner loop）可以在大部
//分的架构上很有效率地被实现出来。
//
//快速排序使用分治法（Divide and conquer）策略来把一个串行（list）分为两个子串行（sub-lists）。
//
//快速排序又是一种分而治之思想在排序算法上的典型应用。本质上来看，快速排序应该算是在冒泡排序基础上的递归分治法。
//
//快速排序的名字起的是简单粗暴，因为一听到这个名字你就知道它存在的意义，就是快，而且效率高！
//它是处理大数据最快的排序算法之一了。虽然 Worst Case 的时间复杂度达到了 O(n²)，但是人家就是优秀，
//在大多数情况下都比平均时间复杂度为 O(n logn) 的排序算法表现要更好，可是这是为什么呢，我也不知道。
//好在我的强迫症又犯了，查了 N 多资料终于在《算法艺术与信息学竞赛》上找到了满意的答案：
//
//快速排序的最坏运行情况是 O(n²)，比如说顺序数列的快排。但它的平摊期望时间是 O(nlogn)，
//且 O(nlogn) 记号中隐含的常数因子很小，比复杂度稳定等于 O(nlogn) 的归并排序要小很多。
//所以，对绝大多数顺序性较弱的随机数列而言，快速排序总是优于归并排序。
//
//算法步骤
//
//1. 从数列中挑出一个元素，称为 "基准"（pivot）;
//
//2. 重新排序数列，所有元素比基准值小的摆放在基准前面，所有元素比基准值大的摆在基准的后面（相同的数可以到任一边）。
//    在这个分区退出之后，该基准就处于数列的中间位置。这个称为分区（partition）操作；
//
//3. 递归地（recursive）把小于基准值元素的子数列和大于基准值元素的子数列排序；

#if !recursive
/**
 *  非递归算法 
 */

typedef struct _Range {
    int start, end;
} Range;
 
Range new_Range(int s, int e) {
    Range r;
    r.start = s;
    r.end = e;
    return r;
}
 
void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}
 
void quick_sort(int arr[], const int len) {
    if (len <= 0)
        return; // 避免len等於負值時引發段錯誤（Segment Fault）
    // r[]模擬列表,p為數量,r[p++]為push,r[--p]為pop且取得元素
    Range r[len];
    int p = 0;
    r[p++] = new_Range(0, len - 1);
    while (p) {
        Range range = r[--p];
        if (range.start >= range.end)
            continue;
        int mid = arr[(range.start + range.end) / 2]; // 選取中間點為基準點
        int left = range.start, right = range.end;
        do {
            while (arr[left] < mid) ++left;   // 檢測基準點左側是否符合要求
            while (arr[right] > mid) --right; //檢測基準點右側是否符合要求
            if (left <= right) {
                swap(&arr[left], &arr[right]);
                left++;
                right--;               // 移動指針以繼續
            }
        } while (left <= right);
        if (range.start < right) r[p++] = new_Range(range.start, right);
        if (range.end > left) r[p++] = new_Range(left, range.end);
    }
}

#else  //recursive

/**
 *  递归算法 
 */
void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}
 
void quick_sort_recursive(int arr[], int start, int end)
{
    if (start >= end)
        return;
    
    int mid = arr[end];
    int left = start, right = end - 1;
    
    while (left < right) {
        while (arr[left] < mid && left < right)
            left++;
        while (arr[right] >= mid && left < right)
            right--;
        swap(&arr[left], &arr[right]);
    }

    if (arr[left] >= arr[end])
        swap(&arr[left], &arr[end]);
    else
        left++;
    
    if (left)
        quick_sort_recursive(arr, start, left - 1);
    
    quick_sort_recursive(arr, left + 1, end);
}
 
void quick_sort(int arr[], int len) {
    quick_sort_recursive(arr, 0, len - 1);
}


#endif

