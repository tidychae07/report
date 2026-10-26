#숫자야구 판정 로직 (기초 입출력 및 조건문)

## 1. 문제 개요
* **평가 범위**: 1주차 (변수, 연산자, 조건문)
* **문제 설명**: 정답 숫자 3개(중복 없음)와 사용자가 입력한 숫자 3개가 주어질 때, 볼(Ball)과 스트라이크(Strike) 개수를 계산하여 출력하는 프로그램 구현
* **판정 규칙**:
  * 숫자와 위치가 모두 일치: `Strike`
  * 숫자는 일치하나 위치가 다름: `Ball`
  * 3 Strike 달성 시: `"HOMERUN"` 추가 출력

---

## 2. 소스 코드 (`main.c`)

```c
#include <stdio.h>

int main(void) {
    // 1. 변수 선언 (정답 숫자 3개, 사용자 입력 숫자 3개, 카운트 변수)
    int ans1, ans2, ans3;
    int user1, user2, user3;
    int strike = 0;
    int ball = 0;

    // 2. 정답 및 사용자 입력 받기
    scanf("%d %d %d", &ans1, &ans2, &ans3);
    scanf("%d %d %d", &user1, &user2, &user3);

    // 3. Strike 판정 (숫자와 위치가 모두 같은 경우)
    if (user1 == ans1) {
        strike++;
    }
    if (user2 == ans2) {
        strike++;
    }
    if (user3 == ans3) {
        strike++;
    }

    // 4. Ball 판정 (숫자는 존재하지만 위치가 다른 경우)
    if (user1 == ans2 || user1 == ans3) {
        ball++;
    }
    if (user2 == ans1 || user2 == ans3) {
        ball++;
    }
    if (user3 == ans1 || user3 == ans2) {
        ball++;
    }

    // 5. 결과 출력
    printf("%d Strike, %d Ball\n", strike, ball);

    // 6. 3 Strike일 경우 HOMERUN 출력
    if (strike == 3) {
        printf("HOMERUN\n");
    }

    return 0;
}
```

---

## 3. 입출력 및 실행 예시

### 예시 1 (1 Strike, 1 Ball)
* **입력**:
  ```text
  1 2 3
  1 3 5
  ```
* **출력**:
  ```text
  1 Strike, 1 Ball
  ```

### 예시 2 (3 Strike / HOMERUN)
* **입력**:
  ```text
  7 8 9
  7 8 9
  ```
* **출력**:
  ```text
  3 Strike, 0 Ball
  HOMERUN
  ```

### 예시 3 (OUT)
* **입력**:
  ```text
  1 2 3
  4 5 6
  ```
* **출력**:
  ```text
  0 Strike, 0 Ball
  ```

---

## 4. 로직 설명

1. **변수 활용**:
   * 배열 대신 기초 변수(`ans1~3`, `user1~3`)를 활용하여 입력 값을 저장합니다.
2. **Strike 연산**:
   * 동일 위치의 변수를 비교(`user1 == ans1`)하여 조건이 참일 때 `strike` 카운트를 증가시킵니다.
3. **Ball 연산**:
   * 서로 다른 위치의 변수와 비교 논리 연산자(`||`)를 활용해 위치는 다르지만 값이 같은 경우 `ball` 카운트를 증가시킵니다.
4. **조건문 출력**:
   * `strike` 값이 3일 경우 추가로 `HOMERUN` 문구를 출력하도록 단일 조건문을 구성합니다.
