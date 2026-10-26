# 과제: C언어 그래픽 라이브러리를 활용한 무지개 블록깨기 게임

C언어와 크로스플랫폼 2D 그래픽 라이브러리인 **Raylib**을 활용하여 구현한 7단계 무지개 블록깨기(Breakout) 게임 프로젝트입니다.

---

## 1. 주요 기능

* **7단계 무지개 블록 레이어**: 빨강, 주황, 노랑, 초록, 파랑, 남색, 보라 색상의 블록 배치
* **물리 및 충돌 처리**: 공의 벽면, 패들, 블록 충돌 및 반사각 계산
* **게임 상태 관리**: 
  * `START`: 게임 시작 대기
  * `PLAYING`: 게임 진행 중
  * `GAMEOVER`: 공 추락 시 실패
  * `VICTORY`: 모든 블록 파괴 시 승리
* **점수 및 재시작 시스템**: 상단 블록 파괴 시 가산점 부여, `R` 키로 즉시 재시작 가능

---

## 2. 개발 환경 및 빌드 방법

### 개발 환경
* **언어**: C (C99 이상)
* **라이브러리**: Raylib 4.5+
* **컴파일러**: GCC / Clang / MSVC

### 빌드 및 실행 명령어 (GCC 기준)

```bash
# Ubuntu / Debian
sudo apt-get install libraylib-dev
gcc -o breakout main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./breakout

# macOS (Homebrew)
brew install raylib
gcc -o breakout main.c -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
./breakout
```

---

## 3. 소스 코드 (`main.c`)

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
    bool active;
} Ball;

typedef struct Block {
    float x;
    float y;
    float width;
    float height;
    bool active;
    Color color;
} Block;

const Color RAINBOW_COLORS[7] = {
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
    ball->active = true;

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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rainbow Block Breaker - C/Raylib");
    SetTargetFPS(60);

    Paddle paddle;
    Ball ball;
    Block blocks[BLOCK_ROWS][BLOCK_COLS];
    int score = 0;
    int blocksLeft = TOTAL_BLOCKS;
    GameState state = STATE_START;

    InitGame(&paddle, &ball, blocks, &score, &blocksLeft, &state);

    while (!WindowShouldClose()) {
        // ----------------------------------------------------
        // 1. 상태 업데이트 및 키 입력
        // ----------------------------------------------------
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

            if (ball.x - ball.radius <= 0 || ball.x + ball.radius >= SCREEN_WIDTH) {
                ball.speedX *= -1.0f;
            }

            if (ball.y - ball.radius <= 0) {
                ball.speedY *= -1.0f;
            }

            if (ball.y + ball.radius >= SCREEN_HEIGHT) {
                state = STATE_GAMEOVER;
            }

            Rectangle paddleRect = { paddle.x, paddle.y, paddle.width, paddle.height };
            Vector2 ballPos = { ball.x, ball.y };

            if (CheckCollisionCircleRec(ballPos, ball.radius, paddleRect) && ball.speedY > 0) {
                ball.speedY *= -1.0f;
                float hitPos = (ball.x - (paddle.x + paddle.width / 2.0f)) / (paddle.width / 2.0f);
                ball.speedX = hitPos * 6.0f;
            }

            for (int r = 0; r < BLOCK_ROWS; r++) {
                for (int c = 0; c < BLOCK_COLS; c++) {
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
                            goto COLLISION_HANDLED;
                        }
                    }
                }
            }
            COLLISION_HANDLED:;
        } 
        else if (state == STATE_GAMEOVER || state == STATE_VICTORY) {
            if (IsKeyPressed(KEY_R)) {
                InitGame(&paddle, &ball, blocks, &score, &blocksLeft, &state);
            }
        }

        // ----------------------------------------------------
        // 2. 렌더링
        // ----------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        for (int r = 0; r < BLOCK_ROWS; r++) {
            for (int c = 0; c < BLOCK_COLS; c++) {
                if (blocks[r][c].active) {
                    DrawRectangleRec((Rectangle){ blocks[r][c].x, blocks[r][c].y, blocks[r][c].width, blocks[r][c].height }, blocks[r][c].color);
                    DrawRectangleLines((int)blocks[r][c].x, (int)blocks[r][c].y, (int)blocks[r][c].width, (int)blocks[r][c].height, RAYWHITE);
                }
            }
        }

        DrawRectangleRec((Rectangle){ paddle.x, paddle.y, paddle.width, paddle.height }, LIGHTGRAY);
        DrawRectangleLines((int)paddle.x, (int)paddle.y, (int)paddle.width, (int)paddle.height, WHITE);
        DrawCircle((int)ball.x, (int)ball.y, ball.radius, WHITE);

        DrawText(TextFormat("SCORE: %04d", score), 20, 20, 20, GREEN);
        DrawText(TextFormat("BLOCKS: %02d", blocksLeft), SCREEN_WIDTH - 150, 20, 20, YELLOW);

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

1. **무지개 색상 배열 (`RAINBOW_COLORS`)**:
   `RED`부터 `PURPLE`까지 7가지 색상을 상단부터 차례대로 블록 행에 매핑.
2. **충돌 판정 (`CheckCollisionCircleRec`)**:
   Raylib의 기본 충돌 함수를 사용하여 공과 패들/블록 간의 접촉을 계산하고, 충돌 지점에 따라 반사각 조정.
3. **가산점 방식**:
   상단에 위치한 블록을 파괴할수록 더 높은 점수(`(BLOCK_ROWS - r) * 10`) 부여.
