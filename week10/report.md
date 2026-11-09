# 🏢 MERGE TOWN (Raylib Merge Building Game - 5x5)

Raylib과 C언어로 제작한 5x5 그리드 기반 건물 합성(Merge) 퍼즐 게임입니다.  
무작위 위치에 생성되는 1단계 건물을 선택·이동·합성하여 고층 건물과 랜드마크를 세우고 최고 점수를 경신하는 것이 목표입니다.

---

## 🎮 게임 소개 및 규칙

1. **건물 생성**: 하단의 `NEW BUILDING (SPAWN)` 버튼을 누르면 5x5 보드의 빈 공간 중 무작위 위치에 **Lv.1 목조 주택**이 생성됩니다.
2. **선택 및 이동/합성**:
   - 그리드 상의 건물을 클릭하여 선택합니다 (선택 시 **빨간색 테두리** 표시).
   - 선택한 상태에서 **빈 칸**을 클릭하면 해당 위치로 건물이 이동합니다.
   - 선택한 상태에서 **동일한 단계(Lv)의 건물**을 클릭하면 두 건물이 합쳐지며 한 단계 높은 건물로 업그레이드되고 점수를 획득합니다.
3. **게임 오버 판정**: 25칸의 보드에 빈 공간이 없고, 상하좌우로 더 이상 합칠 수 있는 인접 타일이 없으면 게임 오버가 됩니다.
4. **재시작**: 키보드 `R` 키를 누르면 언제든지 게임을 초기 상태로 리셋할 수 있습니다.

---

## 🏗️ 건물 단계 (Tier / Level)

| 단계 | 건물 명칭 | 타일 색상 |
| :---: | :--- | :--- |
| **Lv.1** | 목조 주택 | 밝은 베이지 (`#EEE4DA`) |
| **Lv.2** | 벽돌 집 | 샌드 베이지 (`#EDE0C8`) |
| **Lv.3** | 2층 빌라 | 연한 주황 (`#F2B179`) |
| **Lv.4** | 소형 상가 | 주황 (`#F59563`) |
| **Lv.5** | 중형 빌딩 | 진한 주황 (`#F67C5F`) |
| **Lv.6** | 고층 아파트 | 주황 빨강 (`#F65E3B`) |
| **Lv.7** | 스카이스크래퍼 | 골드 (`#EDCF72`) |
| **Lv.8** | 랜드마크 타워 | 브라이트 골드 (`#EDCC61`) |
| **Lv.9+** | 초고층 빌딩 이상 | 퍼플 (`#9C27B0`) |

---

## 🛠️ 개발 환경 및 빌드 방법

