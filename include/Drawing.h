#ifndef MINESWEEPER_DRAWING_H_
#define MINESWEEPER_DRAWING_H_

#include <string>


void DrawTextInBox(int x1, int y1, int x2, int y2, std::wstring text, unsigned int color, int font = -1);

#endif // MINESWEEPER_DRAWING_H_