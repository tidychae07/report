# Raylib 기반 무지개 블록깨기 게임 (Visual Studio Community & NuGet)

Visual Studio Community 환경에서 **NuGet 패키지 관리자**를 통해 **Raylib** 라이브러리를 설치하고 구현한 C언어 2D 무지개 블록깨기 게임 프로젝트입니다.

---

## 1. 개발 환경 설정 및 설치 방법

### 개발 환경

- **IDE**: Visual Studio Community (최신 버전)
- **워크로드**: C++를 사용한 데스크톱 개발 (Desktop development with C++)
- **라이브러리 관리**: NuGet Package Manager

### NuGet을 통한 Raylib 설치 절차

1. **프로젝트 생성**:
   - Visual Studio 실행 후 `새 프로젝트 만들기` 선택
   - `C++ 콘솔 앱` 템플릿 선택 후 프로젝트 생성 (예: `RainbowBreakout`)
2. **NuGet 패키지 설치**:
   - 솔루션 탐색기에서 프로젝트 우클릭 -> `NuGet 패키지 관리(N)...` 선택
   - `찾아보기` 탭에서 **`raylib`** 검색
   - 공식 `raylib` 패키지 선택 후 `설치` 클릭
3. **소스 파일 구성**:
   - 생성된 `.cpp` 파일명을 `main.c`로 변경 (또는 새로 생성)
   - 아래 제공된 C 소스 코드를 복사하여 붙여넣기
   - `F5` 또는 `Ctrl + F5`를 눌러 컴파일 및 실행

---

## 2. 주요 기능 및 상태 관리

- **7단계 무지개 블록 레이어**: 상단부터 Red, Orange, Yellow, Green, Blue, Dark Blue, Purple 색상의 블록 배치
- **물리 및 충돌 처리**: 공의 벽면, 패들, 블록 간 충돌 및 반사각 계산
- **게임 상태 관리**:
  - `STATE_START`: 시작 대기 화면 (`SPACE` 키 입력 시 시작)
  - `STATE_PLAYING`: 게임 진행 중
  - `STATE_GAMEOVER`: 공이 패들 밑으로 떨어졌을 때 게임 오버 (`R` 키로 재시작)
  - `STATE_VICTORY`: 70개 블록을 모두 파괴했을 때 승리 (`R` 키로 재시작)
- **조작법**:
  - 좌/우 방향키 (`Left` / `Right` Arrow): 패들 이동
  - `SPACE`: 게임 시작
  - `R`: 게임 재시작

---

## 3. 소스 코드 (`main.c`)

> **Note**: Visual Studio MSVC 컴파일러의 문자 인코딩 에러(C4819 등)를 방지하기 위해 소스 코드 내의 주석, 변수명, UI 출력 문자열은 모두 100% 영문(ASCII)으로 작성되었습니다.

