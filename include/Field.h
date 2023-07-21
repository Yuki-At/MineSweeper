#ifndef MINESWEEPER_GRID_H_
#define MINESWEEPER_GRID_H_

#include <algorithm>
#include <vector>

#include "Common.h"


struct Grid {
    bool hasMine;
    bool open;
    int aroundMineCount;
    bool guessFlag;
};


class Field {
private:
    int width, height;
    std::vector<Grid> grids;
    int buriedMineCount;

public:
    Field(int width, int height)
        : width(width)
        , height(height)
        , grids(width * height)
        , buriedMineCount(0) {
    }

public:
    // N is number of mines to be buried.
    // No mines will be buried at X.
    void BuryMines(int N, std::vector<Pos> X);

    // Open the pos' grid, then in chains open the surrounding grids.
    // Return false if the grid at pos has a mine, true otherwise.
    bool OpenGrid(Pos pos);

    void Clear() { grids.assign(grids.size(), Grid()); }

    const Grid & GetGrid(Pos pos) const { return grids[GetIndex(pos)]; }

    bool IsFinished() const {
        return std::ranges::count_if(grids, [](const Grid & grid) { return !grid.open; })
            == buriedMineCount;
    }

    int GetClosedGridCount() const {
        return std::ranges::count_if(grids, [](const Grid & grid) { return grid.open; });
    };

    void ChangeGuessFlag(Pos pos) {
        if (!GetGrid(pos).open) {
            grids[GetIndex(pos)].guessFlag = !GetGrid(pos).guessFlag;
        }
    }

    int GetWidth() const { return width; }

    int GetHeight() const { return height; }

    int GetGridCount() const { return width * height; }

    bool IsExist(Pos pos) const { return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height; }

private:
    Pos GetPos(int index) const { return Pos(index % width, index / width); }

    int GetIndex(Pos pos) const { return pos.x + width * pos.y; }

};


#endif // MINESWEEPER_GRID_H_