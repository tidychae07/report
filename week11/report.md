# 11강 보조 자료 코딩하기 및 퀴즈2 코딩하기 (2026.11.16)

> 작성자: 20261458 채우찬

## 1. scanf 및 문자열 처리

### 원래 문제
공백이 포함된 문장을 `scanf("%[^\n]")`로 입력받아 거꾸로 출력하기

### 변형한 부분
- 문자열을 거꾸로 출력한 뒤, **앞뒤가 같은 문장(회문)인지 판별**하는 기능 추가

### 변형 코드
```c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void) {
    char str[100];
    int len, i, same = 1;

    printf("문장 입력: ");
    scanf("%[^\n]", str);

    len = (int)strlen(str);

    printf("거꾸로 출력: ");
    for (i = len - 1; i >= 0; i--) {
        printf("%c", str[i]);
    }
    printf("\n");

    for (i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            same = 0;
            break;
        }
    }

    printf("판별 결과: %s\n", same ? "앞뒤가 같은 문장입니다." : "앞뒤가 다른 문장입니다.");
    return 0;
}
```

### 코드 설명
- `strlen()`으로 문자열 길이를 구한 뒤 끝 인덱스부터 거꾸로 출력
- 반복문으로 문자열의 양 끝(`str[i]`와 `str[len - 1 - i]`)을 절반까지만 비교하여 회문 여부 판별

### 실행 결과
```text
문장 입력: level
거꾸로 출력: level
판별 결과: 앞뒤가 같은 문장입니다.
```

---

## 2. 함수의 포인터

### 원래 문제
함수 포인터 `int (*sum)(int, int)`에 `add` 함수를 저장하고 호출하여 두 수의 합 출력

### 변형한 부분
- 덧셈(`add`) 외에 뺄셈(`sub`), 곱셈(`mul`) 함수를 추가하고 **함수 포인터 하나로 연결을 바꿔가며 호출**

### 변형 코드
```c
#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

int main(void) {
    int a = 22, b = 11;
    int (*fp)(int, int);

    fp = add;
    printf("덧셈: %d\n", fp(a, b));

    fp = sub;
    printf("뺄셈: %d\n", fp(a, b));

    fp = mul;
    printf("곱셈: %d\n", fp(a, b));

    return 0;
}
```

### 코드 설명
- 동일한 매개변수와 반환형을 가진 함수들의 주소를 함수 포인터 `fp`에 대입하여 호출
- 배열이나 복잡한 구조 없이 포인터 변수 하나로 여러 함수를 가리키는 기본 원리 활용

### 실행 결과
```text
덧셈: 33
뺄셈: 11
곱셈: 242
```

---

## 3. 2차원 배열

### 원래 문제
3×4 2차원 배열에 1~12를 저장하고 전체 합 출력

### 변형한 부분
- 배열 크기를 **4×3**으로 변경하고, 전체 합과 함께 **각 행의 합**도 출력

### 변형 코드
```c
#include <stdio.h>

int main(void) {
    int a[4][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10, 11, 12}
    };
    int i, j, rowSum, total = 0;

    for (i = 0; i < 4; i++) {
        rowSum = 0;
        for (j = 0; j < 3; j++) {
            rowSum += a[i][j];
        }
        printf("%d행 합: %d\n", i + 1, rowSum);
        total += rowSum;
    }

    printf("전체 합: %d\n", total);
    return 0;
}
```

### 코드 설명
- 바깥쪽 `for`문에서 행이 바뀔 때마다 `rowSum`을 `0`으로 초기화하여 행별 합계를 계산하고 `total`에 누적

### 실행 결과
```text
1행 합: 6
2행 합: 15
3행 합: 24
4행 합: 33
전체 합: 78
```

---

## 4. 구조체 + 바이너리 파일 저장

### 원래 문제
구조체로 2명의 3과목 성적을 생성해서 바이너리 형태로 `my.bin`에 저장