```c
#include "raylib.h"
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define BLOCK_ROWS 7
#define BLOCK_COLS 10
#define TOTAL_BLOCKS (BLOCK_ROWS * BLOCK_COLS)

typedef enum GameState {
    STATE_START,
    STATE_PLAYING,
    STATE_GAMEOVER,
    STATE_VICTORY
} GameState;

typedef struct Paddle {
    float x;
    float y;
    float width;
    float height;
    float speed;
} Paddle;

typedef struct Ball {
    float x;
    float y;
    float radius;
    float speedX;
    float speedY;
} Ball;

typedef struct Block {
    float x;
    float y;
    float width;
    float height;
    bool active;
    Color color;
} Block;

static const Color RAINBOW_COLORS[7] = {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    DARKBLUE,
    PURPLE
};

void InitGame(Paddle *paddle, Ball *ball, Block blocks[BLOCK_ROWS][BLOCK_COLS], int *score, int *blocksLeft, GameState *state) {
    paddle->width = 110.0f;
    paddle->height = 18.0f;
    paddle->x = (SCREEN_WIDTH - paddle->width) / 2.0f;
    paddle->y = SCREEN_HEIGHT - 50.0f;
    paddle->speed = 8.5f;

    ball->radius = 9.0f;
    ball->x = SCREEN_WIDTH / 2.0f;
    ball->y = paddle->y - ball->radius - 2.0f;
    ball->speedX = 5.0f;
    ball->speedY = -5.0f;

    float blockWidth = (SCREEN_WIDTH - 60.0f) / BLOCK_COLS;
    float blockHeight = 20.0f;
    float startX = 30.0f;
    float startY = 60.0f;

    for (int r = 0; r < BLOCK_ROWS; r++) {
        for (int c = 0; c < BLOCK_COLS; c++) {
            blocks[r][c].x = startX + c * blockWidth;
            blocks[r][c].y = startY + r * (blockHeight + 5.0f);
            blocks[r][c].width = blockWidth - 4.0f;
            blocks[r][c].height = blockHeight;
            blocks[r][c].active = true;
            blocks[r][c].color = RAINBOW_COLORS[r];
        }
    }

    *score = 0;
    *blocksLeft = TOTAL_BLOCKS;
    *state = STATE_START;
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rainbow Breakout Game - Visual Studio");
    SetTargetFPS(60);

    Paddle paddle;
    Ball ball;
    Block blocks[BLOCK_ROWS][BLOCK_COLS];
    int score = 0;
    int blocksLeft = TOTAL_BLOCKS;
    GameState state = STATE_START;

    InitGame(&paddle, &ball, blocks, &score, &blocksLeft, &state);

    while (!WindowShouldClose()) {
        // State Update & Inputs
        if (state == STATE_START) {
            if (IsKeyPressed(KEY_SPACE)) {
                state = STATE_PLAYING;
            }
        }
        else if (state == STATE_PLAYING) {
            if (IsKeyDown(KEY_LEFT) && paddle.x > 0) {
                paddle.x -= paddle.speed;
            }
            if (IsKeyDown(KEY_RIGHT) && paddle.x + paddle.width < SCREEN_WIDTH) {
                paddle.x += paddle.speed;
            }

            ball.x += ball.speedX;
            ball.y += ball.speedY;

            // Wall Collision
            if (ball.x - ball.radius <= 0 || ball.x + ball.radius >= SCREEN_WIDTH) {
                ball.speedX *= -1.0f;
            }

            if (ball.y - ball.radius <= 0) {
                ball.speedY *= -1.0f;
            }

            if (ball.y + ball.radius >= SCREEN_HEIGHT) {
                state = STATE_GAMEOVER;
            }

            // Paddle Collision
            Rectangle paddleRect = { paddle.x, paddle.y, paddle.width, paddle.height };
            Vector2 ballPos = { ball.x, ball.y };

            if (CheckCollisionCircleRec(ballPos, ball.radius, paddleRect) && ball.speedY > 0) {
                ball.speedY *= -1.0f;
                float hitPos = (ball.x - (paddle.x + paddle.width / 2.0f)) / (paddle.width / 2.0f);
                ball.speedX = hitPos * 6.0f;
            }

            // Block Collision
            bool hitBlock = false;
            for (int r = 0; r < BLOCK_ROWS && !hitBlock; r++) {
                for (int c = 0; c < BLOCK_COLS && !hitBlock; c++) {
                    if (blocks[r][c].active) {
                        Rectangle blockRect = { blocks[r][c].x, blocks[r][c].y, blocks[r][c].width, blocks[r][c].height };

                        if (CheckCollisionCircleRec(ballPos, ball.radius, blockRect)) {
                            blocks[r][c].active = false;
                            ball.speedY *= -1.0f;
                            score += (BLOCK_ROWS - r) * 10;
                            blocksLeft--;

                            if (blocksLeft <= 0) {
                                state = STATE_VICTORY;
                            }
                            hitBlock = true;
                        }
                    }
                }
            }
        }
        else if (state == STATE_GAMEOVER || state == STATE_VICTORY) {
            if (IsKeyPressed(KEY_R)) {
                InitGame(&paddle, &ball, blocks, &score, &blocksLeft, &state);
            }
        }

        // Rendering
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw Bricks
        for (int r = 0; r < BLOCK_ROWS; r++) {
            for (int c = 0; c < BLOCK_COLS; c++) {
                if (blocks[r][c].active) {
                    Rectangle bRect = { blocks[r][c].x, blocks[r][c].y, blocks[r][c].width, blocks[r][c].height };
                    DrawRectangleRec(bRect, blocks[r][c].color);
                    DrawRectangleLines((int)blocks[r][c].x, (int)blocks[r][c].y, (int)blocks[r][c].width, (int)blocks[r][c].height, RAYWHITE);
                }
            }
        }

        // Draw Paddle & Ball
        Rectangle pRect = { paddle.x, paddle.y, paddle.width, paddle.height };
        DrawRectangleRec(pRect, LIGHTGRAY);
        DrawRectangleLines((int)paddle.x, (int)paddle.y, (int)paddle.width, (int)paddle.height, WHITE);
        DrawCircle((int)ball.x, (int)ball.y, ball.radius, WHITE);

        // UI Text Rendering
        DrawText(TextFormat("SCORE: %04d", score), 20, 20, 20, GREEN);
        DrawText(TextFormat("BLOCKS: %02d", blocksLeft), SCREEN_WIDTH - 150, 20, 20, YELLOW);

        // State Overlay
        if (state == STATE_START) {
            DrawText("PRESS [SPACE] TO START", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 50, 20, RAYWHITE);
        } else if (state == STATE_GAMEOVER) {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 - 20, 40, RED);
            DrawText("Press [R] to Restart", SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 30, 20, RAYWHITE);
        } else if (state == STATE_VICTORY) {
            DrawText("VICTORY!", SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 - 20, 40, GOLD);
            DrawText("Press [R] to Restart", SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 30, 20, RAYWHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

---

## 4. 핵심 로직 설명

1. **무지개 색상 상수 배열 (`RAINBOW_COLORS`)**:
   Raylib의 기본 `Color` 구조체를 사용하여 빨강(`RED`)부터 보라(`PURPLE`)까지 7단계 행별 색상을 부여합니다.

2. **충돌 판정 함수 (`CheckCollisionCircleRec`)**:
   Raylib 제공 내장 함수를 활용하여 공의 원형 영역과 패들 및 블록의 사각형 영역 간 충돌을 정확히 계산합니다.

3. **가산점 계산식**:
   높은 위치(상단 행)의 블록일수록 파괴 시 더 높은 점수(`(BLOCK_ROWS - r) * 10`)를 획득하도록 설정되었습니다.
