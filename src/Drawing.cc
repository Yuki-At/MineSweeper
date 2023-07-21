#include "Drawing.h"

#include <vector>

#include <DxLib.h>


void DrawTextInBox(int x1, int y1, int x2, int y2, std::wstring text, unsigned int color, int font) {
    int width;
    int height;
    if (font != -1) {
        width = DxLib::GetDrawStringWidthToHandle(text.data(), sizeof(decltype(text)::value_type) * text.size(), font);
        height = DxLib::GetFontLineSpaceToHandle(font);
    }
    else {
        width = DxLib::GetDrawStringWidth(text.data(), sizeof(decltype(text)::value_type) * text.size());
        height = DxLib::GetFontLineSpace();
    }

    int drawingX = (x2 - x1 - width) / 2 + x1;
    int drawingY = (y2 - y1 - height) / 2 + y1;

    if (font != -1) {
        DxLib::DrawStringToHandle(drawingX, drawingY, text.data(), color, font, DxLib::GetColor(255, 255, 255));
    }
    else {
        DxLib::DrawString(drawingX, drawingY, text.data(), color, DxLib::GetColor(255, 255, 255));
    }

}