### 변형한 부분
- **3명의 4과목(국, 영, 수, 과)** 성적으로 데이터를 확장하여 저장

### 변형 코드
```c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

struct S {
    int k, e, m, s;
};

int main(void) {
    struct S a[3] = {
        {85, 90, 78, 92},
        {72, 88, 95, 80},
        {90, 76, 84, 88}
    };
    FILE *p = fopen("my.bin", "wb");

    fwrite(a, sizeof(struct S), 3, p);
    fclose(p);

    printf("저장 완료\n");
    return 0;
}
```

### 코드 설명
- 구조체 멤버에 과학(`s`)을 추가하고, `fwrite()`로 구조체 배열 3개 분량의 데이터를 `my.bin`에 한 번에 기록

### 실행 결과
```text
저장 완료
```

---

## 5. 바이너리 파일 읽기 + 평균 계산

### 원래 문제
`my.bin` 파일을 읽고 개인 평균, 과목 평균 출력

### 변형한 부분
- 4번에서 저장한 **3명 4과목** 데이터를 읽어 반복문으로 개인 평균과 과목 평균 출력

### 변형 코드
```c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

struct S {
    int k, e, m, s;
};

int main(void) {
    struct S a[3];
    int i, sk = 0, se = 0, sm = 0, ss = 0;
    FILE *p = fopen("my.bin", "rb");

    fread(a, sizeof(struct S), 3, p);
    fclose(p);

    for (i = 0; i < 3; i++) {
        printf("학생%d 평균: %.2f\n", i + 1, (a[i].k + a[i].e + a[i].m + a[i].s) / 4.0);
        sk += a[i].k; se += a[i].e; sm += a[i].m; ss += a[i].s;
    }

    printf("과목 평균 -> 국어: %.2f, 영어: %.2f, 수학: %.2f, 과학: %.2f\n",
           sk / 3.0, se / 3.0, sm / 3.0, ss / 3.0);
    return 0;
}
```

### 코드 설명
- `fread()`로 바이너리 파일 내용을 구조체 배열 `a`에 읽어온 뒤, 반복문을 돌며 개인별 평균 계산 및 과목별 총점 누적

### 실행 결과
```text
학생1 평균: 86.25
학생2 평균: 83.75
학생3 평균: 84.50
과목 평균 -> 국어: 82.33, 영어: 84.67, 수학: 85.67, 과학: 86.67
```

---

## 6. 버블 정렬

### 원래 문제
`{2, 4, 1, 5, 3}`을 버블 정렬(오름차순)

### 변형한 부분
- 기존 코드에 **회차마다 배열 상태를 출력**하는 코드만 추가

### 변형 코드
```c
#include <stdio.h>

int main(void) {
    int a[] = { 2, 4, 1, 5, 3 }, i, j, k, t;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4 - i; j++) {
            if (a[j] > a[j + 1]) {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
        printf("%d회차: ", i + 1);
        for (k = 0; k < 5; k++) printf("%d ", a[k]);
        printf("\n");
    }
    return 0;
}
```

### 코드 설명
- 안쪽 `for`문(1회차 비교 및 교환)이 끝날 때마다 현재 배열 전체를 출력하여 정렬 과정을 직관적으로 확인

### 실행 결과
```text
1회차: 2 1 4 3 5 
2회차: 1 2 3 4 5 
3회차: 1 2 3 4 5 
4회차: 1 2 3 4 5 
```

---

## 7. 변수와 합 출력

### 원래 문제
정수형 변수 2개에 각각 22와 33을 저장하고, 두 수의 합을 계산하여 화면에 출력

### 변형한 부분
- 자료형을 **실수형(`double`)**으로 바꾸고 값을 `22.5`, `33.5`로 변경

### 변형 코드
```c
#include <stdio.h>

int main(void) {
    double a = 22.5, b = 33.5;
    printf("%.1f + %.1f = %.1f\n", a, b, a + b);
    return 0;
}
```

