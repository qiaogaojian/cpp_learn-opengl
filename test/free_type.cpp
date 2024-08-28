#include "ft2build.h"
#include "freetype/freetype.h"
#include FT_FREETYPE_H

#include <iostream>
using namespace std;

int main()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    FT_Face face;
    if (FT_New_Face(ft, "res/font/msyh.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    system("pause");
    return 0;
}