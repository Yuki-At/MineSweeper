#include <algorithm>
#include <array>
#include <cassert>
#include <random>
#include <string>
#include <vector>

#include <DxLib.h>

#include "Common.h"
#include "Drawing.h"
#include "Field.h"


enum class GameState {
    eWaiting,
    ePlaying,
    eSucceeded,
    eFailed,
};

constexpr int ResolutionX = 960;
constexpr int ResolutionY = 960;

constexpr int FieldWidth = 16;
constexpr int FieldHeight = 16;
constexpr int MineCount = 40;

Pos GetCursorPos() {
    Pos result;
    DxLib::GetMousePoint(&result.x, &result.y);
    return result;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    DxLib::ChangeWindowMode(true);
    DxLib::SetGraphMode(ResolutionX, ResolutionY, 32);
    DxLib::SetWindowText(L"Mine Sweeper");

    if (DxLib::DxLib_Init()) {
        return -1;
    }

    const int MidiumFont = DxLib::CreateFontToHandle(L"游ゴシック", 16, -1, DX_FONTTYPE_ANTIALIASING_16X16);
    const int LargeFont = DxLib::CreateFontToHandle(L"游ゴシック", 36, -1, DX_FONTTYPE_ANTIALIASING_EDGE_16X16);
    const int GuessFlag = DxLib::LoadGraph(L"res/guess_flag.png");

    bool leftMousePressed = false;
    bool leftMouseClicked = false;
    bool rightMousePressed = false;
    bool rightMouseClicked = false;

    DxLib::SetDrawScreen(DX_SCREEN_BACK);
    DxLib::SetMouseDispFlag(TRUE);

    auto gameState = GameState::eWaiting;

    Field field(FieldWidth, FieldHeight);

    const int GridSize = std::min(ResolutionX / field.GetWidth(), ResolutionY / field.GetHeight());

    const int MarginX = ResolutionX - GridSize * field.GetWidth();
    const int MarginY = ResolutionY - GridSize * field.GetHeight();

    bool selected;
    Pos selectedGrid;

    while (!DxLib::ProcessMessage() && !DxLib::CheckHitKey(KEY_INPUT_ESCAPE)) {
        DxLib::ClearDrawScreen();

        if (DxLib::CheckHitKey(KEY_INPUT_RETURN)) {
            gameState = GameState::eWaiting;
            field.Clear();
        }

        // Calculate a selected grid from cursor position.
        const auto cursorPos = GetCursorPos();
        selectedGrid.x = (cursorPos.x - MarginX / 2 + GridSize) / GridSize - 1;
        selectedGrid.y = (cursorPos.y - MarginY / 2 + GridSize) / GridSize - 1;
        if (selectedGrid.x < 0 || selectedGrid.x >= field.GetWidth()
            || selectedGrid.y < 0 || selectedGrid.y >= field.GetHeight()) {
            selected = false;
        }
        else {
            selected = true;
        }

        // Input-related processing.
        {
            leftMouseClicked = false;
            rightMouseClicked = false;

            if (DxLib::GetMouseInput() & MOUSE_INPUT_LEFT && !leftMousePressed) {
                leftMouseClicked = true;
            }
            if (DxLib::GetMouseInput() & MOUSE_INPUT_RIGHT && !rightMousePressed) {
                rightMouseClicked = true;
            }

            leftMousePressed = DxLib::GetMouseInput() & MOUSE_INPUT_LEFT;
            rightMousePressed = DxLib::GetMouseInput() & MOUSE_INPUT_RIGHT;
        }

        if (selected && leftMouseClicked) {
            switch (gameState) {
            case GameState::eWaiting:
                {
                    std::vector<Pos> safezone = { selectedGrid };
                    std::ranges::copy_if(selectedGrid.GetSurroundingPos(), std::back_inserter(safezone)
                        , [&](const Pos & pos) { return field.IsExist(pos); });
                    field.BuryMines(MineCount, safezone);
                    gameState = GameState::ePlaying;
                }
                [[fallthrough]];
            case GameState::ePlaying:
                if (!field.OpenGrid(selectedGrid)) {
                    gameState = GameState::eFailed;
                }
                else {
                    if (field.IsFinished()) {
                        gameState = GameState::eSucceeded;
                    }
                }
                break;
            default:
                break;
            }
        }

        if (gameState == GameState::ePlaying && rightMouseClicked) {
            field.ChangeGuessFlag(selectedGrid);
        }

        // Rendering process
        for (Pos pos = { }; pos.x < field.GetWidth(); ++pos.x) {
            for (pos.y = 0; pos.y < field.GetHeight(); ++pos.y) {
                const auto & grid = field.GetGrid(pos);

                unsigned int color;
                if (grid.open) {
                    if (grid.hasMine) {
                        color = DxLib::GetColor(255, 0, 0);
                    }
                    else {
                        color = DxLib::GetColor(0, 255, 0);
                    }
                }
                else {
                    color = DxLib::GetColor(127, 127, 127);
                }

                int x = MarginX / 2 + GridSize * pos.x;
                int y = MarginY / 2 + GridSize * pos.y;
                DxLib::DrawBox(x, y, x + GridSize, y + GridSize, color, true);
                if (pos == selectedGrid) {
                    DxLib::DrawBox(x, y, x + GridSize, y + GridSize, DxLib::GetColor(0, 0, 255), false);
                }
                else {
                    DxLib::DrawBox(x, y, x + GridSize, y + GridSize, DxLib::GetColor(191, 191, 191), false);
                }

                if (grid.aroundMineCount != 0) {
                    DrawTextInBox(x, y, x + GridSize, y + GridSize
                        , std::to_wstring(grid.aroundMineCount), DxLib::GetColor(0, 0, 0), MidiumFont);
                }

                if (grid.guessFlag) {
                    DxLib::DrawExtendGraph(x, y, x + GridSize, y + GridSize, GuessFlag, true);
                }
            }
        }

        switch (gameState) {
        case GameState::eSucceeded:
            DrawTextInBox(0, 0, ResolutionX, ResolutionY, L"Success! Press enter to play again."
                , DxLib::GetColor(255, 0, 255), LargeFont);
            break;
        case GameState::eFailed:
            DrawTextInBox(0, 0, ResolutionX, ResolutionY, L"Failed! Press enter to play again."
                , DxLib::GetColor(255, 0, 255), LargeFont);
            break;
        default:
            break;
        }

        DxLib::ScreenFlip();
    }

    DxLib::DxLib_End();

    return 0;
}