### 코드 설명
- 실수 자료형 `double`을 사용하고, 서식 지정자 `%.1f`로 소수점 첫째 자리까지 출력

### 실행 결과
```text
22.5 + 33.5 = 56.0
```

---

## 8. 명령행 인자

### 원래 문제
명령행 인자로 두 정수를 입력받아 합을 출력하는 프로그램 작성

### 변형한 부분
- **세 정수**를 명령행 인자로 입력받아 합을 출력하도록 변경

### 변형 코드
```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int sum = atoi(argv[1]) + atoi(argv[2]) + atoi(argv[3]);
    printf("합계: %d\n", sum);
    return 0;
}
```

### 코드 설명
- 문자열 형태로 전달되는 `argv[1]`, `argv[2]`, `argv[3]`를 `atoi()` 함수로 정수 변환 후 합산

### 실행 결과
```text
> main.exe 10 20 30
합계: 60
```

---

## 9. 반복문으로 합 구하기

### 원래 문제
반복문을 사용하여 5부터 10까지의 정수를 모두 더한 결과 출력

### 변형한 부분
- 범위를 **1부터 20까지**로 변경하고 `while`문 사용

### 변형 코드
```c
#include <stdio.h>

int main(void) {
    int i = 1, sum = 0;

    while (i <= 20) {
        sum += i;
        i++;
    }

    printf("1~20 합: %d\n", sum);
    return 0;
}
```

### 코드 설명
- 제어 변수 `i`를 `1`부터 `20`까지 `1`씩 증가시키며 `sum`에 누적

### 실행 결과
```text
1~20 합: 210
```

---

## 10. 동적 메모리 할당 (malloc)

### 원래 문제
동적 메모리 할당(`malloc`)으로 정수 3개 공간을 할당받아 `{11, 33, 55}`를 저장하고 합 출력(해제 포함)

### 변형한 부분
- 할당 크기를 **정수 4개**로 늘리고 `{10, 20, 30, 40}`의 합과 평균 출력

### 변형 코드
```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = (int *)malloc(sizeof(int) * 4);
    int i, sum = 0;

    for (i = 0; i < 4; i++) {
        p[i] = (i + 1) * 10;
        sum += p[i];
    }

    printf("합: %d, 평균: %.1f\n", sum, sum / 4.0);
    free(p);
    return 0;
}
```

### 코드 설명
- `malloc()`으로 `int` 4개 크기의 힙 메모리를 할당받아 값을 대입·합산한 뒤 `free()`로 메모리 반환

### 실행 결과
```text
합: 100, 평균: 25.0
```

---

## 11. 구조체로 성적 처리

### 원래 문제
3명(`J.C. Shim`, `G.D. Hong`, `J.Y. Ko`)의 성적(국어 `kr`, 영어 `en`)을 구조체로 초기화하고 개인별 평균 출력

### 변형한 부분
- **수학(`ma`) 과목을 추가**해 3과목 평균을 계산하도록 변경

### 변형 코드
```c
#include <stdio.h>

struct Student {
    char name[20];
    int kr, en, ma;
};

int main(void) {
    struct Student st[3] = {
        {"J.C. Shim", 90, 85, 88},
        {"G.D. Hong", 78, 82, 75},
        {"J.Y. Ko",   85, 91, 89}
    };
    int i;

    for (i = 0; i < 3; i++) {
        printf("%s 평균: %.2f\n", st[i].name, (st[i].kr + st[i].en + st[i].ma) / 3.0);
    }
    return 0;
}
```

### 코드 설명
- 구조체 배열 초기화 시 이름과 세 과목 점수를 함께 저장하고, 반복문으로 개인별 평균 출력

### 실행 결과
```text
J.C. Shim 평균: 87.67
G.D. Hong 평균: 78.33
J.Y. Ko 평균: 88.33
```

이 리포트는 생성형 AI툴을 사용하여 작성, 검수되었습니다
