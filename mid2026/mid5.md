# C언어 알고리즘의 빅오(Big-O) 기반 속도 비교

## 1. 빅오 표기법(Big-O Notation) 개요

빅오 표기법($O$)은 알고리즘의 효율성을 상한선(Worst-case) 관점에서 입력 데이터 크기 $N$에 따른 시간 복잡도(Time Complexity)로 나타낸 수학적 표기법입니다. 데이터의 크기 $N$이 증가함에 따라 연산 횟수가 어떻게 증가하는지를 나타내며, 단순 실행 시간이 아닌 알고리즘의 확장성(Scalability)을 평가하는 기준이 됩니다.

---

## 2. 정렬 및 탐색 알고리즘 복잡도 비교

### 2.1 정렬 알고리즘 (Sorting Algorithms)

1. **선택 정렬 (Selection Sort) — $O(N^2)$**
   * 중첩 반복문을 사용하여 매번 최솟값을 찾아 정렬하는 방식입니다.
   * 데이터 개수 $N$에 대하여 약 $\frac{N(N-1)}{2}$번의 비교 연산이 수행되므로 시간 복잡도는 $O(N^2)$입니다.
   * **데이터 폭증 현상**: 데이터 크기가 $N$에서 $5N$으로 $5$배 증가할 경우, 연산 횟수는 $5^2 = 25$배로 대폭 증가합니다.

2. **퀵 정렬 기반 `qsort()` — $O(N \log N)$**
   * C 표준 라이브러리(`<stdlib.h>`)에서 제공하는 `qsort()` 함수는 분할 정재(Divide and Conquer) 방식의 퀵 정렬 알고리즘을 사용합니다.
   * 대용량 데이터 처리 시 평균적으로 $O(N \log N)$의 높은 성능을 보입니다.

### 2.2 탐색 알고리즘 (Search Algorithms)

1. **선형 탐색 (Linear Search) — $O(N)$**
   * 배열의 첫 번째 요소부터 마지막 요소까지 순차적으로 값을 비교하는 방식입니다.
   * 정렬되지 않은 데이터에 적용 가능하지만, 최악의 경우 $N$번의 비교가 필요합니다.

2. **이진 탐색 (Binary Search) — $O(\log N)$**
   * 정렬된 배열의 중앙값과 비교하여 검색 범위를 절반씩 줄여나가는 방식입니다.
   * 100만 개($N = 1,000,000$)의 데이터도 약 20번 이내의 비교만으로 검색을 완료할 수 있어 매우 신속합니다.
   * **전제 조건**: 데이터가 반드시 미리 정렬되어 있어야 합니다.

---

## 3. 속도 비교 실험 소스 코드 (`main.c`)

`<time.h>`의 `clock()` 함수를 사용하여 $O(N^2)$ 알고리즘과 $O(N \log N)$ 알고리즘의 실측 수행 시간을 비교하는 프로그램입니다.

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 50000

int a[N];
int b[N];

// Comparison function for qsort and bsearch
int cmp(const void *x, const void *y) {
    return *(int*)x - *(int*)y;
}

// Selection sort implementation (O(N^2))
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
        }
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    // Initialize arrays with identical random numbers
    for (int i = 0; i < N; i++) {
        a[i] = b[i] = rand();
    }

    // 1. Measure execution time of Selection Sort O(N^2)
    clock_t start = clock();
    selection_sort(a, N);
    double selection_time = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("Selection Sort (N=%d): %.3f sec\n", N, selection_time);

    // 2. Measure execution time of qsort O(N log N)
    start = clock();
    qsort(b, N, sizeof(int), cmp);
    double qsort_time = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("qsort (N=%d): %.3f sec\n", N, qsort_time);

    // 3. Binary Search using bsearch O(log N)
    int target = b[N / 2];
    start = clock();
    int *res = (int*)bsearch(&target, b, N, sizeof(int), cmp);
    double bsearch_time = (double)(clock() - start) / CLOCKS_PER_SEC;

    if (res != NULL) {
        printf("Binary Search Success: Target %d found (Time: %.6f sec)\n", *res, bsearch_time);
    }

    return 0;
}
```

---

## 4. 실습 결과 및 성능 분석

### 4.1 데이터 크기($N$) 변화에 따른 정렬 성능 비교

| 데이터 개수 ($N$) | 선택 정렬 ($O(N^2)$) 측정 시간 | `qsort()` ($O(N \log N)$) 측정 시간 | 비고 |
| :--- | :--- | :--- | :--- |
| **$N = 10,000$** | 약 0.15초 내외 | 약 0.001초 미만 | 정렬 차이 체감 시작 |
| **$N = 50,000$** | 약 3.75초 내외 | 약 0.005초 내외 | **선택 정렬 약 25배 지연** |

* **분석**: $N$이 $10,000$에서 $50,000$으로 $5$배 증가할 때, $O(N^2)$ 알고리즘인 선택 정렬의 연산 시간은 약 $5^2 = 25$배 폭증합니다. 반면 $O(N \log N)$ 알고리즘인 `qsort()`는 미세한 차이만 발생하여 대용량 처리 시의 압도적인 효율성을 증명합니다.

### 4.2 탐색 속도 체감 ($O(N)$ vs $O(\log N)$)

* 정렬된 배열 `b`에서 `bsearch()`를 통한 이진 탐색 시 측정 시간은 `0.000000초`에 가깝게 측정됩니다.
* 데이터 수가 수만 개 이상으로 늘어나더라도 이진 탐색은 불과 15~16회의 연산만으로 대상을 찾아내므로 선형 탐색($O(N)$) 대비 압도적인 탐색 성능을 제공합니다.