- **개발 언어**: C Language (C11)
- **IDE**: Microsoft Visual Studio 2026
- **그래픽 라이브러리**: [Raylib](https://www.raylib.com/)

### Visual Studio 2026 세팅 방법
1. Visual Studio 2026에서 **C++ 빈 프로젝트(Empty Project)**를 생성합니다.
2. 프로젝트 우클릭 ➔ **NuGet 패키지 관리자(Manage NuGet Packages)** 실행
3. `raylib` 검색 후 패키지 설치
4. `.c` 소스 파일(예: `main.c`) 생성 후 아래 전체 코드를 복사하여 빌드 및 실행 (`F5`)

---

## 💻 소스 코드 (`main.c`)

```c
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define GRID_SIZE 5
#define CELL_SIZE 70
#define CELL_PADDING 10
#define BOARD_OFFSET_X 55
#define BOARD_OFFSET_Y 130

typedef struct {
    int row;
    int col;
} CellPos;

// 타일 단계별 색상 지정
Color GetTierColor(int tier) {
    switch (tier) {
        case 1: return (Color){ 238, 228, 218, 255 }; // 1단계: 목조 주택
        case 2: return (Color){ 237, 224, 200, 255 }; // 2단계: 벽돌 집
        case 3: return (Color){ 242, 177, 121, 255 }; // 3단계: 2층 빌라
        case 4: return (Color){ 245, 149, 99, 255 };  // 4단계: 소형 상가
        case 5: return (Color){ 246, 124, 95, 255 };  // 5단계: 중형 빌딩
        case 6: return (Color){ 246, 94, 59, 255 };   // 6단계: 고층 아파트
        case 7: return (Color){ 237, 207, 114, 255 }; // 7단계: 스카이스크래퍼
        case 8: return (Color){ 237, 204, 97, 255 };  // 8단계: 랜드마크 타워
        default: return (Color){ 156, 39, 176, 255 }; // 9단계 이상
    }
}

// 빈 공간 중 무작위 1단계 건물 생성
bool SpawnBuilding(int grid[GRID_SIZE][GRID_SIZE]) {
    CellPos emptyCells[GRID_SIZE * GRID_SIZE];
    int count = 0;

    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (grid[r][c] == 0) {
                emptyCells[count].row = r;
                emptyCells[count].col = c;
                count++;
            }
        }
    }

    if (count > 0) {
        int index = rand() % count;
        grid[emptyCells[index].row][emptyCells[index].col] = 1;
        return true;
    }
    return false;
}

// 게임 오버 여부 판정 (빈 칸이 없고 합칠 수 있는 인접 타일도 없는 경우)
bool CheckGameOver(int grid[GRID_SIZE][GRID_SIZE]) {
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (grid[r][c] == 0) return false;
            if (r > 0 && grid[r][c] == grid[r - 1][c]) return false;
            if (r < GRID_SIZE - 1 && grid[r][c] == grid[r + 1][c]) return false;
            if (c > 0 && grid[r][c] == grid[r][c - 1]) return false;
            if (c < GRID_SIZE - 1 && grid[r][c] == grid[r][c + 1]) return false;
        }
    }
    return true;
}

int main(void) {
    const int screenWidth = 500;
    const int screenHeight = 670;

    InitWindow(screenWidth, screenHeight, "Raylib Merge Building Game (5x5)");
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));

    int grid[GRID_SIZE][GRID_SIZE] = { 0 };
    int score = 0;
    int selectedRow = -1;
    int selectedCol = -1;
    bool isGameOver = false;

    // 초기 건물 2개 생성
    SpawnBuilding(grid);
    SpawnBuilding(grid);

    // 하단 버튼 영역 정의
    Rectangle spawnBtn = { 50, 560, 400, 50 };

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // ------------------ 게임 로직 업데이트 ------------------
        if (!isGameOver) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                // 1. [건물 생성] 버튼 클릭 시
                if (CheckCollisionPointRec(mousePos, spawnBtn)) {
                    if (SpawnBuilding(grid)) {
                        selectedRow = -1;
                        selectedCol = -1;
                        if (CheckGameOver(grid)) isGameOver = true;
                    }
                }
                // 2. 그리드 타일 클릭 시
                else {
                    for (int r = 0; r < GRID_SIZE; r++) {
                        for (int c = 0; c < GRID_SIZE; c++) {
                            Rectangle cellRect = {
                                BOARD_OFFSET_X + c * (CELL_SIZE + CELL_PADDING),
                                BOARD_OFFSET_Y + r * (CELL_SIZE + CELL_PADDING),
                                CELL_SIZE,
                                CELL_SIZE
                            };

                            if (CheckCollisionPointRec(mousePos, cellRect)) {
                                // 기존 선택이 없는 경우 -> 타일 선택
                                if (selectedRow == -1 && selectedCol == -1) {
                                    if (grid[r][c] > 0) {
                                        selectedRow = r;
                                        selectedCol = c;
                                    }
                                }
                                // 같은 타일 재클릭 -> 선택 해제
                                else if (selectedRow == r && selectedCol == c) {
                                    selectedRow = -1;
                                    selectedCol = -1;
                                }
                                // 타일 이동 또는 합성
                                else {
                                    // 빈 공간으로 이동
                                    if (grid[r][c] == 0) {
                                        grid[r][c] = grid[selectedRow][selectedCol];
                                        grid[selectedRow][selectedCol] = 0;
                                        selectedRow = -1;
                                        selectedCol = -1;
                                    }
                                    // 동일 단계 건물 합성 (Merge)
                                    else if (grid[r][c] == grid[selectedRow][selectedCol]) {
                                        grid[r][c]++;
                                        score += grid[r][c] * 10;
                                        grid[selectedRow][selectedCol] = 0;
                                        selectedRow = -1;
                                        selectedCol = -1;
                                    }
                                    // 다른 단계 건물 클릭 시 선택 변경
                                    else {
                                        selectedRow = r;
                                        selectedCol = c;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // 재시작 키 (R)
        if (IsKeyPressed(KEY_R)) {
            for (int r = 0; r < GRID_SIZE; r++) {
                for (int c = 0; c < GRID_SIZE; c++) {
                    grid[r][c] = 0;
                }
            }
            score = 0;
            selectedRow = -1;
            selectedCol = -1;
            isGameOver = false;
            SpawnBuilding(grid);
            SpawnBuilding(grid);
        }

        // ------------------ 화면 렌더링 ------------------
        BeginDrawing();
        ClearBackground((Color){ 245, 245, 245, 255 });

        // 상단 헤더 & 점수 표시
        DrawText("MERGE TOWN", 50, 30, 32, DARKGRAY);
        DrawText(TextFormat("SCORE: %d", score), 50, 75, 22, BLUE);
        DrawText("[R] Restart", 350, 75, 18, GRAY);

        // 그리드 보드 배경 (크기 동적 연산)
        float boardWidth = GRID_SIZE * (CELL_SIZE + CELL_PADDING) - CELL_PADDING + 20;
        float boardHeight = GRID_SIZE * (CELL_SIZE + CELL_PADDING) - CELL_PADDING + 20;
        DrawRectangleRounded((Rectangle){ BOARD_OFFSET_X - 10, BOARD_OFFSET_Y - 10, boardWidth, boardHeight }, 0.05f, 4, LIGHTGRAY);

        // 5x5 타일 렌더링
        for (int r = 0; r < GRID_SIZE; r++) {
            for (int c = 0; c < GRID_SIZE; c++) {
                float posX = BOARD_OFFSET_X + c * (CELL_SIZE + CELL_PADDING);
                float posY = BOARD_OFFSET_Y + r * (CELL_SIZE + CELL_PADDING);
                Rectangle cellRect = { posX, posY, CELL_SIZE, CELL_SIZE };

                if (grid[r][c] == 0) {
                    DrawRectangleRounded(cellRect, 0.1f, 4, GRAY);
                } else {
                    Color tileColor = GetTierColor(grid[r][c]);
                    DrawRectangleRounded(cellRect, 0.1f, 4, tileColor);

                    // 선택된 타일 강조 표시 (두꺼운 테두리)
                    if (selectedRow == r && selectedCol == c) {
                        DrawRectangleRoundedLines(cellRect, 0.1f, 4, 4, RED);
                    }

                    // 건물 단계 라벨 정중앙 자동 배치
                    const char* levelText = TextFormat("Lv.%d", grid[r][c]);
                    int fontSize = 20;
                    int textWidth = MeasureText(levelText, fontSize);
                    int textOffsetX = (CELL_SIZE - textWidth) / 2;
                    int textOffsetY = (CELL_SIZE - fontSize) / 2;

                    DrawText(levelText, posX + textOffsetX, posY + textOffsetY, fontSize, (grid[r][c] >= 3) ? WHITE : DARKGRAY);
                }
            }
        }

        // 하단 건물 생성 버튼 렌더링
        Color btnColor = CheckCollisionPointRec(mousePos, spawnBtn) ? DARKBLUE : BLUE;
        DrawRectangleRounded(spawnBtn, 0.2f, 4, btnColor);
        DrawText("NEW BUILDING (SPAWN)", 115, 573, 20, WHITE);

        // 게임 오버 오버레이 렌더링
        if (isGameOver) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.75f));
            DrawText("GAME OVER!", 130, 270, 40, RED);
            DrawText(TextFormat("Final Score: %d", score), 160, 330, 24, WHITE);
            DrawText("Press [R] to Restart", 145, 380, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

---

## ⚙️ 주요 함수 및 구조 설명

- **`GetTierColor(int tier)`**: 건물 레벨(1~8+)에 해당하는 고유 RGB 색상을 반환합니다.
- **`SpawnBuilding(int grid[][])`**: 보드 내 빈 칸(`0`)의 위치를 추적하여 무작위 위치에 1단계 건물(`1`)을 배치합니다.
- **`CheckGameOver(int grid[][])`**: 보드가 가득 찼는지, 그리고 상하좌우에 동일한 단계의 건물이 존재하는지 검사하여 게임 종료 여부를 판단합니다.
- **`MeasureText()` 정렬 방식**: `MeasureText` 함수를 이용해 타일 크기에 구애받지 않고 텍스트가 항상 타일 정중앙에 배치되도록 자동 오프셋을 계산합니다.

이 리포트 및 코드는 생성형 AI (LLM)[Google Gemini]를 활용해 생성되었습니다
