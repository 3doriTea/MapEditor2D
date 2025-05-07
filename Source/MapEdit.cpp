#include "MapEdit.h"

MapEdit::MapEdit() :
	Frame{ 50, 50, TILE_WIDTH * EDIT_TILE_COLUMN_COUNT, TILE_HEIGHT * EDIT_TILE_ROW_COUNT, true }
{
}

MapEdit::~MapEdit()
{
}

void MapEdit::UpdateFrame()
{
}

void MapEdit::DrawFrame()
{
	for (int y = 0; y < EDIT_TILE_ROW_COUNT; y++)
	{
		for (int x = 0; x < EDIT_TILE_COLUMN_COUNT; x++)
		{
			DrawBox(
				x * TILE_WIDTH + offsetX_, y * TILE_HEIGHT + offsetY_,
				(x + 1) * TILE_WIDTH + offsetX_, (y + 1) * TILE_HEIGHT + offsetY_,
				0xffffff, FALSE);
		}
	}
}

const int MapEdit::TILE_ROW_COUNT{ 12 };
const int MapEdit::TILE_COLUMN_COUNT{ 16 };

const int MapEdit::TILE_WIDTH{ 32 };
const int MapEdit::TILE_HEIGHT{ 32 };

const int MapEdit::EDIT_TILE_COLUMN_COUNT{ 20 };
const int MapEdit::EDIT_TILE_ROW_COUNT{ 20 };
