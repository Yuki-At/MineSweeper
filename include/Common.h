#ifndef MINESWEEPER_COMMON_H_
#define MINESWEEPER_COMMON_H_

#include <cassert>


struct Pos {
    int x, y;

    bool operator == (const Pos & pos) const {
        return this->x == pos.x && this->y == pos.y;
    }

    // Returns adjacent positions.
    // North is 0 and increases counterclockwise.
    // e.g.) No.1: North West, No.5: South East
    std::vector<Pos> GetSurroundingPos() const {
        return {
            Pos(this->x,   this->y-1),
            Pos(this->x-1, this->y-1),
            Pos(this->x-1, this->y  ),
            Pos(this->x-1, this->y+1),
            Pos(this->x,   this->y+1),
            Pos(this->x+1, this->y+1),
            Pos(this->x+1, this->y  ),
            Pos(this->x+1, this->y-1),
        };
    }

};

#endif // MINESWEEPER_COMMON_H_